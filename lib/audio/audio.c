#include "audio.h"


void read_audio_file(const char *filename, wav_header *header, uint8_t *data) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }
    // Read header
    fread(header, sizeof(wav_header), 1, file);

    // Read following id 
    // Read data
    // data = (uint8_t *)malloc(header->data_bytes);
    // if (data == NULL) {
    //     fprintf(stderr, "Error: Could not allocate memory for data\n");
    //     exit(1);
    // }
    // fread(data, header->data_bytes, 1, file);

    fclose(file);
}

void debug_print_header(wav_header *header) {
    printf("header->riff_header: %.4s\n", header->riff_header);
    printf("header->wav_size: %d\n", header->wav_size);
    printf("header->wave_header: %.4s\n", header->wave_header);
    printf("header->fmt_header: %.4s\n", header->fmt_header);
    printf("header->fmt_chunk_size: %d\n", header->fmt_chunk_size);
    printf("header->audio_format: %d\n", header->audio_format);
    printf("header->num_channels: %d\n", header->num_channels);
    printf("header->sample_rate: %d\n", header->sample_rate);
    printf("header->byte_rate: %d\n", header->byte_rate);
    printf("header->sample_alignment: %d\n", header->sample_alignment);
    printf("header->bit_depth: %d\n", header->bit_depth);
}