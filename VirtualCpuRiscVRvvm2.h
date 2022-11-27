#pragma once

#include "VirtualCpuRiscV.h"
#include <Referenceable.h>
#include <pthread.h>

extern "C" {
#include <rvvmlib.h>
}

class VirtualCpuRiscVRvvm2: public VirtualCpuRiscV {
private:
	static class Machine: public BReferenceable {
	private:
		rvvm_machine_t *fMachine{};
		pthread_mutex_t fMutex = PTHREAD_MUTEX_INITIALIZER;

	public:
		Machine();
		
		void FirstReferenceAcquired() final;
		void LastReferenceReleased() final;

		inline rvvm_machine_t *Get() {return fMachine;}
		inline pthread_mutex_t *GetMutex() {return &fMutex;}
	} sMachine;

	BReference<Machine> fMachine;
	rvvm_cpu_handle_t fCpu{};

	uint64 fPc{};
	uint64 fRegs[32]{};
	uint64 fFRegs[32]{};
	int fCause = -1;
	uint64 fTval{};

public:
	VirtualCpuRiscVRvvm2();
	virtual ~VirtualCpuRiscVRvvm2();
	uint64 &Pc() final;
	uint64 *Regs() final;
	double *FRegs() final;
	int Cause() final;
	uint64 Tval() final;
	void Run() final;
};
