#include "include/main.h"


int main() {
    wav_header *header; 
    wav_data_chunk *data_chunk;

    // Audio file stuff
    wav_read_audio_file("C:/Users/prime/pianotranscription/lib/audio/test_file2.wav", &header, &data_chunk);
    wav_print_header(header);
    wav_print_recording_info(header, data_chunk);

    fft(header, data_chunk, 8096);
    // FFT stuff



    return 0;
}