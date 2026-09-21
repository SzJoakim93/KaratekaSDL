/* tables.h – declarations for all global data that were previously
   defined in assembly files or scattered across C sources. */
#ifndef TABLES_H
#define TABLES_H

/* ------------------------------------------------------------------
   Constant lookup tables – values copied verbatim from the original
   assembly sources.  They are declared `const` because the program never
   writes to them.
   ------------------------------------------------------------------ */
extern const unsigned char D_E018[27];
extern const unsigned char D_E032[26];
extern const unsigned char D_E04C[10];
extern const unsigned char D_E056[10];
extern const unsigned char D_E060[10];
extern const unsigned char D_E06A[10];
extern const unsigned char D_E074[10];
extern const short        D_E07E[12];
extern const unsigned char D_E0A2[18];

/* Index and data tables used by the graphics engine */
extern unsigned char ks_index[0x100];
extern unsigned char  ks_data[0x41A0];
extern unsigned char km_index[0x100];
extern unsigned char  km_data[0x1D9C];

/* Graphics / asset buffers */
extern unsigned char D_A606[0x1040];   /* background tile data */
extern unsigned char *D_B9C0;         /* dynamically allocated render buffer */

/* Misc string literals – exact length matches the original comments */
extern const char D_DD8A[];   /* "make sure your karateka" */
extern const char D_DDB6[];   /* "press any key to continue" */
extern const char D_DDD0[];   /* "center joystick and press a key{" */
extern const char D_DE2B[];   /* " no joystick{  press any key{" */
extern const char D_DE49[];   /* " press q to quit| d for demo" */

/* Single‑byte globals (initially zero) */
/* Global keyboard buffer variables defined in the ASM input module */
extern unsigned char D_DE68; /* Last key pressed */
extern unsigned char D_DE69; /* Key pending flag */

extern unsigned char D_BB60;
extern unsigned char D_BB94[];
extern unsigned char D_BB65;
extern unsigned char D_DE70;
extern unsigned char D_DE72;
/* New globals */
extern int D_B9BA; // integer flag used by several modules
extern int D_B9BE; // buffer index used in assembly replacements
extern int D_BB92; // script buffer identifier
// Prototype for CodeChecksum placeholder
int CodeChecksum(void);

/* Misc integer globals */
extern int D_B9BE;
extern unsigned short D_D4E0;
extern unsigned short D_D4E2;
extern unsigned int D_D442;
extern unsigned char  D_D500;
extern unsigned char  D_D501;
extern unsigned int D_0168;

/* Function prototypes for assembly‑level stubs (implemented as no‑ops) */
void C_4055(void);
int C_3F83(void);
int C_3F9E(void);
int C_3FE4(void);
int C_4007(void);
int C_3F6C(void);
void Beep(void);
int Question(const char *msg);
void C_402A(void);

/* ------------------------------------------------------------------
   Helper: allocate the dynamic render buffer.
   Call this once (early in `main`) before any use of `D_B9C0`.
   ------------------------------------------------------------------ */
void init_dynamic_buffers(void);

#endif /* TABLES_H */
