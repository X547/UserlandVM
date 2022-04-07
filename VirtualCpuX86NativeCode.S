#define FUNCTION(name)			.global name; .type name, @function; name
#define FUNCTION_END(name)		1: .size name, 1b - name


#define USER_DATA_SELECTOR 0x23

#define State_ip           0x8
#define State_retIp       0x14
#define State_ctx         0x1c
#define State_retCtx     0x258
#define State_longjmpAdr 0x2a0

#define Context_ip           0
#define Context_regs      0x10


FUNCTION(VirtualCpuX86NativeCodeBegin):


FUNCTION(VirtualCpuX86NativeLoadContext):
.code32
	mov $USER_DATA_SELECTOR, %dx
	mov %dx, %ds
	mov %dx, %es

	xor %edx, %edx
	mov	%gs:(%edx), %eax

	# TODO: other registers
	mov		State_ctx + Context_regs + 4*3(%eax), %ebx
	mov		State_ctx + Context_regs + 4*4(%eax), %esp
	mov		State_ctx + Context_regs + 4*5(%eax), %ebp
	mov		State_ctx + Context_regs + 4*6(%eax), %esi
	mov		State_ctx + Context_regs + 4*7(%eax), %edi
	mov		State_ctx + Context_ip(%eax), %edx
	push	%edx

	ret
FUNCTION_END(VirtualCpuX86NativeLoadContext)

FUNCTION(VirtualCpuX86NativeSaveContext):
.code32
	xor %edx, %edx
	mov	%gs:(%edx), %eax

	# TODO: other registers
	mov		%ebx, State_ctx + Context_regs + 4*3(%eax)
	mov		%esp, State_ctx + Context_regs + 4*4(%eax)
	mov		%ebp, State_ctx + Context_regs + 4*5(%eax)
	mov		%esi, State_ctx + Context_regs + 4*6(%eax)
	mov		%edi, State_ctx + Context_regs + 4*7(%eax)
	mov   0(%esp), %edx # return address
	mov		%edx, State_ctx + Context_ip(%eax)

	lea State_retIp(%eax), %edx
	ljmp *(%edx)
FUNCTION_END(VirtualCpuX86NativeSaveContext)

FUNCTION(VirtualCpuX86NativeReturn):
.code64
	mov	%gs:0, %rax

	lea State_retCtx(%rax), %rdi
	mov $1, %rsi
	mov State_longjmpAdr(%rax), %rdx
	call *%rdx
FUNCTION_END(VirtualCpuX86NativeReturn)


FUNCTION(VirtualCpuX86NativeCodeEnd):


FUNCTION(VirtualCpuX86NativeSwitch32):
	mov	%gs:0, %rax
	mov State_ctx + Context_regs + 4*4(%rax), %esp
	lea State_ip(%rax), %rdx
	rex64 ljmp *(%rdx)
FUNCTION_END(VirtualCpuX86NativeSwitch32)
