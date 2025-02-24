#include "audio.h"


void read_audio_file(const char *filename, wav_header *header, data_chunk *data_chunk) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }

    // Read header
    fread(header, sizeof(wav_header), 1, file);
    debug_print_header(header);

    // For now don't care about other chunk ids except data
    char chunk_id[4];
    uint8_t data_found = 0;
    while (fread(chunk_id, 4, 1, file) == 1) {
        if (strncmp(chunk_id, "data", 4) == 0) {
            data_found = 1;
            if (fread(&data_chunk->size, 4, 1, file) != 1) {
                fprintf(stderr, "Error: Could not read data size\n");
                exit(1);
            }

            // Read data
            data_chunk->data = (uint8_t *)malloc(data_chunk->size);
            if (data_chunk->data == NULL) {
                fprintf(stderr, "Error: Could not allocate memory for data\n");
                exit(1);
            }
            if (fread(data_chunk->data, data_chunk->size, 1, file) != 1) {
                fprintf(stderr, "Error: Could not read data\n");
                exit(1);
            }
            printf("Read \"%.4s\" block with size: %d\n", chunk_id, data_chunk->size);
            break;
        }
        else {
            int64_t chunk_size = 0;
            fread(&chunk_size, 4, 1, file);

            // Move file pointer to the beginning of the next chunk
            fseek(file, chunk_size, SEEK_CUR);
            printf("Skipping \"%.4s\" chunk with length %d\n", chunk_id, chunk_size);
        }
    }
    if (data_found == 0) {
        fprintf(stderr, "Error: Could not find data chunk\n");
        exit(1);
    }
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
