
shellcode= (
	"\x31\xc0"    /* # xorl   %eax,%eax */
	"\x31\xdb"    /* # xorl   %ebx,%ebx */
	"\xb0\xd5"    /* # movb   $0xd5,%a1	*/
	"\xcd\x80"    /* # int    $0x80		*/
);

#include <stdio.h>


void func(int* a1)
{
	printf(" :: a1's address is 0x%x \n", (unsigned int) &a1);
}

int main()
{
	int x = 3;
	func(&x);
	return 1;
}