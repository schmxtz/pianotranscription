#ifndef __PIANOTRANSCRIPTION_FFT_INCLUDE__
#define __PIANOTRANSCRIPTION_FFT_INCLUDE__

#include <stdint.h>
#include <math.h>
#include "../audio/audio.h"

#define M_PI 3.14159265358979323846

void fft(const wav_header *header, const wav_data_chunk *data_chunk, int32_t window_size);
void rfft(int32_t *samples, double *ouput_magnitudes, int32_t N);
void fft_complex(double complex *x, int32_t N);
double calc_mag(double complex x);
double calc_freq(int32_t N, int32_t k, int32_t sample_rate);

#endif
