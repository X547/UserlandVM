project(
	'UserlandVM', 'cpp', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14']
 )

rv32 = static_library(
	'rv32',
	'rvvm/cpu/riscv_a.c',
	'rvvm/cpu/riscv_c.c',
	'rvvm/cpu/riscv_d.c',
	'rvvm/cpu/riscv_f.c',
	'rvvm/cpu/riscv_i.c',
	'rvvm/cpu/riscv_m.c',
	include_directories: ['rvvm'],
	c_args: ['-DHOST_64BIT', '-DUSE_RV64', '-DUSE_FPU'],
)

rv64 = static_library(
	'rv64',
	'rvvm/cpu/riscv_a.c',
	'rvvm/cpu/riscv_c.c',
	'rvvm/cpu/riscv_d.c',
	'rvvm/cpu/riscv_f.c',
	'rvvm/cpu/riscv_i.c',
	'rvvm/cpu/riscv_m.c',
	include_directories: ['rvvm'],
	c_args: ['-DHOST_64BIT', '-DUSE_RV64', '-DUSE_FPU', '-DRV64'],
)

rvvm = static_library(
	'rvvm',
	'rvvm/riscv_hart.c',
	'rvvm/riscv_cpu.c',
	'rvvm/riscv_priv.c',
	'rvvm/riscv_csr.c',
	'rvvm/riscv_mmu.c',
	'rvvm/rvtimer.c',
	'rvvm/utils.c',
	'rvvm/threading.c',
	c_args: ['-DHOST_64BIT', '-DUSE_RV64', '-DUSE_FPU'],
	link_with: [rv32, rv64],
)

executable(
	'UserlandVM',
	'Loader.cpp',
	'Syscalls.cpp',
	'Main.cpp',
	include_directories: [
		'/boot/system/develop/headers/private/shared',
		'/boot/system/develop/headers/private/system',
		'/boot/system/develop/headers/private/system/arch/x86_64',
	],
	cpp_args: ['-DHOST_64BIT', '-DUSE_RV64', '-DUSE_FPU'],
	link_with: rvvm,
	install : true
)
