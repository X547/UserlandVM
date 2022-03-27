rvvm_defines = ['-DHOST_64BIT', '-DUSE_RV64', '-DUSE_FPU']

rvvm_cpu_sources = [
	'cpu/riscv_a.c',
	'cpu/riscv_c.c',
	'cpu/riscv_d.c',
	'cpu/riscv_f.c',
	'cpu/riscv_i.c',
	'cpu/riscv_m.c',
]

rv32 = static_library(
	'rv32',
	rvvm_cpu_sources,
	c_args: rvvm_defines,
)

rv64 = static_library(
	'rv64',
	rvvm_cpu_sources,
	c_args: rvvm_defines + ['-DRV64'],
)

rvvm = static_library(
	'rvvm',
	'riscv_hart.c',
	'riscv_cpu.c',
	'riscv_priv.c',
	'riscv_csr.c',
	'riscv_mmu.c',
	'rvtimer.c',
	'utils.c',
	'threading.c',
	c_args: rvvm_defines,
	link_with: [rv32, rv64],
)
