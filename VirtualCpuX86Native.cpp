#include "VirtualCpuX86Native.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <OS.h>


area_id	vm32_create_area(const char *name, void **address, uint32 addressSpec, size_t size, uint32 lock, uint32 protection);


extern char VirtualCpuX86NativeCodeBegin[];
extern char VirtualCpuX86NativeCodeEnd[];

extern "C" void VirtualCpuX86NativeEnter32(void *arg);
extern "C" void VirtualCpuX86NativeReturn();


VirtualCpuX86Native::VirtualCpuX86Native()
{
	size_t codeSize = VirtualCpuX86NativeCodeEnd - VirtualCpuX86NativeCodeBegin;
	fCodeArea.SetTo(vm32_create_area("VirtualCpuX86Code", &fCodeBase, B_ANY_ADDRESS, codeSize, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA | B_EXECUTE_AREA));
	if (!fCodeArea.IsSet()) abort();
	printf("X86:fCodeBase: %p\n", fCodeBase);
	memcpy(fCodeBase, VirtualCpuX86NativeCodeBegin, VirtualCpuX86NativeCodeEnd - VirtualCpuX86NativeCodeBegin);
	set_area_protection(fCodeArea.Get(), B_READ_AREA | B_EXECUTE_AREA);

	fRetProcAdr = (uint32)(addr_t)fCodeBase + ((char*)VirtualCpuX86NativeReturn - VirtualCpuX86NativeCodeBegin);
}


uint32 &VirtualCpuX86Native::Ip() {return fContext.ip;}
uint32 &VirtualCpuX86Native::Flags() {return fContext.flags;}
uint32 *VirtualCpuX86Native::Regs() {return fContext.regs;}
uint16 *VirtualCpuX86Native::Segs() {return fContext.segs;}
uint8 *VirtualCpuX86Native::Fpsave() {return fContext.fpsave;}

uint32 VirtualCpuX86Native::IntVec() {return fIntVec;}
uint32 VirtualCpuX86Native::IntArg() {return fIntArg;}

uint32 VirtualCpuX86Native::RetProcAdr() {return fRetProcAdr;}
VirtualCpuX86Native::StopCause VirtualCpuX86Native::Cause() {return fCause;}

void VirtualCpuX86Native::Run()
{
	if (setjmp(fRetCtx) == 0) {
		VirtualCpuX86NativeEnter32(NULL);
		abort();
	}
}
