#pragma once

#include "VirtualCpu.h"

struct PhysMemoryMap;
struct PhysMemoryRange;
struct RISCVCPUState;


class VirtualCpuTemu: public VirtualCpu {
private:
	PhysMemoryMap *fMemMap;
	PhysMemoryRange *fRam;
	RISCVCPUState *fCpu;

public:
	VirtualCpuTemu();
	virtual ~VirtualCpuTemu();
	uint64 &Pc() final;
	uint64 *Regs() final;
	double *FRegs() final;
	int Cause() final;
	uint64 Tval() final;
	void Run() final;
};
