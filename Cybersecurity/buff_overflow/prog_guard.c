#include <stdio.h>
#include <stdlib.h>

shellcode= (
	"\x31\xc0"    # xorl   %eax,%eax
	"\x31\xdb"    # xorl   %ebx,%ebx
	"\xb0\xd5"    # movb   $0xd5,%a1	
	"\xcd\x80"    # int   $0x80);

void func(char* str)
{
	int guard;
	guard = secret;

	char buffer[12];
	strcpy (buffer, str);

	if ( guard == secret )
	{
		return;
	} else {
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	func(argv[0]);
	return 1;
}