#ifndef __PIANOTRANSCRIPTION_FFT_INCLUDE__
#define __PIANOTRANSCRIPTION_FFT_INCLUDE__

#include <stdint.h>
#include "../audio/audio.h"

#define M_PI 3.14159265358979323846

void fft(const wav_header *header, const wav_data_chunk *data_chunk, int32_t **fft_data, int32_t window_size);

#endif
