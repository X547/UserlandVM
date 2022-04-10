#pragma once

#include "VirtualCpuRiscV.h"

struct PhysMemoryMap;
struct PhysMemoryRange;
struct RISCVCPUState;


class VirtualCpuRiscVTemu: public VirtualCpuRiscV {
private:
	PhysMemoryMap *fMemMap;
	PhysMemoryRange *fRam;
	RISCVCPUState *fCpu;

public:
	VirtualCpuRiscVTemu();
	virtual ~VirtualCpuRiscVTemu();
	uint64 &Pc() final;
	uint64 *Regs() final;
	double *FRegs() final;
	int Cause() final;
	uint64 Tval() final;
	void Run() final;
};
