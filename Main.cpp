#include <private/system/user_runtime.h>
#include <private/kernel/ksyscalls.h>
#include "syscall_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include <algorithm>

#include <signal.h>
#include <OS.h>
#include <image.h>
#include <TLS.h>
#include <private/system/tls.h>
#include <private/system/syscalls.h>
#include <private/system/thread_defs.h>
#include <private/system/user_thread_defs.h>
#include <private/libroot/pthread_private.h>
#include <private/system/commpage_defs.h>
#include <private/system/arch/riscv64/arch_commpage_defs.h>

#include "Loader.h"
#include "Syscalls.h"
#include "VirtualCpu.h"
#include "VirtualCpuTemu.h"
#include "VirtualCpuRvvm.h"


extern void *__gCommPageAddress;

thread_local VirtualCpu *tCpu = NULL;
thread_local bool tThreadExit = false;
thread_local status_t tThreadReturnValue = B_OK;

bool gInSignalHandler = false;


typedef VirtualCpuTemu VirtualCpuDefault;


area_id	vm32_create_area(const char *name, void **address, uint32 addressSpec, size_t size, uint32 lock, uint32 protection)
{
	if (addressSpec != B_ANY_ADDRESS) {
		return _kern_create_area(name, address, addressSpec, size, lock, protection);
	}
	// crazy random allocator :D
	size = (size + (B_PAGE_SIZE - 1)) / B_PAGE_SIZE * B_PAGE_SIZE;
	for (int i = 0; ; i++) {
		void *address32 = (void*)(rand() % (0xffffffff - size + 1));
		area_id area = _kern_create_area(name, &address32, B_EXACT_ADDRESS, size, lock, protection);
		if (area >= B_OK || !(i < 100)) {
			*address = address32;
			return area;
		}
	}
}

void *GetImageBase(const char *name)
{
	int32 cookie = 0;
	image_info info;
	while (get_next_image_info(B_CURRENT_TEAM, &cookie, &info) >= B_OK) {
		if (strcmp(basename(info.name), name) == 0) return info.text;
	}
	return NULL;
}

void WritePC(uint64 pc)
{
	int32 cookie = 0;
	image_info info;
	while (get_next_image_info(B_CURRENT_TEAM, &cookie, &info) >= B_OK) {
		if (pc >= (addr_t)info.text && pc <= (addr_t)info.text + info.text_size - 1) {
			printf("<%s> %#" B_PRIx64, info.name, pc - (addr_t)info.text);
			return;
		}
	}
	printf("%#" B_PRIx64, pc);
}

void StackTrace()
{
	uint64 pc = tCpu->Pc();
	uint64 sp = tCpu->Regs()[2];
	uint64 fp = tCpu->Regs()[8];
	
	for (;;) {
		printf("FP: %#" B_PRIx64 ", PC: ", fp); WritePC(pc); printf("\n");
		if (fp == 0) break;
		pc = *((uint64*)fp - 1);
		fp = *((uint64*)fp - 2);
	}
}

static void HaikuTrap()
{
	//printf("HaikuTrap: Hart %p trap at %#08" PRIxXLEN ", cause %x, tval %#08" PRIxXLEN "\n", tCpu, tCpu->Pc(), tCpu->Cause(), tCpu->Tval());
	switch (tCpu->Cause()) {
		case -1:
			break;
		case TRAP_ENVCALL_MMODE: {
			uint64 syscall = tCpu->Regs()[5];
			//printf("syscall %" B_PRIu64 "(%s)\n", syscall, syscall < (uint64)kSyscallCount ? kExtendedSyscallInfos[syscall].name : "?");

			uint64 args[20];
			if (syscall < (uint64)kSyscallCount) {
				uint32 argCnt = kExtendedSyscallInfos[syscall].parameter_count;
				memcpy(&args[0], &tCpu->Regs()[10], sizeof(uint64)*std::min<uint32>(argCnt, 8));
				if (argCnt > 8) {
					memcpy(&args[8], (void*)tCpu->Regs()[2], sizeof(uint64)*(argCnt - 8));
				}
			}

			DispatchSyscall(syscall, args, &tCpu->Regs()[10]);
			break;
		}
		default:
			printf("[!] unhandled trap: Hart %p trap at %#08" PRIxXLEN ", cause %x, tval %#08" PRIxXLEN "\n", tCpu, tCpu->Pc(), tCpu->Cause(), tCpu->Tval());
			StackTrace();
			fgetc(stdin);
			_exit(1);
	}
}

status_t HaikuThreadStart(pthread_t pthread, uint64 entry, uint64 arg1, uint64 arg2)
{
	size_t stackSize = 0x100000;
	void *stack;
	AreaDeleter stackArea(create_area("thread", &stack, B_ANY_ADDRESS, stackSize, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA));

	user_thread userThread {
		.pthread = pthread
	};

	uint64 tls[TLS_MAX_KEYS]{};
	tls[TLS_BASE_ADDRESS_SLOT] = (addr_t)&tls;
	tls[TLS_THREAD_ID_SLOT] = find_thread(NULL);
	tls[TLS_USER_THREAD_SLOT] = (addr_t)&userThread;

	static uint32 threadExitCode[] = {
		0x03800293, // addi	t0, zero, 56 // _kern_exit_thread
		0x00000073, // ecall
	};

	VirtualCpuDefault cpu;
	tCpu = &cpu;
	cpu.Regs()[1] = (addr_t)&threadExitCode;
	cpu.Regs()[2] = (addr_t)stack + stackSize;
	cpu.Regs()[4] = (addr_t)&tls;
	cpu.Regs()[10] = arg1;
	cpu.Regs()[11] = arg2;
	cpu.Pc() = entry;
	while (!tThreadExit) {
		cpu.Run();
		HaikuTrap();
	}
	tCpu = NULL;

	return tThreadReturnValue;
}

int32 ThreadEntry(void *arg)
{
	ObjectDeleter<thread_creation_attributes> attributes((thread_creation_attributes*)arg);
	return HaikuThreadStart((pthread_t)attributes->args2, (addr_t)attributes->entry, (addr_t)attributes->args1, (addr_t)attributes->args2);
}

thread_id vm_spawn_thread(struct thread_creation_attributes* attributes)
{
	ObjectDeleter<struct thread_creation_attributes> guestAttrs(new struct thread_creation_attributes());
	memcpy(guestAttrs.Get(), attributes, sizeof(thread_creation_attributes));
	thread_id thread = spawn_thread(ThreadEntry, attributes->name, attributes->priority, guestAttrs.Detach());
	return thread;
}

void vm_exit_thread(status_t returnValue)
{
	tThreadExit = true;
	tThreadReturnValue = returnValue;
}

void SignalHandler(int sig, siginfo_t *info, ucontext_t *ctx, void *arg)
{
	printf("SignalHandler(%d)\n", sig);
	if (gInSignalHandler) {
		exit(0);
	}
	gInSignalHandler = true;
	StackTrace();
	gInSignalHandler = false;
	exit(0);
}

void BuildArgs(ArrayDeleter<uint8> &mem, user_space_program_args &args, char **argv, char **env)
{
	size_t argCnt = 0;
	while (argv[argCnt] != NULL) argCnt++;
	size_t envCnt = 0;
	while (env[envCnt] != NULL) envCnt++;
	size_t argSize = 0;
	for (size_t i = 0; i < argCnt; i++) {
		argSize += strlen(argv[i]) + 1;
	}
	for (size_t i = 0; i < envCnt; i++) {
		argSize += strlen(env[i]) + 1;
	}
	
	mem.SetTo(new uint8[sizeof(void*)*(argCnt + envCnt + 2) + argSize]);
	char **outArgs = (char**)&mem[0];
	char *outChars = (char*)&mem[sizeof(void*)*(argCnt + envCnt + 2)];
	for (size_t i = 0; i < argCnt; i++) {
		size_t len = strlen(argv[i]) + 1;
		*outArgs = outChars; outArgs++;
		memcpy(outChars, argv[i], len);
		outChars += len;
	}
	*outArgs = NULL; outArgs++;
	for (size_t i = 0; i < envCnt; i++) {
		size_t len = strlen(env[i]) + 1;
		*outArgs = outChars; outArgs++;
		memcpy(outChars, env[i], len);
		outChars += len;
	}
	*outArgs = NULL; outArgs++;
	
	char buf[B_PATH_NAME_LENGTH];
	strcpy(buf, argv[0]);
	strcpy(args.program_name, basename(buf));
	strcpy(args.program_path, argv[0]);

	args.error_port = -1;
	args.arg_count = argCnt;
	args.env_count = envCnt;
	args.args = (char**)&mem[0];
	args.env = (char**)&mem[0] + (argCnt + 1);
}

int main(int argc, char **argv)
{
	srand(time(NULL));

	ObjectDeleter<ElfImage> image(ElfImage::Load("../runtime_loader.riscv64"));

	user_space_program_args args{};
	ArrayDeleter<uint8> argsMem;
	char *env[] = {"A=1", NULL};
	BuildArgs(argsMem, args, argv + 1, env);

	void *commpage = __gCommPageAddress;
#if 0
	AreaDeleter stackArea(create_area("guest commpage", &commpage, B_ANY_ADDRESS, COMMPAGE_SIZE, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA));
	((uint64*)commpage)[COMMPAGE_ENTRY_MAGIC] = COMMPAGE_SIGNATURE;
	((uint64*)commpage)[COMMPAGE_ENTRY_VERSION] = COMMPAGE_VERSION;
	((uint64*)commpage)[COMMPAGE_ENTRY_REAL_TIME_DATA] = 0;
#endif

#if 0
	int signals[] = {SIGILL, SIGSEGV};
	struct sigaction oldAction[B_COUNT_OF(signals)];
	struct sigaction action {
		.sa_sigaction = (__siginfo_handler_t)SignalHandler,
		.sa_flags = SA_SIGINFO
	};
	for (int i = 0; i < B_COUNT_OF(signals); i++) {
		sigaction(signals[i], &action, &oldAction[i]);
	}
#endif

	status_t res = HaikuThreadStart(NULL, (addr_t)image->GetEntry(), (addr_t)&args, (addr_t)commpage);

#if 0
	for (int i = 0; i < B_COUNT_OF(signals); i++) {
		sigaction(signals[i], &oldAction[i], NULL);
	}
#endif

	return res;
}
