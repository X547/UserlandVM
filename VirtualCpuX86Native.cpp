#include "VirtualCpuX86Native.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <OS.h>

#define DPL_USER 3
#define DT_CODE_EXECUTE_ONLY 0x8
#define DT_CODE_DATA_SEGMENT 1
#include <private/kernel/arch/x86/64/descriptors.h>
#include <private/system/arch/x86/arch_thread_defs.h>
#include <private/system/syscalls.h>


area_id	vm32_create_area(const char *name, void **address, uint32 addressSpec, size_t size, uint32 lock, uint32 protection);


extern char VirtualCpuX86NativeCodeBegin[];
extern char VirtualCpuX86NativeCodeEnd[];

extern "C" void VirtualCpuX86NativeSwitch32(void *state);
extern "C" void VirtualCpuX86NativeLoadContext();
extern "C" void VirtualCpuX86NativeSaveContext();
extern "C" void VirtualCpuX86NativeReturn();


static void SetGsBase(uint64 gsBase)
{
	_kern_generic_syscall(THREAD_SYSCALLS, THREAD_SET_GS_BASE, &gsBase, sizeof(gsBase));
}

static void *GetCurState()
{
	void* state;
	__asm__ __volatile__ ("movq	%%gs:0, %0" : "=r" (state));
	return state;
}


VirtualCpuX86Native::VirtualCpuX86Native()
{
	// TODO: 32 code area can be created once for all VirtualCpu instances
	size_t codeSize = VirtualCpuX86NativeCodeEnd - VirtualCpuX86NativeCodeBegin;
	fCodeArea.SetTo(vm32_create_area("VirtualCpuX86Code", &fCodeBase, B_ANY_ADDRESS, codeSize, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA | B_EXECUTE_AREA));
	if (!fCodeArea.IsSet()) abort();
	printf("X86:fCodeBase: %p\n", fCodeBase);
	memcpy(fCodeBase, VirtualCpuX86NativeCodeBegin, VirtualCpuX86NativeCodeEnd - VirtualCpuX86NativeCodeBegin);
	set_area_protection(fCodeArea.Get(), B_READ_AREA | B_EXECUTE_AREA);

	fRetProcAdr = (uint32)(addr_t)fCodeBase + ((char*)VirtualCpuX86NativeSaveContext - VirtualCpuX86NativeCodeBegin);

	fStateArea.SetTo(vm32_create_area("VirtualCpuX86State", (void**)&fState, B_ANY_ADDRESS, codeSize, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA));
	if (!fStateArea.IsSet()) abort();
	printf("X86:fState: %p\n", fState);
	fState->self = fState;
	fState->ip = (addr_t)fCodeBase + ((char*)VirtualCpuX86NativeLoadContext - VirtualCpuX86NativeCodeBegin);
	fState->cs = USER32_CODE_SELECTOR;
	fState->retIp = (uint32)(addr_t)fCodeBase + ((char*)VirtualCpuX86NativeReturn - VirtualCpuX86NativeCodeBegin);
	fState->retCs = USER_CODE_SELECTOR;
	fState->ctx = {};
	fState->longjmpAdr = (void*)longjmp;

#if 0
	printf("offsetof(State, ip): %#" B_PRIx32 "\n", offsetof(State, ip));
	printf("offsetof(State, retIp): %#" B_PRIx32 "\n", offsetof(State, retIp));
	printf("offsetof(State, ctx): %#" B_PRIx32 "\n", offsetof(State, ctx));
	printf("offsetof(State, retCtx): %#" B_PRIx32 "\n", offsetof(State, retCtx));
	printf("offsetof(State, longjmpAdr): %#" B_PRIx32 "\n", offsetof(State, longjmpAdr));

	printf("offsetof(Context, regs): %#" B_PRIx32 "\n", offsetof(Context, regs));

	printf("USER_DATA_SELECTOR: %#" B_PRIx32 "\n", USER_DATA_SELECTOR);
#endif
}


uint32 &VirtualCpuX86Native::Ip() {return fState->ctx.ip;}
uint32 &VirtualCpuX86Native::Flags() {return fState->ctx.flags;}
uint32 *VirtualCpuX86Native::Regs() {return fState->ctx.regs;}
uint16 *VirtualCpuX86Native::Segs() {return fState->ctx.segs;}
uint8 *VirtualCpuX86Native::Fpsave() {return fState->ctx.fpsave;}

uint32 VirtualCpuX86Native::IntVec() {return fIntVec;}
uint32 VirtualCpuX86Native::IntArg() {return fIntArg;}

uint32 VirtualCpuX86Native::RetProcAdr() {return fRetProcAdr;}
uint32 VirtualCpuX86Native::RetProcArg() {return (uint32)(addr_t)fState;}
VirtualCpuX86Native::StopCause VirtualCpuX86Native::Cause() {return fCause;}

void VirtualCpuX86Native::Run()
{
	//SetGsBase((uint64)(addr_t)fState);
	//printf("  GetCurState(): %p\n", GetCurState());
	if (setjmp(fState->retCtx) == 0) {
		VirtualCpuX86NativeSwitch32(fState);
	}
	fCause = StopCause::retProc;
}
