#include "VirtualCpuRiscVTemu.h"

extern "C" {
#include "temu/riscv_cpu.h"
#include "temu/riscv_cpu_priv.h"
#include "temu/iomem.h"
}

#include <pthread.h>
#include <new>

pthread_mutex_t sCpuMutex = PTHREAD_MUTEX_INITIALIZER;


VirtualCpuRiscVTemu::VirtualCpuRiscVTemu()
{
	fMemMap = phys_mem_map_init();
	fRam = register_ram_entry(fMemMap, 0x1000, (uint64_t)(-0x1000), 0);
	fRam->phys_mem = (uint8_t*)0x1000;
	fCpu = riscv_cpu_init(fMemMap, 64);
	fCpu->fs = 1;
}

VirtualCpuRiscVTemu::~VirtualCpuRiscVTemu()
{
	riscv_cpu_end(fCpu); fCpu = NULL;
	fRam->phys_mem = NULL;
	phys_mem_map_end(fMemMap); fMemMap = NULL;
}

uint64 &VirtualCpuRiscVTemu::Pc() {return fCpu->pc;}
uint64 *VirtualCpuRiscVTemu::Regs() {return fCpu->reg;}
double *VirtualCpuRiscVTemu::FRegs() {return (double*)fCpu->fp_reg;}
int VirtualCpuRiscVTemu::Cause() {return fCpu->pending_exception;}
uint64 VirtualCpuRiscVTemu::Tval() {return fCpu->pending_tval;}

void VirtualCpuRiscVTemu::Run()
{
	// TinyEMU is not thread-safe for now (do not implement atomic instructions) so use mutex
	pthread_mutex_lock(&sCpuMutex);
	fCpu->power_down_flag = FALSE;
	fCpu->pending_exception = -1;
	riscv_cpu_interp(fCpu, 10000);
	switch (fCpu->pending_exception) {
		case CAUSE_USER_ECALL:
		case CAUSE_SUPERVISOR_ECALL:
		case CAUSE_HYPERVISOR_ECALL:
		case CAUSE_MACHINE_ECALL:
			fCpu->pc += 4;
		default:
			;
	}
	pthread_mutex_unlock(&sCpuMutex);
}


VirtualCpuRiscV *instantiate_virtual_cpu_riscv()
{
	return new(std::nothrow) VirtualCpuRiscVTemu();
}
