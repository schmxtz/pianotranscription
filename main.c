#include "main.h"
#include "lib/audio/audio.h"


int main(int argc, char **argv) {
    wav_header *wav_header; 
    wav_data_chunk *wav_data_chunk;
    wav_read_audio_file("C:/Users/prime/pianotranscription/lib/audio/test_file.wav", wav_header, wav_data_chunk);
    wav_print_header(wav_header);
    return 0;
}