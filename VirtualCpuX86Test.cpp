#include <stdio.h>
#include <stdlib.h>

#include <OS.h>

#include "Loader.h"
#include "VirtualCpuX86Native.h"

area_id	vm32_create_area(const char *name, void **address, uint32 addressSpec, size_t size, uint32 lock, uint32 protection);


void VirtualCpuX86Test()
{
	printf("+VirtualCpuX86Test\n");

	ObjectDeleter<ElfImage> image(ElfImage::Load("../TestX86"));

	size_t stackSize = 0x100000;
	uint8 *stack;
	AreaDeleter stackArea(vm32_create_area("thread", (void**)&stack, B_ANY_ADDRESS, stackSize, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA));
	
	VirtualCpuX86Native cpu;
	
	uint32 *sp = (uint32*)(stack + stackSize);
	sp--; *sp = 0xabcd;
	sp--; *sp = cpu.RetProcAdr();

	cpu.Ip() = (uint32)(addr_t)image->GetEntry();
	cpu.Regs()[4] = (uint32)(addr_t)sp;
	cpu.Run();

	printf("-VirtualCpuX86Test\n");
}
