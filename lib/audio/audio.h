#ifndef __PIANOTRANSCRIPTION_AUDIO_INCLUDE__
#define __PIANOTRANSCRIPTION_AUDIO_INCLUDE__

#include <stdio.h>

// WAV header spec information:
// https://docs.fileformat.com/audio/wav/

typedef struct wav_header {
    // RIFF Header
    char riff_header[4]; // "RIFF"
    int wav_size; // Size of the overall file - 8 bytes
    char wave_header[4]; // "WAVE"
    
    // Format Header
    char fmt_header[4]; // "fmt "
    int fmt_chunk_size; // Length of format data as listed above, should be 16 for PCM
    short audio_format; // Should be 1 for PCM. 3 for IEEE Float
    short num_channels; 
    int sample_rate; // Sample Rate = Number of Samples per second, or Hertz.
    int byte_rate; // Number of bytes per second. sample_rate * Bytes Per Sample * num_channels 
    short sample_alignment; // num_channels * Bytes Per Sample (1 - 8 bit mono, 2 - 8 bit stereo, 1 - 16 bit mono, 4 - 16 bit stereo)
    short bit_depth; // Number of bits per sample
    
    // Data
    char data_header[4]; // "data"
    int data_bytes; // Number of bytes in data. Number of samples * num_channels * sample byte size
    // uint8_t bytes[]; // Remainder of wave file is bytes
} wav_header;

read_audio_file(const char *filename, wav_header *header, uint8_t *data);

#endif