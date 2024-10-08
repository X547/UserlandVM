#include <private/system/user_runtime.h>
#include <private/kernel/ksyscalls.h>
#include "syscall_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include <algorithm>
#include <dlfcn.h>

#include <signal.h>
#include <OS.h>
#include <image.h>
#include <TLS.h>
#include <private/system/tls.h>
#include <private/system/syscalls.h>
#include <private/system/thread_defs.h>
#include <private/system/user_thread_defs.h>
#include <private/libroot/pthread_private.h>
#include <private/libroot/libroot_private.h>
#include <private/system/commpage_defs.h>
#include <private/system/arch/riscv64/arch_commpage_defs.h>
#include <private/system/real_time_data.h>

#include "Loader.h"
#include "Syscalls.h"
#include "VirtualCpuRiscV.h"

#define CheckRet(err) {status_t _err = (err); if (_err < B_OK) return _err;}


struct riscv64_real_time_data {
	bigtime_t system_time_offset;
	uint64    system_time_conversion_factor;
};


static VirtualCpuRiscV *(*sInstantiateVirtualCpuRiscv)();

extern void *__gCommPageAddress;

thread_local VirtualCpuRiscV *tCpu = NULL;
thread_local bool tThreadExit = false;
thread_local status_t tThreadReturnValue = B_OK;

bool gInSignalHandler = false;


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
		case causeMEcall:
		case causeSEcall:
		case causeUEcall:
		{
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
			printf("[!] unhandled trap: Hart %p trap at %#08" B_PRIx64 ", cause %x, tval %#08" B_PRIx64 "\n", tCpu, tCpu->Pc(), tCpu->Cause(), tCpu->Tval());
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
		0x03b00293, // addi	t0, zero, 59 // _kern_exit_thread
		0x00000073, // ecall
	};

	ObjectDeleter<VirtualCpuRiscV> cpu(sInstantiateVirtualCpuRiscv());
	tCpu = cpu.Get();
	cpu->Regs()[1] = (addr_t)&threadExitCode;
	cpu->Regs()[2] = (addr_t)stack + stackSize;
	cpu->Regs()[4] = (addr_t)&tls;
	cpu->Regs()[10] = arg1;
	cpu->Regs()[11] = arg2;
	cpu->Pc() = entry;
	while (!tThreadExit) {
		cpu->Run();
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


// #pragma mark - syscall overrides

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

thread_id vm_fork()
{
	VirtualCpuRiscV *cpu = tCpu;
	thread_id res = fork();
	if (res == 0) tCpu = cpu;
	return res;
}

static const char sAppPath[] = "UserlandVM";

thread_id vm_load_image(const char* const* flatArgs, size_t flatArgsSize, int32 argCount, int32 envCount, int32 priority, uint32 flags, port_id errorPort, uint32 errorToken)
{
	printf("[!] vm_load_image\n");
	char** outFlatArgs;
	size_t outFlatSize;
	int32 outEnvCount = envCount;
	{
		ArrayDeleter<const char*> args2(new(std::nothrow) const char*[argCount + 2]);
		if (!args2.IsSet()) return B_NO_MEMORY;
		args2[0] = sAppPath;
		memcpy(&args2[1], flatArgs, sizeof(void*)*(argCount + 1));
		CheckRet(__flatten_process_args(args2.Get(), argCount + 1, flatArgs + argCount + 1, &outEnvCount, NULL, &outFlatArgs, &outFlatSize));
	}
	status_t res = _kern_load_image(outFlatArgs, outFlatSize, argCount + 1, outEnvCount, priority, flags, errorPort, errorToken);
	free(outFlatArgs);
	return res;
}

thread_id vm_exec(const char *path, const char* const* flatArgs, size_t flatArgsSize, int32 argCount, int32 envCount, mode_t umask)
{
	char** outFlatArgs;
	size_t outFlatSize;
	int32 outEnvCount = envCount;
	{
		ArrayDeleter<const char*> args2(new(std::nothrow) const char*[argCount + 2]);
		if (!args2.IsSet()) return B_NO_MEMORY;
		args2[0] = sAppPath;
		memcpy(&args2[1], flatArgs, sizeof(void*)*(argCount + 1));
		CheckRet(__flatten_process_args(args2.Get(), argCount + 1, flatArgs + argCount + 1, &outEnvCount, NULL, &outFlatArgs, &outFlatSize));
	}
	status_t res = _kern_exec(sAppPath, outFlatArgs, outFlatSize, argCount + 1, outEnvCount, umask);
	free(outFlatArgs);
	return res;
}

status_t vm_sigaction(int sig, const struct sigaction *action, struct sigaction *oldAction)
{
	//printf("[!] vm_sigaction: unimplemented\n");
	return B_OK;
}


// #pragma mark -

void SignalHandler(int sig, siginfo_t *info, ucontext_t *ctx, void *arg)
{
	printf("SignalHandler(%d)\n", sig);
	printf("PC: %#" B_PRIx64 "\n", tCpu->Pc());
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

void VirtualCpuX86Test();

int main(int argc, char **argv, char **env)
{
	srand(time(NULL));

	char **curArgv = argv;
	char **endArgv = argv + argc;

	// skip program self name
	curArgv++;

	if (endArgv - curArgv <= 0) {
		VirtualCpuX86Test();
		return 0;
	}

	CObjectDeleter<void, int, dlclose> virtualCpuAddon;
	const char *addonName = "./librvvm2.so";
	if (endArgv - curArgv >= 1 && strcmp(curArgv[0], "--engine") == 0) {
		curArgv++;
		if (endArgv - curArgv <= 0) {
			fprintf(stderr, "argument missing\n");
			return 1;
		}
		addonName = curArgv[0];
		curArgv++;
	}
	virtualCpuAddon.SetTo(dlopen(addonName, 0));
	if (!virtualCpuAddon.IsSet()) {
		fprintf(stderr, "can't load virtual CPU add-on \"%s\"\n", addonName);
		return 1;
	}
	*(void**)&sInstantiateVirtualCpuRiscv = dlsym(virtualCpuAddon.Get(), "instantiate_virtual_cpu_riscv");
	if (sInstantiateVirtualCpuRiscv == NULL) {
		fprintf(stderr, "bad virtual CPU add-on \"%s\"\n", addonName);
		return 1;
	}

	ObjectDeleter<ElfImage> image(ElfImage::Load("../runtime_loader.riscv64"));

	user_space_program_args args{};
	ArrayDeleter<uint8> argsMem;
	//char *env[] = {"A=1", NULL};
	BuildArgs(argsMem, args, curArgv, env);

	void *hostCommpage = __gCommPageAddress;
	real_time_data *hostRtData = (real_time_data*)(((uint64*)hostCommpage)[COMMPAGE_ENTRY_REAL_TIME_DATA] + (uint8*)hostCommpage);
	
	void *commpage;
	AreaDeleter commpageArea(create_area("guest commpage", &commpage, B_ANY_ADDRESS, COMMPAGE_SIZE, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA));

	riscv64_real_time_data *rtData = (riscv64_real_time_data*)((uint64*)commpage + COMMPAGE_TABLE_ENTRIES);
	rtData->system_time_offset = hostRtData->arch_data.system_time_offset;
	rtData->system_time_conversion_factor = 1ULL << 32;

	((uint64*)commpage)[COMMPAGE_ENTRY_MAGIC] = COMMPAGE_SIGNATURE;
	((uint64*)commpage)[COMMPAGE_ENTRY_VERSION] = COMMPAGE_VERSION;
	((uint64*)commpage)[COMMPAGE_ENTRY_REAL_TIME_DATA] = (uint8*)rtData - (uint8*)commpage;

	int signals[] = {/*SIGILL, SIGSEGV,*/ 0};
	struct sigaction oldAction[B_COUNT_OF(signals)];
	struct sigaction action {
		.sa_sigaction = (__siginfo_handler_t)SignalHandler,
		.sa_flags = SA_SIGINFO
	};
	for (int i = 0; signals[i] != 0; i++) {
		sigaction(signals[i], &action, &oldAction[i]);
	}

	status_t res = HaikuThreadStart(NULL, (addr_t)image->GetEntry(), (addr_t)&args, (addr_t)commpage);

	for (int i = 0; signals[i] != 0; i++) {
		sigaction(signals[i], &oldAction[i], NULL);
	}

	return res;
}
