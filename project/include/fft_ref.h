#ifndef FFT_REF_H
#define FFT_REF_H

#include <inttypes.h>

int fft_ref_dit(int16_t *fr, int16_t *fi, int m, int inverse);
int fft_ref_dif(int16_t *fr, int16_t *fi, int m, int inverse);

#endif /* FFT_REF_H */
