#include "fft.h"


void fft(const wav_header *header, const wav_data_chunk *data_chunk, int32_t **fft_data, int32_t window_size) {
    (*fft_data) = (int32_t *) malloc(data_chunk->size * sizeof(int32_t *));
    (*fft_data)[0] = 1; 
    int32_t num_samples = data_chunk->size / header->sample_alignment;
    for (int i = 0; i < header->num_channels; i++) {
        int32_t max, min = data_chunk->channel_data[i * num_samples];
        for (int j = 0; j < num_samples; j++) {
            if (data_chunk->channel_data[i * num_samples + j] > max) {
                max = data_chunk->channel_data[i * num_samples + j];
            }
            if (data_chunk->channel_data[i * num_samples + j] < min) {
                min = data_chunk->channel_data[i * num_samples + j];
            }
            continue;
        }

    }
}

void rfft(int32_t *samples, double *ouput_magnitudes, int32_t N, int32_t min, int32_t max) {
    double complex *temp = (double complex *) malloc(N / 2 * sizeof(double complex));
    if (temp == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for complex temp array.\n");
        exit(1);
    }

    int32_t range = max - min;
    for (int32_t i = 0; i < N; i += 2) {
        temp[i] = normalize(samples[i], min, max, range) + I * normalize(samples[i + 1], min, max, range);
    }

    fft_complex(temp, N / 2);
    
    ouput_magnitudes[0] = calc_mag(creal(temp[0]) + cimag(temp[0]) + 0 * I);
    ouput_magnitudes[N / 2] = calc_mag(creal(temp[N / 2]) + cimag(temp[N / 2]) + 0 * I);
}

void fft_complex(double complex *x, int32_t N) {
    if (N <= 1) {
        return;
    }

    double complex *even = (double complex *) malloc(N / 2 * sizeof(double complex));
    double complex *odd = (double complex *) malloc(N / 2 * sizeof(double complex));
    if (even == NULL || odd == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for even and odd arrays.\n");
        exit(1);
    }

    for (int32_t i = 0; i < N; i += 2) {
        even[i] = x[i];
        odd[i] = x[i + 1];
    }

    fft_complex(even, N / 2);
    fft_complex(odd, N / 2);

    for (int32_t k = 0; k < N / 2; k++) {
        double angle = -2.0 * M_PI * k / N;
        double complex w = cos(angle) + I * sin(angle);
        double complex t = creal(odd[k]) * creal(w) - cimag(odd[k]) * cimag(w);
        x[k] += even[k] + t;
        x[k + N / 2] = even[k] - t;
    }

    free(even);
    free(odd);
}

double normalize(int32_t value, int32_t min, int32_t max, int32_t range) {
    return 2 * (value - min) / range - 1;
}

double calc_mag(double complex x) {
    return sqrt(creal(x) * creal(x) + cimag(x) * cimag(x));
}

double calc_freq(int32_t N, int32_t k, int32_t sample_rate) {
    return (double) k * sample_rate / N;
}