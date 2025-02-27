#include "include/main.h"


int main() {
    wav_header *header; 
    wav_data_chunk *data_chunk;
    wav_read_audio_file("C:/Users/prime/pianotranscription/lib/audio/test_file.wav", &header, &data_chunk);
    wav_print_header(header);
    wav_print_recording_info(header, data_chunk);
    return 0;
}