#include "fft.h"


void fft(const wav_header *header, const wav_data_chunk *data_chunk, int32_t window_size) {
    window_size = 8;
    int32_t samples[8] = {1, 0, -1, 0, 1, 0, -1, 0};
    double *output_magnitudes = (double *) malloc((window_size / 2 + 1) * sizeof(double));
    if (output_magnitudes == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for output magnitudes array.\n");
        exit(1);
    }

    int32_t num_samples = data_chunk->size / header->sample_alignment;
    for (int32_t i = 0; i < header->num_channels; i++) {
        int32_t min, max;
        max = min = data_chunk->channel_data[i * num_samples];
        for (int32_t j = 0; j < num_samples; j++) {
            if (data_chunk->channel_data[i * num_samples + j] > max) {
                max = data_chunk->channel_data[i * num_samples + j];
            }
            if (data_chunk->channel_data[i * num_samples + j] < min) {
                min = data_chunk->channel_data[i * num_samples + j];
            }
        }
        rfft(samples, output_magnitudes, window_size);
        for (int32_t k = 0; k < window_size / 2 + 1; k++) {
            printf("Freq %.1f Hz: %.4f\n", calc_freq(8, k, 44100.0), calc_mag(output_magnitudes[k]));
        }
    }
}

void rfft(int32_t *samples, double *ouput_magnitudes, int32_t N) {
    double complex *temp = (double complex *) malloc((N / 2) * sizeof(double complex));
    if (temp == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for complex temp array.\n");
        exit(1);
    }

    for (int32_t i = 0; i < N / 2; i++) {
        temp[i] = samples[2 * i] + I * samples[2 * i + 1];
    }

    fft_complex(temp, N / 2);
    
    ouput_magnitudes[0] = calc_mag(creal(temp[0]) + cimag(temp[0]) + 0 * I);
    ouput_magnitudes[N / 2] = calc_mag(creal(temp[N / 2]) + cimag(temp[N / 2]) + 0 * I);

    for (int32_t k = 1; k < N / 2; k++) {
        double angle = M_PI * k / N;
        double complex a = temp[k];
        double complex b = creal(temp[N / 2 - k]) - cimag(temp[N / 2 - k]) * I;
        double complex w = cos(angle) + sin(angle) * I;
        ouput_magnitudes[k] = calc_mag(0.5 * (a + b) - I * 0.5 * w * (a - b));
    }

    free(temp);
}

void fft_complex(double complex *x, int32_t N) {
    if (N <= 1) {
        return;
    }

    double complex *even = (double complex *) malloc((N / 2) * sizeof(double complex));
    double complex *odd = (double complex *) malloc((N / 2) * sizeof(double complex));
    if (even == NULL || odd == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for even and odd arrays.\n");
        exit(1);
    }

    for (int32_t i = 0; i < N / 2; i++) {
        even[i] = x[2 * i];
        odd[i] = x[2 * i + 1];
    }

    fft_complex(even, N / 2);
    fft_complex(odd, N / 2);

    for (int32_t k = 0; k < N / 2; k++) {
        double angle = -2.0 * M_PI * k / N;
        double complex w = cos(angle) + I * sin(angle);
        double complex t = creal(odd[k]) * creal(w) - cimag(odd[k]) * cimag(w) + I * (creal(odd[k]) * cimag(w) + cimag(odd[k]) * creal(w));
        x[k + N / 2] = even[k] - t;
    }

    free(even);
    free(odd);
}

double normalize(int32_t value, int32_t min, int32_t range) {
    if (min == 0 && range == 0) {
        return 0.0;
    }
    return (double) value;
}    

double calc_mag(double complex x) {
    return sqrt(x * x);
}

double calc_freq(int32_t N, int32_t k, int32_t sample_rate) {
    return (double) k * sample_rate / N;
}