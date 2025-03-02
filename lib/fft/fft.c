#include "fft.h"


void fft(const wav_header *header, const wav_data_chunk *data_chunk, int32_t window_size) {
    complex *samples = (complex *) calloc(window_size / 2, sizeof(complex));
    double *output = (double *) malloc(4 * (window_size / 2 + 1) * sizeof(double));
    if (output == NULL || samples == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for FFT.\n");
        exit(1);
    }

    int32_t num_samples = data_chunk->size / header->sample_alignment;
    for (int32_t i = 0; i < num_samples - window_size; i += window_size) {
        for (int32_t j = 0; j < window_size / 2; j++) {
            // average of all channels
            double sample = 0;
            for (int32_t k = 0; k < header->num_channels; k++) {
                sample += data_chunk->channel_data[k * num_samples + i + 2 * j];
            }
            sample /= header->num_channels;
            samples[2 * j].real = sample;

            sample = 0;
            for (int32_t k = 0; k < header->num_channels; k++) {
                sample += data_chunk->channel_data[k * num_samples + i + 2 * j + 1];
            }
            sample /= header->num_channels;
            samples[2 * j + 1].imag = sample;
        }

        rfft(samples, window_size);

        extract_rffts_to_buffer(samples, output, window_size);

        double max = -1;
        int index = -1;
        for (int i = 0; i <= window_size / 2; i++) {
            double mag = sqrt(output[4 * i] * output[4 * i] + output[4 * i + 1] * output[4 * i + 1]);
            if (mag > max) {
                max = mag;
                index = i;
            }
        }
        double freq = index * (double) header->sample_rate / window_size;
        double mag = sqrt(output[4 * index] * output[4 * index] + output[4 * index + 1] * output[4 * index + 1]);
        printf("Frequency %.1f Hz: Magnitude %.4f\n", freq, mag);
    }
}

// Bit reversal for in-place FFT
void bit_reverse(complex *data, int32_t n) {
    int32_t i, j = 0;
    for (i = 0; i < n; i++) {
        if (i < j) {
            complex tmp = data[i];
            data[i] = data[j];
            data[j] = tmp;
        }
        int32_t m = n >> 1;
        while (m >= 1 && j >= m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
}

// In-place complex FFT (N must be power of 2)
void rfft(complex *data, int32_t n) {
    bit_reverse(data, n);
    for (int32_t len = 2; len <= n; len <<= 1) {
        double angle = -2.0 * M_PI / len;
        for (int32_t i = 0; i < n; i += len) {
            for (int32_t j = 0; j < len / 2; j++) {
                double cos_a = cos(j * angle);
                double sin_a = sin(j * angle);
                complex w = {cos_a, sin_a};
                complex a = data[i + j];
                complex b = data[i + j + len / 2];
                complex t = {b.real * w.real - b.imag * w.imag, 
                            b.real * w.imag + b.imag * w.real};
                data[i + j].real = a.real + t.real;
                data[i + j].imag = a.imag + t.imag;
                data[i + j + len / 2].real = a.real - t.real;
                data[i + j + len / 2].imag = a.imag - t.imag;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        data[i].real /= n;
        data[i].imag /= n;
    }
}

// Extract two RFFTs and interleave into one buffer
void extract_rffts_to_buffer(complex *fft_data, double *output, int32_t n) {
    output[0] = fft_data[0].real;  // First sequence, real
    output[1] = 0.0;               // First sequence, imag
    output[2] = fft_data[0].imag;  // Second sequence, real
    output[3] = 0.0;               // Second sequence, imag

    output[2 * (n / 2) + 0] = fft_data[n / 2].real;  // First sequence, real
    output[2 * (n / 2) + 1] = 0.0;                   // First sequence, imag
    output[2 * (n / 2) + 2] = fft_data[n / 2].imag;  // Second sequence, real
    output[2 * (n / 2) + 3] = 0.0;                   // Second sequence, imag

    for (int32_t k = 1; k < n / 2; k++) {
        complex z = fft_data[k];
        complex z_conj = {fft_data[n - k].real, -fft_data[n - k].imag};
        output[4 * k + 0] = (z.real + z_conj.real) / 2.0;  // real1
        output[4 * k + 1] = (z.imag + z_conj.imag) / 2.0;  // imag1
        output[4 * k + 2] = (z.imag - z_conj.imag) / 2.0;  // real2
        output[4 * k + 3] = -(z.real - z_conj.real) / 2.0; // imag2
    }
}


