#include "audio.h"


void wav_read_audio_file(const char *filename, wav_header **header, wav_data_chunk **data_chunk) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }

    wav_read_audio_header(file, header);
    wav_read_data_chunk(file, *header, data_chunk);
    fclose(file);
}

void wav_read_audio_header(FILE *file, wav_header **header) {
    *header = malloc(sizeof(wav_header));
    if (header == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for wav_header.\n");
        exit(1);
    }

    fread(*header, sizeof(wav_header), 1, file);
}

void wav_read_data_chunk(FILE *file, wav_header *header, wav_data_chunk **data_chunk) {
    // For now don't care about other chunk ids except data
    char chunk_id[4];
    while (fread(chunk_id, 4, 1, file) == 1) {
        if (strncmp(chunk_id, "data", 4) == 0) {
            *data_chunk = malloc(sizeof(wav_data_chunk));
            if (fread(&((*data_chunk)->size), 4, 1, file) != 1) {
                fprintf(stderr, "Error: Could not read data size\n");
                exit(1);
            }
            uint8_t *data = (uint8_t *)malloc((*data_chunk)->size);
            if (data == NULL) {
                fprintf(stderr, "Error: Could not allocate memory for data\n");
                exit(1);
            }
            if (fread(data, (*data_chunk)->size, 1, file) != 1) {
                fprintf(stderr, "Error: Could not read data\n");
                exit(1);
            }
            printf("Read \"%.4s\" block with size: %d\n", chunk_id, (*data_chunk)->size);
            uint32_t num_samples = (*data_chunk)->size / (header->sample_alignment);
            assert(num_samples * header->sample_alignment == (*data_chunk)->size);
            printf("Number of samples: %d\n", num_samples);
            printf("Length of recording: %d\n", num_samples / header->sample_rate);
            // // Copy data to their respective channels
            // data_chunk->channel_data = (uint8_t **)malloc(header->num_channels * sizeof(uint8_t *));
            // if (data_chunk->channel_data == NULL) {
            //     fprintf(stderr, "Error: Could not allocate memory for channel data\n");
            //     exit(1);
            // }
            // for (int i = 0; i < header->num_channels; i++) {
            //     data_chunk->channel_data[i] = (uint8_t *)malloc(data_chunk->size / header->num_channels);
            //     if (data_chunk->channel_data[i] == NULL) {
            //         fprintf(stderr, "Error: Could not allocate memory for channel data\n");
            //         exit(1);
            //     }
            // }
            
            // for (int i = 0; i < data_chunk->size; i += 2) {
                
            // }
            // free(data);
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
    if (data_chunk == NULL) {
        fprintf(stderr, "Error: Could not find data chunk\n");
        exit(1);
    }
}

void wav_print_header(wav_header *header) {
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