#include <stdio.h>
#include <stdlib.h>

#include <OS.h>

#include "Loader.h"
#include "VirtualCpuX86Native.h"

area_id	vm32_create_area(const char *name, void **address, uint32 addressSpec, size_t size, uint32 lock, uint32 protection);

void WritePC(uint64 pc);


static bool TrapHandler(VirtualCpuX86Native &cpu)
{
	uint32 op = *((uint32*)cpu.Regs()[4] + 1);
	//printf("op: %#" B_PRIx32 "\n", op);
	switch (op) {
		case 1:
			return true;
			break;
		case 2:
			printf("%s", (const char *)(*((uint32*)cpu.Regs()[4] + 2)));
			break;
		default:
			abort();
	}
	return false;
}


void VirtualCpuX86Test()
{
	for (int i = 0; i < 1; i++) {
		printf("+VirtualCpuX86Test\n");
	
		ObjectDeleter<ElfImage> image(ElfImage::Load("../TestX86"));
	
		size_t stackSize = 0x100000;
		uint8 *stack;
		AreaDeleter stackArea(vm32_create_area("thread", (void**)&stack, B_ANY_ADDRESS, stackSize, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA));
		
		VirtualCpuX86Native cpu;
	
		printf("image.GetImageBase(): %p\n", image->GetImageBase());
		printf("cpu.RetProcAdr(): %#" B_PRIx32 "\n", cpu.RetProcAdr());
		printf("cpu.RetProcArg(): %#" B_PRIx32 "\n", cpu.RetProcArg());

		void *symAdr;
		if (!image->FindSymbol("gRetProc", &symAdr, NULL)) abort();
		*(uint32*)symAdr = cpu.RetProcAdr();
		if (!image->FindSymbol("gRetProcArg", &symAdr, NULL)) abort();
		*(uint32*)symAdr = cpu.RetProcArg();

		uint32 *sp = (uint32*)(stack + stackSize);

		cpu.Ip() = (uint32)(addr_t)image->GetEntry();
		cpu.Regs()[4] = (uint32)(addr_t)sp;
	
		printf("IP: "); WritePC(cpu.Ip()); printf("\n");
		printf("SP: %#" B_PRIx32 "\n", cpu.Regs()[4]);
	
		printf("+Run()\n");
		while (true) {
			cpu.Run();
			if (TrapHandler(cpu)) break;
		}
		printf("-Run()\n");
	
		printf("IP: "); WritePC(cpu.Ip()); printf("\n");
		printf("SP: %#" B_PRIx32 "\n", cpu.Regs()[4]);
	
		printf("-VirtualCpuX86Test\n");
	}
}
