#include "VirtualCpuRiscVRvvm2.h"

#include <new>


VirtualCpuRiscVRvvm2::Machine VirtualCpuRiscVRvvm2::sMachine;


VirtualCpuRiscVRvvm2::Machine::Machine()
{
	fReferenceCount = 0;
}

void VirtualCpuRiscVRvvm2::Machine::FirstReferenceAcquired()
{
	fMachine = rvvm_create_userland(true);
}

void VirtualCpuRiscVRvvm2::Machine::LastReferenceReleased()
{
	rvvm_free_machine(fMachine);
	fMachine = NULL;
}


VirtualCpuRiscVRvvm2::VirtualCpuRiscVRvvm2()
{
	fMachine.SetTo(&sMachine);
	pthread_mutex_lock(fMachine->GetMutex());
	fCpu = rvvm_create_user_thread(fMachine->Get());
	pthread_mutex_unlock(fMachine->GetMutex());
}

VirtualCpuRiscVRvvm2::~VirtualCpuRiscVRvvm2()
{
	pthread_mutex_lock(fMachine->GetMutex());
	rvvm_free_user_thread(fCpu);
	pthread_mutex_unlock(fMachine->GetMutex());
}

uint64 &VirtualCpuRiscVRvvm2::Pc() {return fPc;}
uint64 *VirtualCpuRiscVRvvm2::Regs() {return fRegs;}
double *VirtualCpuRiscVRvvm2::FRegs() {return (double*)fFRegs;}
int VirtualCpuRiscVRvvm2::Cause() {return fCause;}
uint64 VirtualCpuRiscVRvvm2::Tval() {return fTval;}

void VirtualCpuRiscVRvvm2::Run()
{
	rvvm_write_cpu_reg(fCpu, RVVM_REGID_PC, fPc);
	for (uint32 i = 0; i < 32; i++)
		rvvm_write_cpu_reg(fCpu, RVVM_REGID_X0 + i, fRegs[i]);
	for (uint32 i = 0; i < 32; i++)
		rvvm_write_cpu_reg(fCpu, RVVM_REGID_F0 + i, fFRegs[i]);

	fCause = rvvm_run_user_thread(fCpu);

	fPc = rvvm_read_cpu_reg(fCpu, RVVM_REGID_PC);
	for (uint32 i = 0; i < 32; i++)
		fRegs[i] = rvvm_read_cpu_reg(fCpu, RVVM_REGID_X0 + i);
	for (uint32 i = 0; i < 32; i++)
		fFRegs[i] = rvvm_read_cpu_reg(fCpu, RVVM_REGID_F0 + i);
	fTval = rvvm_read_cpu_reg(fCpu, RVVM_REGID_TVAL);
}


VirtualCpuRiscV *instantiate_virtual_cpu_riscv()
{
	return new(std::nothrow) VirtualCpuRiscVRvvm2();
}
