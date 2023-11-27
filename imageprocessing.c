#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO Task 1

void swap_ptrs(void **a, void **b) {
    void *tmp = *a;
    *a = *b;
    *b = tmp;
}

int ***alloc_image(int N, int M) {
    int ***new_image = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        new_image[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            new_image[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    return new_image;
}
int ***alloc_image_rgb(int N, int M, int r, int g, int b) {
    int ***new_image = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        new_image[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            new_image[i][j] = (int *)malloc(3 * sizeof(int));
            new_image[i][j][0] = r;
            new_image[i][j][1] = g;
            new_image[i][j][2] = b;
        }
    }
    return new_image;
}
void free_image(int ***img, int N, int M) {
    if (!img) 
        return;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(img[i][j]);
        }
        free(img[i]);
    }
    free(img);
}


int ***flip_horizontal(int ***image, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M / 2; j++) {
            swap_ptrs((void **)&image[i][j], (void **)&image[i][M - j - 1]);
        }
    }
    return image;
}

// TODO Task 2
int ***rotate_left(int ***image, int N, int M) {
    int ***new_image = alloc_image(M, N);

	for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            swap_ptrs((void **)&new_image[M - j - 1][i], (void **)&image[i][j]);
        }
    }
    free_image(image, N, M);
    return new_image;
}

// TODO Task 3
int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
	int ***new_image = alloc_image(h, w);
    for (int i = x; i < x + h; i++) {
        for (int j = y; j < y + w; j++) {
            swap_ptrs((void **)&new_image[i - x][j - y], (void **)&image[i][j]);
        }
    }
    free_image(image, N, M);
    return new_image;
}

// TODO Task 4
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    int new_rows = 2 * rows + N, new_cols = 2 * cols + M;
    
    int ***new_image = alloc_image_rgb(new_rows, new_cols, new_R, new_G, new_B);
    for (int i = rows; i < rows + N; i++) {
        for (int j = cols ; j < cols + M; j++) {
            swap_ptrs((void **)&new_image[i][j], (void **)&image[i - rows][j - cols]);
        }
    }
    free_image(image, N, M);
    return new_image;
}

// TODO Task 5
int ***paste(int ***image_dst, int N_dst, int M_dst, int *** image_src, int N_src, int M_src, int x, int y) {
	int minN = N_dst - x > N_src ? N_src : N_dst - x;
    int minM = M_dst - y > M_src ? M_src : M_dst - y;

    for (int i = 0; i < minN; i++) {
        for (int j = 0; j < minM; j++) {
            for (int k = 0; k < 3; k++) {
                image_dst[i+x][j+y][k] = image_src[i][j][k];
            }
        }
    } 

    return image_dst;

}

int is_within_borders(int x, int y, int N, int M) {
    if (x < 0 || y < 0)
        return 0;
    if (x >= N || y >= M)
        return 0;
    return 1;
}
void apply_filter_to_one(int ***output_image, int ***input_image, int x, int y, int N, int M, float **filter, int filter_size) {
    int newN = x - filter_size / 2;
    int newM = y - filter_size / 2;

    float color_total[3] = {0, 0, 0};
    float color[3];

    for (int i = newN; i < newN + filter_size; i++) {
        for (int j = newM; j < newM + filter_size; j++) {
            if (is_within_borders(i, j, N, M)) {
                for (int k = 0; k < 3; k++) {
                    color[k] = input_image[i][j][k];
                }
            } else {
                color[0] = color[1] = color[2] = 0;
            }
            for (int k = 0; k < 3; k++) {
                color_total[k] += color[k] * filter[i - newN][j - newM];
            }
        }
    }
    for (int k = 0; k < 3; k++) {
        if (color_total[k] < 0) {
            color_total[k] = 0;
        }
        if (color_total[k] > 255) {
            color_total[k] = 255;
        }
        output_image[x][y][k] = (int)color_total[k];
    }
}

// TODO Task 6
int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    int ***output = alloc_image(N, M);

	for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            apply_filter_to_one(output, image, i, j, N, M, filter, filter_size);
        }
    }

    free_image(image, N, M);
    return output;
}



void read_from_bmp(int ***pixel_matrix, int N, int M, const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            unsigned char color[3];
            fread(color, sizeof(unsigned char), 3, file);
            pixel_matrix[N-i-1][j][0] = (int)color[2]; // Red
            pixel_matrix[N-i-1][j][1] = (int)color[1]; // Green
            pixel_matrix[N-i-1][j][2] = (int)color[0]; // Blue
        }
    }

    fclose(file);
}

void write_to_bmp(int ***pixel_matrix, int N, int M, const char *path) {
    FILE *file = fopen(path, "wb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    unsigned char header[54] = {
        0x42, 0x4D, // BMP signature
        0, 0, 0, 0, // File size
        0, 0, 0, 0, // Reserved
        54, 0, 0, 0, // Data offset
        40, 0, 0, 0, // Header size
        0, 0, 0, 0, // Width
        0, 0, 0, 0, // Height
        1, 0,       // Planes
        24, 0,      // Bits per pixel
        0, 0, 0, 0, // Compression
        0, 0, 0, 0, // Image size
        0, 0, 0, 0, // X pixels per meter
        0, 0, 0, 0, // Y pixels per meter
        0, 0, 0, 0, // Total colors
        0, 0, 0, 0  // Important colors
    };

    int padding = (4 - (M * 3) % 4) % 4;
    int fileSize = 54 + 3 * N * M;
    *(int *)&header[2] = fileSize;
    *(int *)&header[18] = M;
    *(int *)&header[22] = N;

    fwrite(header, sizeof(unsigned char), 54, file);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            unsigned char color[3];
            color[2] = (unsigned char)(pixel_matrix[N-i-1][j][0]); // Red
            color[1] = (unsigned char)(pixel_matrix[N-i-1][j][1]); // Green
            color[0] = (unsigned char)(pixel_matrix[N-i-1][j][2]); // Blue
            fwrite(color, sizeof(unsigned char), 3, file);
        }
        for (int k = 0; k < padding; k++) {
            fputc(0, file);
        }
    }

    fclose(file);
}