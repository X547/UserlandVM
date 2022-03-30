#include "VirtualCpuRvvm.h"


extern "C" void riscv_trap(rvvm_hart_t* vm, bitcnt_t cause, maxlen_t tval)
{
	//printf("Hart %p trap at %#08" PRIxXLEN ", cause %x, tval %#08" PRIxXLEN "\n", vm, vm->registers[REGISTER_PC], cause, tval);
	VirtualCpuRvvm *cpu = (VirtualCpuRvvm*)((uint8*)vm - offsetof(VirtualCpuRvvm, fVm));
	cpu->fCause = cause;
	cpu->fTval = tval;
	atomic_or_uint32(&vm->pending_events, EXT_EVENT_PAUSE);
	riscv_restart_dispatch(vm);
}

VirtualCpuRvvm::VirtualCpuRvvm()
{
	riscv_hart_init(&fVm, true);
	fVm.machine = &fMachine;
	fVm.mem = rvvm_ram_t{
		.begin = 0x1000,
		.size = (paddr_t)(-0x1000),
		.data = (vmptr_t)0x1000
	};
	maxlen_t mstatus = 0xA00000000 + (FS_INITIAL << 13);
	riscv_csr_op(&fVm, 0x300, &mstatus, CSR_SWAP);
}

VirtualCpuRvvm::~VirtualCpuRvvm()
{
}

uint64 &VirtualCpuRvvm::Pc() {return fVm.registers[REGISTER_PC];}
uint64 *VirtualCpuRvvm::Regs() {return &fVm.registers[REGISTER_X0];}
double *VirtualCpuRvvm::FRegs() {return &fVm.fpu_registers[0];}
int VirtualCpuRvvm::Cause() {return fCause;}
uint64 VirtualCpuRvvm::Tval() {return fTval;}

void VirtualCpuRvvm::Run()
{
	fCause = -1;
	riscv_hart_run(&fVm);
}
