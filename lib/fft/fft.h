#ifndef __PIANOTRANSCRIPTION_FFT_INCLUDE__
#define __PIANOTRANSCRIPTION_FFT_INCLUDE__

#include <stdint.h>
#include "../audio/audio.h"

void fft(const wav_header *header, const wav_data_chunk *data_chunk, int32_t **fft_data);

#endif
