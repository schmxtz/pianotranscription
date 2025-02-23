#include "main.h"
#include "lib/audio/audio.h"


int main(int argc, char **argv) {
    wav_header *header = malloc(sizeof(wav_header));
    if (header == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for wav_header.\n");
        exit(1);
    }

    data_chunk *data_chunk = malloc(sizeof(data_chunk));
    if (data_chunk == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for data.\n");
        exit(1);
    }
    read_audio_file("C:/Users/prime/pianotranscription/lib/audio/test_file.wav", header, data_chunk);
    // printf("%c%c%c APWDPWAD", data_chunk->data[5], data_chunk->data[6], data_chunk->data[7]);
    return 0;
}