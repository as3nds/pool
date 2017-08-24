#include <stdio.h>
#include <openssl/sha.h>

#define RET_ASM 0xc9

void mf1() {printf("mf1\n");}
void mf2() {printf("mf2\n");}
void mf3() {printf("mf3\n");}

int
main (int argc, char* argv[])
{
	char *ptr;
	int md;

	ptr = (char *)mf1;

	printf("mf1 - @%p\n", mf1);
	printf("@%p - d:%x\n", mf1, *ptr);
	printf("mf2 - @%p\n", mf2);
	printf("mf3 - @%p\n", mf3);

	md = checksum (mf1);
	printf("checksum: 0x%x\n", md);

	return 0;
}	

int checksum (void* (f)(void)) { int i;
	int ret = 0;
	unsigned char *ptr;
	unsigned char md[SHA_DIGEST_LENGTH];
	SHA_CTX ctx;
	
	ptr = (unsigned char *)f;

	printf("SHA_DIGEST_LENGTH: %d\n", SHA_DIGEST_LENGTH);

	SHA1_Init(&ctx);
	while (*ptr != RET_ASM)
		SHA1_Update(&ctx, ptr++, 1);
	SHA1_Final(md, &ctx);

	for (i=0; i < 4; i++) {
		ret += ((unsigned int) md[i] << (8*i));
	}

	return ret;
}
