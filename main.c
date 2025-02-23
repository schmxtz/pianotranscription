#include "main.h"
#include "lib/audio/audio.h"


int main(int argc, char **argv) {
    wav_header *header = malloc(sizeof(wav_header));
    if (header == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for wav_header.\n");
        exit(1);
    }

    data_chunk *data = malloc(sizeof(data_chunk));
    if (data == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for data.\n");
        exit(1);
    }
    read_audio_file("C:/Users/prime/pianotranscription/lib/audio/test_file.wav", header, data);
    printf("%c%c%c%c\n", data->data[0], data->data[1], data->data[2], data->data[3]);
    return 0;
}