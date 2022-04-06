#pragma once

#include <SupportDefs.h>


class VirtualCpuX86 {
public:
	enum class StopCause {
		init,
		cycleLimit,
		interrupt,
		retProc
	};

	virtual ~VirtualCpuX86() {}

	virtual uint32 &Ip() = 0;
	virtual uint32 &Flags() = 0;
	virtual uint32 *Regs() = 0;
	virtual uint16 *Segs() = 0;
	virtual uint8 *Fpsave() = 0;
	
	virtual uint32 IntVec() = 0;
	virtual uint32 IntArg() = 0;

	virtual uint32 RetProcAdr() = 0;
	virtual StopCause Cause() = 0;
	virtual void Run() = 0;
};
