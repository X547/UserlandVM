temu_defines = ['-DCONFIG_RISCV_MAX_XLEN=64']

cpu32 = static_library(
	'cpu32',
	'riscv_cpu.c',
	c_args: temu_defines + ['-DMAX_XLEN=32'],
)

cpu64 = static_library(
	'cpu64',
	'riscv_cpu.c',
	c_args: temu_defines + ['-DMAX_XLEN=64'],
)

temu = static_library(
	'temu',
	'cutils.c',
	'iomem.c',
	'softfp.c',
	c_args: temu_defines,
	link_with: [cpu32, cpu64],
)
