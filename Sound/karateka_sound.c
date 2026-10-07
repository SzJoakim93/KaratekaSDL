/*
 * Karateka (IBM version) PC-speaker sound -> PCM renderer.
 * See karateka_sound.h.  Data tables live in karateka_sound_data.h.
 *
 * TIMING MODEL
 * ------------
 * All delays in the original are CPU busy-loops, so pitch and length depend
 * on CPU speed.  Cycle counts below are estimates for a 4.77 MHz 8088
 * (the original IBM PC), counted from the instructions in the asm.
 * To change the overall pitch/speed, change KS_CPU_HZ: a higher value means
 * the loops finish sooner, so everything gets higher in pitch and shorter.
 */
#include "karateka_sound.h"

#include <stdlib.h>
#include <string.h>

#include "karateka_sound_data.h"

#ifndef KS_CPU_HZ
#define KS_CPU_HZ          4772727.0 /* 4.77 MHz 8088 */
#endif
#ifndef KS_AMPLITUDE
#define KS_AMPLITUDE       9000      /* half the swing of the square wave   */
#endif
#ifndef KS_DC_BLOCK
#define KS_DC_BLOCK        1         /* 1 = ~10 Hz high-pass (speakers are
                                        AC-coupled; avoids DC and clicks)    */
#endif

#define KS_TOGGLE_CYCLES   94   /* C_3E68: call + push/in/xor/out/pop/ret   */
#define KS_DELAY_MUL       4    /* D_D59E[0], multiplier used by C_3E4C     */
#define KS_TICK_CYCLES     75   /* melody sequencer, one pass of C_3DE8     */
#define KS_FIRE_CYCLES     200  /* extra cost when a melody voice toggles   */
#define KS_YAAA_BIT_BASE   111  /* yaaa: per-bit cost excluding delay loop  */

/* yaaa pitch parameter = D_D59E[2,4,6,8] */
static const unsigned ks_yaaa_delay[4] = { 12, 9, 7, 4 };

/* ------------------------------------------------------------------ */
/* Speaker event recorder                                              */
/* ------------------------------------------------------------------ */
typedef struct {
    uint64_t *t;      /* timestamps (CPU cycles) at which the speaker flips */
    size_t    n, cap;
    uint64_t  now;    /* current time in CPU cycles */
    int       oom;
} ks_rec;

static void rec_flip(ks_rec *r)
{
    if (r->n == r->cap) {
        size_t nc = r->cap ? r->cap * 2 : 4096;
        uint64_t *p = (uint64_t *)realloc(r->t, nc * sizeof *p);
        if (!p) { r->oom = 1; return; }
        r->t = p; r->cap = nc;
    }
    r->t[r->n++] = r->now;
}

/* C_3E68: flip the speaker */
static void toggle(ks_rec *r)
{
    rec_flip(r);
    r->now += KS_TOGGLE_CYCLES;
}

/* C_3E4C: nested busy-loop, about M * CX^2 loop iterations */
static void delay(ks_rec *r, unsigned cx)
{
    r->now += (uint64_t)KS_DELAY_MUL * (20u * cx * cx + 35u * cx + 9u) + 100u;
}

/* ------------------------------------------------------------------ */
/* "hit" effects: sound_00 .. sound_06 (sound numbers 1..7)            */
/* ------------------------------------------------------------------ */
static void play_hit(ks_rec *r, int n)
{
    unsigned cx;
    int i;
    switch (n) {
    case 1:  /* sound_00 */
        for (cx = 2; cx < 0x28; cx += 1) { toggle(r); delay(r, cx); }
        break;
    case 2:  /* sound_01 */
        for (cx = 2; cx < 0x3A; cx += 2) { toggle(r); delay(r, cx); }
        break;
    case 3:  /* sound_02 */
        for (cx = 2; cx <= 0x24; cx += 2) { toggle(r); delay(r, cx); }
        break;
    case 4:  /* sound_03 */
        for (i = 0; i < 10; i++) { delay(r, 5);    toggle(r); }
        break;
    case 5:  /* sound_04 */
        for (i = 0; i < 16; i++) { delay(r, 10);   toggle(r); }
        break;
    case 6:  /* sound_05 */
        for (cx = 16; cx != 1; cx >>= 1) { toggle(r); delay(r, cx); }
        break;
    case 7:  /* sound_06 */
        for (i = 0; i < 3; i++)  { delay(r, 16);   toggle(r); }
        break;
    }
}

/* ------------------------------------------------------------------ */
/* "yaaa" 1-bit voice sample (C_3E71)                                  */
/* ------------------------------------------------------------------ */
static void play_yaaa(ks_rec *r, unsigned d)
{
    unsigned i;
    int bit;
    uint8_t state = 0;                 /* D_D59D: last level written */

    for (i = 0; i < 256; i++) {
        uint8_t al = ks_yaaa[i];
        for (bit = 0; bit < 8; bit++) {
            /* delay loop: D iterations of  DEC [mem] / JNZ  */
            r->now += 37u * d - 12u + KS_YAAA_BIT_BASE;
            if ((al ^ state) & 0x80) { /* bit differs from current level */
                state = al;
                toggle(r);
            }
            al <<= 1;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Melody sequencer (C_3D84)                                           */
/*                                                                     */
/* Two "voices" (A and B) share one speaker; each flip of either voice */
/* toggles the speaker, so the result is an XOR of two square waves.   */
/* With detune == 0 voice B drifts one tick per period against A (a    */
/* sweeping pulse width); with detune != 0 B is re-synced `detune`     */
/* ticks after A, giving a fixed pulse width.                          */
/* ------------------------------------------------------------------ */
static void play_melody(ks_rec *r, const uint8_t *s)
{
    unsigned H = s[0];
    const uint8_t *p = s + 2;
    uint8_t detune = 0;
    uint8_t cl, dl, a, b;

    for (;;) {
        uint8_t c = *p;
        unsigned dur, h, t;
        uint8_t n1, per;

        if (c == 0xFF) return;                       /* end */
        if (c == 0xFE) { detune = 0;    p += 1; continue; }
        if (c == 0xFD) { detune = p[1]; p += 2; continue; }

        dur = p[0]; n1 = p[1]; per = p[2];
        p += 3;
        cl = dl = per;                               /* CL, DL      */
        a = b = n1;                                  /* D_D591/592  */

        for (; dur; dur--)                           /* D_D599 */
        for (h = H; h; h--)                          /* D_D594 */
        for (t = 255; t; t--) {                      /* D_D595 */
            r->now += KS_TICK_CYCLES;

            /* voice A */
            if (--cl == 0) {
                if (--a == 0) {
                    rec_flip(r); r->now += KS_FIRE_CYCLES;
                    cl = per;
                    a  = n1;
                    if (detune) dl = detune;         /* re-sync voice B */
                }
            }
            /* voice B */
            if (--dl == 0) {
                if (--b != 0) {
                    dl--;                            /* wraps to 0xFF */
                } else {
                    rec_flip(r); r->now += KS_FIRE_CYCLES;
                    dl = (uint8_t)(per - 1);
                    b  = n1;
                }
            }
            if (r->oom) return;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public API                                                          */
/* ------------------------------------------------------------------ */
int ks_sound_allowed(int n, int mode)
{
    if (n < 1 || n > KS_SOUND_COUNT) return 0;
    if (mode == 0) return 1;
    if (mode == 1) return n < 9 || (n >= 0x16 && n < 0x19);
    return 0;
}

static const char *const ks_names[KS_SOUND_COUNT + 1] = {
    0,
    "hit (asm sound_00)",  "hit (asm sound_01)",  "hit (asm sound_02)",
    "hit (asm sound_03)",  "hit (asm sound_04)",  "hit (asm sound_05)",
    "hit (asm sound_06)",  "yaaa (asm sound_07)",
    "melody (asm sound_08)", "melody (asm sound_09)", "melody (asm sound_0a)",
    "melody (asm sound_0b)", "melody (asm sound_0c)", "melody (asm sound_0d)",
    "melody (asm sound_0e)", "melody (asm sound_0f)", "melody (asm sound_10)",
    "melody (asm sound_11)", "melody (asm sound_12)", "melody (asm sound_13)",
    "melody (asm sound_14)",
    "yaaa (asm sound_15)", "yaaa (asm sound_16)", "yaaa (asm sound_17)",
    "melody (asm sound_18)", "melody (asm sound_19)"
};

const char *ks_sound_name(int n)
{
    return (n >= 1 && n <= KS_SOUND_COUNT) ? ks_names[n] : 0;
}

int16_t *ks_render(int n, int sample_rate, size_t *num_samples)
{
    ks_rec r;
    int16_t *out = 0;
    size_t count, i, k = 0;
    double cps, level = 0.0;     /* cycles per output sample */
    double hp_x = -(double)KS_AMPLITUDE, hp_y = 0.0;   /* DC blocker state */
    const double hp_r = 0.9986;                        /* ~10 Hz @ 44.1 kHz */
    int cur = 0;

    if (n < 1 || n > KS_SOUND_COUNT || sample_rate <= 0) return 0;
    memset(&r, 0, sizeof r);

    if (n <= 7)                         play_hit(&r, n);
    else if (n == 8)                    play_yaaa(&r, ks_yaaa_delay[0]);
    else if (n >= 22 && n <= 24)        play_yaaa(&r, ks_yaaa_delay[n - 21]);
    else                                play_melody(&r, ks_melodies[n].data);

    if (r.oom) { free(r.t); return 0; }

    cps   = KS_CPU_HZ / (double)sample_rate;
    count = (size_t)((double)r.now / cps) + 1;
    out   = (int16_t *)malloc(count * sizeof *out);
    if (!out) { free(r.t); return 0; }

    /* Area-sample the 0/1 speaker waveform: each output sample is the
     * average level over its time window (cheap anti-aliasing), then the
     * DC offset is removed. */
    for (i = 0; i < count; i++) {
        double a = (double)i * cps, b = a + cps, pos = a, acc = 0.0;
        while (k < r.n && (double)r.t[k] < b) {
            double te = (double)r.t[k];
            if (te < pos) te = pos;
            if (cur) acc += te - pos;
            pos = te;
            cur ^= 1;
            k++;
        }
        if (cur) acc += b - pos;
        level = acc / cps;
        {
            double x = (level - 0.5) * 2.0 * KS_AMPLITUDE;
#if KS_DC_BLOCK
            double y = x - hp_x + hp_r * hp_y;
            hp_x = x; hp_y = y;
            x = y;
#endif
            if (x >  32767.0) x =  32767.0;
            if (x < -32768.0) x = -32768.0;
            out[i] = (int16_t)x;
        }
    }

    /* short fade-out (~6 ms, less for tiny sounds) so playback ends silently */
    {
        size_t f = count < 1024 ? count / 4 : 256;
        for (i = 0; i < f; i++)
            out[count - f + i] =
                (int16_t)(out[count - f + i] * (1.0 - (double)(i + 1) / (double)f));
    }

    free(r.t);
    if (num_samples) *num_samples = count;
    return out;
}
