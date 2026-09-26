/* tables.c – definitions of all globals declared in tables.h */

#include "tables.h"
#include <stdlib.h>

/* ------------------------------------------------------------------
   Constant tables – exact byte sequences taken from the original
   assembly files.
   ------------------------------------------------------------------ */
const unsigned char D_E018[27] = {
    0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x0C,
    0x10,0x10,0x10,0x0C,0x10,0x10,0x0C,0x10,
    0x10,0x10,0x10,0x10,0x10,0x0C,0x10,0x0C,
    0x10,0x0C,0x10
};

const unsigned char D_E032[26] = {
    0,1,2,3,4,5,6,7,6,8,9,7,9,9,7,9,9,9,9,6,0,8,0,6,0,6
};

const unsigned char D_E04C[10] = { 3,3,0,0,0,3,3,0,3,0 };
const unsigned char D_E056[10] = { 3,3,3,0,0,3,3,3,3,3 };
const unsigned char D_E060[10] = { 2,2,0,0,0,2,2,2,0,2 };
const unsigned char D_E06A[10] = { 2,0,2,0,0,2,2,2,0,2 };
const unsigned char D_E074[10] = { 0,0,0,0,0,0,0,0,2,0 };

const short D_E07E[12] = {
    0x28, 0x2A, 0x26, 0x22, 0x22, 0x1C,
    -4, -4, -4, -0x14, -0x14, -0x14
};

const unsigned char D_E0A2[18] = {
    0x82,0x8C,0x96,0x8D,0x92,0x9E,
    0x82,0x8C,0x96,0x8B,0x90,0x9E,
    0x82,0x8C,0x96,0x84,0x90,0x9E
};

/* ------------------------------------------------------------------
   Index / data tables – zero‑initialised; they will be filled at
   runtime by the asset‑loading code.
   ------------------------------------------------------------------ */
unsigned char ks_index[0x200] = {0};
unsigned char  ks_data[0x41A0] = {0};
unsigned char km_index[0x200] = {0};
unsigned char  km_data[0x1D9C] = {0};

/* ------------------------------------------------------------------
   Graphics / asset buffers
   ------------------------------------------------------------------ */
unsigned char D_A606[0x10F4] = {0};

/* Dynamic render buffer – allocated on program start */
unsigned char *D_B9C0 = NULL;

/* ------------------------------------------------------------------
   String literals – length matches the comments from the original
   assembly (null‑terminated automatically by the compiler).
   ------------------------------------------------------------------ */
const char D_DD8A[] = "make sure your karateka";
const char D_DDB6[] = "press any key to continue";
const char D_DDD0[] = "center joystick and press a key{";
const char D_DE2B[] = " no joystick{  press any key{";
const char D_DE49[] = " press q to quit| d for demo";
/* Additional empty string literals referenced in C_19F5.c */
const char D_E0CB[] = "";
const char D_E0CC[] = "";
const char D_E0CD[] = "";
const char D_E0CE[] = "";

/* ------------------------------------------------------------------
   Single‑byte globals – default to zero.
   ------------------------------------------------------------------ */
/* Global keyboard buffer variables defined in the ASM input module */
unsigned char D_DE68 = 0; /* Last key pressed */
unsigned char D_DE69 = 0; /* Key pending flag */

unsigned char D_BB60 = 0;
unsigned char D_BB94[0x400] = {
   0x04, 0x02, 0x64, 0x00, 0x64,
   0x04, 0x01, 0x96, 0x00, 0x64,
   0x08,
   0x10, 0x5A,
   0x06, 0x01, 0x02, 0x96, 0x00, 0x14,
   0x08,
   0xFF
};
unsigned char D_BB65 = 0;
unsigned char D_DE70 = 0;
unsigned char D_DE72 = 0;

/* ------------------------------------------------------------------
   Misc integer globals
   ------------------------------------------------------------------ */
int D_B9BA = 0; // integer flag used by several modules
int D_B9BE = 0; // buffer index used in assembly replacements
int D_BB92 = 0; // script buffer identifier
unsigned short D_D4E0 = 0;
unsigned short D_D4E2 = 0;
unsigned int D_D442 = 0;
unsigned char  D_D500 = 0;
unsigned char  D_D501 = 0;
unsigned int D_0168 = 0;

/* ------------------------------------------------------------------
   Stub functions – the original DOS/asm versions are omitted.
   They are defined as empty bodies in `stubs.c`.  The prototypes are
   kept here for the linker.
   ------------------------------------------------------------------ */
void C_4055(void);
int C_3F83(void);
int C_3F9E(void);
int C_3FE4(void);
int C_4007(void);
int C_3F6C(void);

/* ------------------------------------------------------------------
   Runtime initialiser – allocate the render buffer.
   ------------------------------------------------------------------ */
void init_dynamic_buffers(void)
{
    if (!D_B9C0) {
        /* 1 KB is the size used by the original code. */
        D_B9C0 = (unsigned char *)malloc(1024);
        if (!D_B9C0) {
            /* In a real program we would abort, but for this build we
               simply zero the pointer to avoid crashes. */
            D_B9C0 = NULL;
        } else {
            /* Zero‑initialise – the original assembly left it undefined,
               but zero is a safe default. */
            for (size_t i = 0; i < 1024; ++i) D_B9C0[i] = 0;
        }
    }
}
