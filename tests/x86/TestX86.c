#include <stdint.h>

#include <TLS.h>

typedef int (*RetProc)(void *retProgArg);

RetProc gRetProc;
void *gRetProcArg;

int __attribute__((naked)) Syscall(int op, ...)
{
	asm("mov gRetProcArg, %eax");
	asm("push 0(%eax)");
	asm("call *gRetProc");
	asm("ret");
}

void WriteString(const char* val)
{
	Syscall(2, val);
}

void WriteChar(char ch)
{
	char str[2];
	str[0] = ch;
	str[1] = 0;
	WriteString(str);
}

void WriteInt(int32_t val)
{
	if (val == INT32_MIN) WriteString("-2147483648");
	else if (val < 0) {WriteString("-"); WriteInt(-val);}
	else {
		if (val > 9) WriteInt(val/10);
		WriteChar(val%10 + '0');
	}
}

void Recursion(int n)
{
	WriteString("+Recursion("); WriteInt(n); WriteString(")\n");
	if (n > 0) {
		Recursion(n - 1);
	}
	WriteString("-Recursion("); WriteInt(n); WriteString(")\n");
}


void _start()
{
	WriteString("This is a test\n");
	WriteInt(1234); WriteString("\n");

	// currently crashes
	//WriteString("TLS: "); WriteInt((uint32_t)tls_address(0)); WriteString("\n");

	Recursion(10);

	Syscall(1);
}
