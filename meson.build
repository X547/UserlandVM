project(
	'UserlandVM', 'cpp', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++2a']
 )

cpp = meson.get_compiler('cpp')

dep_rvvm = dependency('rvvm')

subdir('rvvm')
subdir('temu')


addon_temu = shared_library('temu',
	'VirtualCpuRiscVTemu.cpp',	
	cpp_args: temu_defines + ['-DMAX_XLEN=64'],
	link_with: [temu],
	gnu_symbol_visibility: 'hidden',
	install: true
)
dep_addon_temu = declare_dependency(
	link_with: addon_temu
)

addon_rvvm = shared_library('rvvm',
	'VirtualCpuRiscVRvvm.cpp',
	link_with: [rvvm],
	cpp_args: rvvm_defines + ['-DMAX_XLEN=64'],
	gnu_symbol_visibility: 'hidden',
	install: true
)
dep_addon_rvvm = declare_dependency(
	link_with: addon_rvvm
)

addon_rvvm2 = shared_library('rvvm2',
	'VirtualCpuRiscVRvvm2.cpp',
	'Referenceable.cpp',
	dependencies: [dep_rvvm],
	gnu_symbol_visibility: 'hidden',
	install: true
)
dep_addon_rvvm2 = declare_dependency(
	link_with: addon_rvvm2
)


executable(
	'UserlandVM',
	'Loader.cpp',
	'Syscalls.cpp',
	'VirtualCpuX86Native.cpp',
	'VirtualCpuX86NativeCode.S',
	'VirtualCpuX86Test.cpp',
	'Main.cpp',
	include_directories: [
		'/boot/system/develop/headers/private/shared',
		'/boot/system/develop/headers/private/system',
		'/boot/system/develop/headers/private/system/arch/x86_64',
	],
	gnu_symbol_visibility: 'hidden',
	install: true
)
