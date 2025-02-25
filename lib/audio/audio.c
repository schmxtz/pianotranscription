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
            if (*data_chunk == NULL) {
                fprintf(stderr, "Error: Could not allocate memory for data chunk\n");
                exit(1);
            }
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
            if ((*data_chunk)->size % header->sample_alignment != 0) {
                fprintf(stderr, "Data size is not a multiple of sample alignment. There might be some error in the recording.\n");
            }

            // Copy data to their respective channels
            (*data_chunk)->channel_data = malloc(sizeof(float complex *) * header->num_channels);
            if ((*data_chunk)->channel_data == NULL) {
                fprintf(stderr, "Error: Could not allocate memory for channel data\n");
                exit(1);
            }

            int32_t size = (*data_chunk)->size;
            int32_t num_channels = header->num_channels;
            int32_t sample_alignment = header->sample_alignment;
            int32_t bytes_per_sample = header->bit_depth / 8;
            int32_t num_samples = size / sample_alignment;
            // Init channel data memory
            for (int i = 0; i < num_channels; i++) {
                (*data_chunk)->channel_data[i] = malloc(sizeof(float complex) * num_samples);
                if ((*data_chunk)->channel_data[i] == NULL) {
                    fprintf(stderr, "Error: Could not allocate memory for channel data\n");
                    exit(1);
                }
            }
            // Process data and put into channel data
            int32_t sample;
            int32_t data_index = 0;
            for (int sample_index = 0; sample_index < num_samples; sample_index++) {
                for (int channel_index = 0; channel_index < num_channels; channel_index++) {
                    data_index = sample_index * sample_alignment + channel_index * bytes_per_sample;
                    sample = 0;
                    // TODO: Read bytes and create complex                    
                }
            }    
            // for (int i = 0; i < header->num_channels; i++) {
            //     data_chunk->channel_data[i] = (uint8_t *)malloc(data_chunk->size / header->num_channels);
            //     if (data_chunk->channel_data[i] == NULL) {
            //         fprintf(stderr, "Error: Could not allocate memory for channel data\n");
            //         exit(1);
            //     }
            // }
            
            // for (int i = 0; i < data_chunk->size; i += 2) {
                
            // }
            free(data);
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
    printf("----- WAV file header: -----\n");
    printf("RIFF header: %.4s\n", header->riff_header);
    printf("File size: %d\n", header->wav_size);
    printf("WAVE header: %.4s\n", header->wave_header);
    printf("Format header: %.4s\n", header->fmt_header);
    printf("Format chunk size: %d\n", header->fmt_chunk_size);
    printf("Audio format (1: PCM, 3: IEEE Float): %d\n", header->audio_format);
    printf("Number of channels: %d\n", header->num_channels);
    printf("Sample rate: %d\n", header->sample_rate);
    printf("Byte rate: %d\n", header->byte_rate);
    printf("Sample alignment in bytes: %d\n", header->sample_alignment);
    printf("Bits per sample (per channel): %d\n", header->bit_depth);
}

void wav_print_recording_info(wav_header *header, wav_data_chunk *data_chunk) {
    uint32_t num_samples = data_chunk->size / header->sample_alignment;
    uint32_t recording_length_seconds = num_samples / header->sample_rate;
    printf("--- WAV file information: ---\n");
    printf("Number of samples: %d\n", num_samples);
    printf("Length of recording: %dm %ds\n", recording_length_seconds / 60, recording_length_seconds % 60);
}