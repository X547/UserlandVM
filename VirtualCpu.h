#pragma once

#include <SupportDefs.h>


class VirtualCpu {
public:
	virtual ~VirtualCpu() {}
	virtual uint64 &Pc() = 0;
	virtual uint64 *Regs() = 0;
	virtual double *FRegs() = 0;
	virtual int Cause() = 0;
	virtual uint64 Tval() = 0;
	virtual void Run() = 0;
};
