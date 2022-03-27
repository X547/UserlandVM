extern "C" {
#include "rvvm/rvvm.h"
#include "rvvm/riscv_hart.h"
#include "rvvm/riscv_csr.h"
}

#include <private/system/user_runtime.h>
#include <private/kernel/ksyscalls.h>
#include "syscall_table.h"
#include "Loader.h"
#include "Syscalls.h"

#include <stdio.h>
#include <string.h>
#include <libgen.h>
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

extern void *__gCommPageAddress;

thread_local rvvm_hart_t *tVm = NULL;
bool gInSignalHandler = false;


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
	uint64 pc = tVm->registers[REGISTER_PC];
	uint64 sp = tVm->registers[REGISTER_X2];
	uint64 fp = tVm->registers[REGISTER_X8];
	
	for (;;) {
		printf("FP: %#" B_PRIx64 ", PC: ", fp); WritePC(pc); printf("\n");
		if (fp == 0) break;
		pc = *((uint64*)fp - 1);
		fp = *((uint64*)fp - 2);
	}
}

extern "C" void riscv_trap(rvvm_hart_t* vm, bitcnt_t cause, maxlen_t tval)
{
	//rvvm_info("Hart %p trap at %#08" PRIxXLEN ", cause %x, tval %#08" PRIxXLEN, vm, vm->registers[REGISTER_PC], cause, tval);
	switch (cause) {
		case TRAP_ENVCALL_MMODE: {
			uint64 syscall = vm->registers[REGISTER_X5];
			//printf("syscall %" B_PRIu64 "(%s)\n", syscall, syscall < (uint64)kSyscallCount ? kExtendedSyscallInfos[syscall].name : "?");

			uint64 args[20];
			if (syscall < (uint64)kSyscallCount) {
				uint32 argCnt = kExtendedSyscallInfos[syscall].parameter_count;
				memcpy(&args[0], &vm->registers[REGISTER_X10], sizeof(uint64)*std::min<uint32>(argCnt, 8));
				if (argCnt > 8) {
					memcpy(&args[8], (void*)vm->registers[REGISTER_X2], sizeof(uint64)*(argCnt - 8));
				}
			}

			DispatchSyscall(syscall, args, &vm->registers[REGISTER_X10]);
			break;
		}
		default:
			printf("[!] unhandled trap\n");
			StackTrace();
			fgetc(stdin);
			_exit(1);
	}
}

int32 ThreadEntry(void *arg)
{
	ObjectDeleter<thread_creation_attributes> attributes((thread_creation_attributes*)arg);
	//printf("ThreadEntry\n");
	//printf("  entry: %p\n", attributes->entry);
	//printf("  args1: %p\n", attributes->args1);
	//printf("  args2: %p\n", attributes->args2);

	size_t stackSize = 0x100000;
	void *stack;
	AreaDeleter stackArea(create_area("image", &stack, B_ANY_ADDRESS, stackSize, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA));

	user_thread userThread {
		.pthread = (pthread_t)attributes->args2
	};

	uint64 tls[TLS_MAX_KEYS]{};
	tls[TLS_BASE_ADDRESS_SLOT] = (addr_t)&tls;
	tls[TLS_THREAD_ID_SLOT] = find_thread(NULL);
	tls[TLS_USER_THREAD_SLOT] = (addr_t)&userThread;

	static uint32 threadExitCode[] = {
		0x03800293, // addi	t0, zero, 56 // _kern_exit_thread
		0x00000073, // ecall
	};

	rvvm_machine_t machine{};
	rvvm_hart_t vm{};
	tVm = &vm;
	riscv_hart_init(&vm, true);
	vm.machine = &machine;
	vm.mem = rvvm_ram_t{
		.begin = 0x1000,
		.size = (paddr_t)(-0x1000),
		.data = (vmptr_t)0x1000
	};
	maxlen_t mstatus = 0xA00000000 + (FS_INITIAL << 13);
	riscv_csr_op(&vm, 0x300, &mstatus, CSR_SWAP);
	vm.registers[REGISTER_X1] = (addr_t)&threadExitCode;
	vm.registers[REGISTER_X2] = (addr_t)stack + stackSize;
	vm.registers[REGISTER_X4] = (addr_t)&tls;
	vm.registers[REGISTER_X10] = (addr_t)attributes->args1;
	vm.registers[REGISTER_X11] = (addr_t)attributes->args2;
	vm.registers[REGISTER_PC] = (addr_t)attributes->entry;
	attributes.Unset();
	riscv_hart_run(&vm);

	return vm.registers[REGISTER_X10];
}

thread_id vm_spawn_thread(struct thread_creation_attributes* attributes)
{
	//printf("vm_spawn_thread\n");
	//printf("  entry: %p\n", attributes->entry);
	//printf("  args1: %p\n", attributes->args1);
	//printf("  args2: %p\n", attributes->args2);
	ObjectDeleter<struct thread_creation_attributes> guestAttrs(new struct thread_creation_attributes());
	memcpy(guestAttrs.Get(), attributes, sizeof(thread_creation_attributes));
	thread_id thread = spawn_thread(ThreadEntry, attributes->name, attributes->priority, guestAttrs.Detach());
	return thread;
}

void SignalHandler(int sig, siginfo_t *info, ucontext_t *ctx, void *arg)
{
	printf("SignalHandler\n");
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
	rvvm_set_loglevel(LOG_INFO);

	ObjectDeleter<ElfImage> image(ElfImage::Load("../runtime_loader"));

	size_t stackSize = 0x100000;
	void *stack;
	AreaDeleter stackArea(create_area("image", &stack, B_ANY_ADDRESS, stackSize, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA));

	user_space_program_args args{};
	ArrayDeleter<uint8> argsMem;
	char *env[] = {"A=1", NULL};
	BuildArgs(argsMem, args, argv + 1, env);

	void *commpage = __gCommPageAddress;

	uint64 tls[TLS_MAX_KEYS]{};
	tls[TLS_BASE_ADDRESS_SLOT] = (addr_t)&tls;
	tls[TLS_THREAD_ID_SLOT] = find_thread(NULL);
	tls[TLS_USER_THREAD_SLOT] = (addr_t)tls_get(TLS_USER_THREAD_SLOT);

	struct sigaction oldAction;
	struct sigaction action {
		.sa_sigaction = (__siginfo_handler_t)SignalHandler,
		.sa_flags = SA_SIGINFO
	};
	sigaction(SIGSEGV, &action, &oldAction);

	rvvm_machine_t machine{};
	rvvm_hart_t vm{};
	tVm = &vm;
	riscv_hart_init(&vm, true);
	vm.machine = &machine;
	vm.mem = rvvm_ram_t{
		.begin = 0x1000,
		.size = (paddr_t)(-0x1000),
		.data = (vmptr_t)0x1000
	};
	maxlen_t mstatus = 0xA00000000 + (FS_INITIAL << 13);
	riscv_csr_op(&vm, 0x300, &mstatus, CSR_SWAP);
	vm.registers[REGISTER_X2] = (addr_t)stack + stackSize;
	vm.registers[REGISTER_X4] = (addr_t)&tls;
	vm.registers[REGISTER_X10] = (addr_t)&args;
	vm.registers[REGISTER_X11] = (addr_t)commpage;
	vm.registers[REGISTER_PC] = (addr_t)image->GetEntry();
	riscv_hart_run(&vm);

	sigaction(SIGSEGV, &oldAction, NULL);

	return 0;
}
