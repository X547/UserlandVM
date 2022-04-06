#pragma once

#include "VirtualCpuX86.h"
#include <setjmp.h>
#include <AutoDeleterOS.h>


class VirtualCpuX86Native: public VirtualCpuX86 {
private:
	struct Context {
		uint32 ip;
		uint32 flags;
		uint32 fsbase;
		uint32 gsbase;
		uint32 regs[8];
		uint16 segs[6];
		uint8 fpsave[512];
	};
	Context fContext{};
	uint32 fIntVec{};
	uint32 fIntArg{};
	uint32 fRetProcAdr{};
	StopCause fCause = StopCause::init;

	AreaDeleter fCodeArea;
	void *fCodeBase{};

	jmp_buf fRetCtx{};

public:
	VirtualCpuX86Native();
	virtual ~VirtualCpuX86Native() {}

	uint32 &Ip() override;
	uint32 &Flags() override;
	uint32 *Regs() override;
	uint16 *Segs() override;
	uint8 *Fpsave() override;

	uint32 IntVec() override;
	uint32 IntArg() override;

	uint32 RetProcAdr() override;
	StopCause Cause() override;
	void Run() override;
};
