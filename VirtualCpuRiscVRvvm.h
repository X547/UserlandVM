#pragma once

#include "VirtualCpuRiscV.h"

extern "C" {
#include "rvvm/rvvm.h"
#include "rvvm/riscv_hart.h"
#include "rvvm/riscv_csr.h"
#include "rvvm/atomics.h"
}


extern "C" void riscv_trap(rvvm_hart_t* vm, bitcnt_t cause, maxlen_t tval);

class VirtualCpuRiscVRvvm: public VirtualCpuRiscV {
private:
	friend void riscv_trap(rvvm_hart_t* vm, bitcnt_t cause, maxlen_t tval);

	rvvm_machine_t fMachine{};
	rvvm_hart_t fVm{};
	int fCause = -1;
	uint64 fTval{};

public:
	VirtualCpuRiscVRvvm();
	virtual ~VirtualCpuRiscVRvvm();
	uint64 &Pc() final;
	uint64 *Regs() final;
	double *FRegs() final;
	int Cause() final;
	uint64 Tval() final;
	void Run() final;
};
