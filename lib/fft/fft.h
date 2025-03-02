#ifndef __PIANOTRANSCRIPTION_FFT_INCLUDE__
#define __PIANOTRANSCRIPTION_FFT_INCLUDE__

#include <stdint.h>
#include <math.h>
#include "../audio/audio.h"

#define M_PI 3.14159265358979323846
#define M_E 2.71828182845904523536

typedef struct {
    double real;
    double imag;
} complex;

void fft(const wav_header *header, const wav_data_chunk *data_chunk, int32_t window_size);
void bit_reverse(complex *data, int32_t n);
void rfft(complex *data, int32_t n);
void extract_rffts_to_buffer(complex *fft_data, double *output, int32_t n);


#endif
