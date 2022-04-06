project(
	'UserlandVM', 'cpp', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++2a']
 )

subdir('rvvm')
subdir('temu')

executable(
	'UserlandVM',
	'Loader.cpp',
	'Syscalls.cpp',
	'VirtualCpuRvvm.cpp',
	'VirtualCpuTemu.cpp',
	'VirtualCpuX86Native.cpp',
	'VirtualCpuX86NativeCode.S',
	'Main.cpp',
	include_directories: [
		'/boot/system/develop/headers/private/shared',
		'/boot/system/develop/headers/private/system',
		'/boot/system/develop/headers/private/system/arch/x86_64',
	],
	cpp_args: rvvm_defines + temu_defines + ['-DMAX_XLEN=64'],
	link_with: [rvvm, temu],
	install : true
)
