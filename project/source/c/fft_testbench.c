#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern int fft(); // TIE implementation
#include "fft_ref.h" // reference C implementation

#define M_MIN 5
#define M_MAX 10
#define N_ITER 10
#define VERBOSE 0

static int16_t int16_rand()
{
  return rand() % ((1 << 16) - 1) - (1 << 15);
}

static int compare_results(int16_t *fr, int16_t *fi,
                           int16_t *fr_ref, int16_t *fi_ref, int n)
{
    int error = 0;

    int j;
    for (j = 0; j < n; ++j) {
        if (fr[j] != fr_ref[j] || fi[j] != fi_ref[j]) {
            printf("ERROR: fr[%d] = 0x%08x; fi[%d] = 0x%08x;"
                   " vs. fr_ref[%d] = 0x%08x; fi_ref[%d] = 0x%08x;\n",
                   j, fr[j], j, fi[j], j, fr_ref[j], j, fi_ref[j]);
            error = 1;
        } else if (VERBOSE) {
            printf("fr[%d] = 0x%08x; fi[%d] = 0x%08x;\n",
                   j, fr[j], j, fi[j]);
        }
    }
    return error;
}

/* Repeatedly perform FFT and inverse FFT with randomly initialized input
 * vectors, for a range of values of N. Compare the results with the
 * reference implementation. Note: if DIF is enabled in for the asm/tie FFT
 * function, a definition for the preprocessor symbol "DIF" should be passed to
 * gcc when compiling this testbench.
 */
int main()
{
    int16_t *fr, *fi, *fr_ref, *fi_ref;

    fr = (int16_t *) 0x5FFE0000;
    fi = (int16_t *) 0x5FFE8000;

    srand(time(NULL));

    int m, iter;
    for (m = M_MIN; m <= M_MAX; ++m) {
        printf("# M = %d\n", m);

        int n = 1 << m;

        fr_ref = (int16_t *) (0x5FFE0000 + 2 * n);
        fi_ref = (int16_t *) (0x5FFE8000 + 2 * n);

        for (iter = 1; iter <= N_ITER; ++iter) {
            printf("## ITERATION %d/%d\n", iter, N_ITER);

            // Fill FFT input vectors with arbitrary test values.
            puts("### Initializing inputs...");

            int i;
            for (i = 0; i < n; ++i) {
                fr[i] = 100 * i; //int16_rand();
                fi[i] = 100 + i; //int16_rand();
                fr_ref[i] = fr[i];
                fi_ref[i] = fi[i];

                if (VERBOSE) {
                    printf("fr[%d] = 0x%08x; fi[%d] = 0x%08x;\n",
                           i, fr[i], i, fi[i]);
                }
            }

            // Perform FFT.
            int scale = fft(fr, fi, m, 0);
#ifdef DIF
            int scale_ref = fft_ref_dif(fr_ref, fi_ref, m, 0);
#else
            int scale_ref =fft_ref_dit(fr_ref, fi_ref, m, 0);
#endif
            if (scale != scale_ref) {
                printf("ERROR: scale conflict: scale = %d, scale_ref = %d\n",
                       scale, scale_ref);
                goto failure;
            }

            // Compare results with reference implementation.
            puts("### Evaluating FFT implementation...");
            if (compare_results(fr, fi, fr_ref, fi_ref, n) != 0)
                goto failure;

            // Perform inverse FFT (should yield initial values back).
            scale = fft(fr, fi, m, 1);
#ifdef DIF
            scale_ref = fft_ref_dif(fr_ref, fi_ref, m, 1);
#else
            scale_ref = fft_ref_dit(fr_ref, fi_ref, m, 1);
#endif
            if (scale != scale_ref) {
                printf("ERROR: scale conflict: scale = %d, scale_ref = %d\n",
                       scale, scale_ref);
                goto failure;
            }

            // Compare results with reference implementation.
            puts("### Evaluating inverse FFT implementation...");
            if (compare_results(fr, fi, fr_ref, fi_ref, n) != 0)
                goto failure;
        }
    }
    printf("No errors encountered (%d iterations each for m from %d to %d)\n",
           N_ITER, M_MIN, M_MAX);

    return 0;

failure:
    printf("Error(s) encountered for m = %d (iteration %d)\n", m, iter);
    return 1;
}
