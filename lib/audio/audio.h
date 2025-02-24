#ifndef __PIANOTRANSCRIPTION_AUDIO_INCLUDE__
#define __PIANOTRANSCRIPTION_AUDIO_INCLUDE__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// WAV header spec information:
// https://docs.fileformat.com/audio/wav/

typedef struct wav_header {
    // RIFF Header
    char riff_header[4]; // "RIFF"
    int32_t wav_size; // Size of the overall file - 8 bytes
    char wave_header[4]; // "WAVE"
    
    // Format Header
    char fmt_header[4]; // "fmt "
    int32_t fmt_chunk_size; // Length of format data as listed above, should be 16 for PCM
    int16_t audio_format; // Should be 1 for PCM. 3 for IEEE Float
    int16_t num_channels; 
    int32_t sample_rate; // Sample Rate = Number of Samples per second, or Hertz.
    int32_t byte_rate; // Number of bytes per second. sample_rate * Bytes Per Sample * num_channels 
    int16_t sample_alignment; // num_channels * Bytes Per Sample (1 - 8 bit mono, 2 - 8 bit stereo, 1 - 16 bit mono, 4 - 16 bit stereo)
    int16_t bit_depth; // Number of bits per sample
} wav_header;

typedef struct wav_data_chunk {
    int32_t size;
    uint8_t **channel_data;
} wav_data_chunk;

void wav_read_audio_file(const char *filename, wav_header *wav_header, wav_data_chunk *wav_data_chunk);
void wav_read_audio_header(FILE *file, wav_header *wav_header);
void wav_read_data_chunk(FILE *file, wav_header *wav_header, wav_data_chunk *wav_data_chunk);
void wav_print_header(wav_header *wav_header);

#endif