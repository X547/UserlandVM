#pragma once

#include <SupportDefs.h>


enum {
	modeU = 0,
	modeS = 1,
	modeM = 3,
};

enum {
	causeExecMisalign     = 0,
	causeExecAccessFault  = 1,
	causeIllegalInst      = 2,
	causeBreakpoint       = 3,
	causeLoadMisalign     = 4,
	causeLoadAccessFault  = 5,
	causeStoreMisalign    = 6,
	causeStoreAccessFault = 7,
	causeECall            = 8,
	causeUEcall           = causeECall + modeU,
	causeSEcall           = causeECall + modeS,
	causeMEcall           = causeECall + modeM,
	causeExecPageFault    = 12,
	causeLoadPageFault    = 13,
	causeStorePageFault   = 15,
};


class VirtualCpuRiscV {
public:
	virtual ~VirtualCpuRiscV() {}
	virtual uint64 &Pc() = 0;
	virtual uint64 *Regs() = 0;
	virtual double *FRegs() = 0;
	virtual int Cause() = 0;
	virtual uint64 Tval() = 0;
	virtual void Run() = 0;
};


extern "C" {
_EXPORT VirtualCpuRiscV *instantiate_virtual_cpu_riscv();
};
