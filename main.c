#include "main.h"
#include "lib/audio/audio.h"


int main(int argc, char **argv) {
    wav_header *header = malloc(sizeof(wav_header));
    uint8_t *data = NULL;
    read_audio_file("C:/Users/prime/pianotranscription/lib/audio/test_file.wav", header, data);
    debug_print_header(header);
    return 0;
}