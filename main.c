#include "include/main.h"


int main() {
    wav_header *header; 
    wav_data_chunk *data_chunk;

    // Audio file stuff
    wav_read_audio_file("C:/Users/prime/pianotranscription/lib/audio/test_file2.wav", &header, &data_chunk);
    wav_print_header(header);
    wav_print_recording_info(header, data_chunk);

    int32_t *fft_data;
    fft(header, data_chunk, &fft_data);
    // FFT stuff



    return 0;
}