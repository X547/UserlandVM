#define FUNCTION(name)			.global name; .type name, @function; name
#define FUNCTION_END(name)		1: .size name, 1b - name


FUNCTION(VirtualCpuX86NativeEnter32):
	rex64 ljmp *(%rdi)
FUNCTION_END(VirtualCpuX86NativeEnter32)

FUNCTION(VirtualCpuX86NativeCodeBegin):

FUNCTION(VirtualCpuX86NativeReturn):
.code32
	ljmp *(%eax)
FUNCTION_END(VirtualCpuX86NativeReturn)

FUNCTION(VirtualCpuX86NativeCodeEnd):
