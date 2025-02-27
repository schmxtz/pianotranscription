#include "fft.h"


void fft(const wav_header *header, const wav_data_chunk *data_chunk, int32_t **fft_data) {
    (*fft_data) = (int32_t *)malloc(data_chunk->size * sizeof(int32_t *));
    (*fft_data)[0] = 1; 
    int32_t num_samples = data_chunk->size / header->sample_alignment;
    printf("Start");
    for (int i = 0; i < header->num_channels; i++) {
        for (int j = 0; j < num_samples; j++) {
            if (i > 5) printf("%d\n", (data_chunk->channel_data[i * num_samples + j]));
            continue;
        }
    }
    printf("End");
}