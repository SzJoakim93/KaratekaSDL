/* KARATEKA - C replacements for original assembly routines. */
#include "karateka.h"
#include "assembly_replacements.h"

/* Pseudo-random number generator mapping to original Prime LCG */
int k_rand(int range)
{
	int r = C_414A();
	r = (r * 2) & 0xFFFF;
	return (int)(((unsigned long)(range + 1) * r) >> 16);
}

/* Stub/Null exit routines */
void C_4426(void) {}
