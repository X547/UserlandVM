project(
	'UserlandVM', 'cpp', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14']
 )

subdir('rvvm')

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
	cpp_args: rvvm_defines,
	link_with: rvvm,
	install : true
)
