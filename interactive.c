#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "imageprocessing.h"

typedef struct img_data {
    int ***img;
    int N, M;
} img_data_t;

typedef struct filter_data {
    float **filter;
    int sz;
} filter_data_t;


int main()
{
    int max_img_cnt = 64;
    int current_img_cnt = 0;
    img_data_t *image_arr = (img_data_t *) calloc(max_img_cnt, sizeof(img_data_t));

    int max_filter_cnt = 64;
    int current_filter_cnt = 0;
    filter_data_t *filter_arr = (filter_data_t *) calloc(max_filter_cnt, sizeof(filter_data_t));

    char str_buffer[100];
    while (1) {
        scanf("%99s", str_buffer);

        if (strcmp(str_buffer, "e") == 0) {
            for (int i = 0; i < max_img_cnt; i++) {
                free_image(image_arr[i].img, image_arr[i].N, image_arr[i].M);
            }
            free(image_arr);
            return 0;
        }

        if (strcmp(str_buffer, "l") == 0) {

            int n, m;
            scanf("%d %d %99s", &n, &m, str_buffer);

            if (current_img_cnt == max_img_cnt) {
                printf("cannot add more imgages\n");
                continue;
            }

            int ***ptr = alloc_image(n, m);
            read_from_bmp(ptr, n, m, str_buffer);

            image_arr[current_img_cnt].img = ptr;
            image_arr[current_img_cnt].N = n;
            image_arr[current_img_cnt].M = m;

            current_img_cnt++;
        }

        if (strcmp(str_buffer, "s") == 0) {
            int idx;
            scanf("%d %99s", &idx, str_buffer);

            write_to_bmp(image_arr[idx].img, image_arr[idx].N, image_arr[idx].M, str_buffer);
        }

        if (strcmp(str_buffer, "ah") == 0) {
            int idx;
            scanf("%d", &idx);

            image_arr[idx].img = flip_horizontal(image_arr[idx].img, image_arr[idx].N, image_arr[idx].M);
        }

        if (strcmp(str_buffer, "ar") == 0) {
            int idx;
            scanf("%d", &idx);

            image_arr[idx].img = rotate_left(image_arr[idx].img, image_arr[idx].N, image_arr[idx].M);
        }

        if (strcmp(str_buffer, "ac") == 0) {
            int idx, x, y, w, h;
            scanf("%d %d %d %d %d", &idx, &x, &y, &w, &h);

            image_arr[idx].img = crop(image_arr[idx].img, image_arr[idx].N, image_arr[idx].M, x, y, h, w);
            image_arr[idx].N = h;
            image_arr[idx].M = w;
        }
        if (strcmp(str_buffer, "ae") == 0) {
            int idx, rows, cols, r, g, b;
            scanf("%d %d %d %d %d %d", &idx, &rows, &cols, &r, &g, &b);

            image_arr[idx].img = extend(image_arr[idx].img, image_arr[idx].N, image_arr[idx].M, rows, cols, r, g, b);
            image_arr[idx].N += rows * 2;
            image_arr[idx].M += cols * 2;
        }

        if (strcmp(str_buffer, "ap") == 0) {
            int idxd, idxs, x, y;
            scanf("%d %d %d %d", &idxd, &idxs, &x, &y);

            image_arr[idxd].img = paste(image_arr[idxd].img, image_arr[idxd].N, image_arr[idxd].M, image_arr[idxs].img, image_arr[idxs].N, image_arr[idxs].M, x, y);
        }

        if (strcmp(str_buffer, "cf") == 0) {
            int sz;
            scanf("%d", &sz);

            float **filter = malloc(sz * sizeof(float *));
            for (int i = 0; i < sz; i++) {
                filter[i] = malloc(sz * sizeof(float));
                for (int j = 0; j < sz; j++) {
                    scanf("%f", &filter[i][j]);
                }
            }
            filter_arr[current_filter_cnt].filter = filter;
            filter_arr[current_filter_cnt++].sz = sz;
        }

        if (strcmp(str_buffer, "af") == 0) {
            int idxi, idxf;
            scanf("%d %d", &idxi, &idxf);

            image_arr[idxi].img = apply_filter(image_arr[idxi].img, image_arr[idxi].N, image_arr[idxi].M, filter_arr[idxf].filter, filter_arr[idxf].sz);
        }

        if (strcmp(str_buffer, "df") == 0) {
            int idxf;
            scanf("%d", &idxf);

            free(filter_arr[idxf].filter);

            for (int i = idxf; i < max_filter_cnt - 1; i++) {
                filter_arr[i] = filter_arr[i + 1];
            }
            current_filter_cnt--;
        }

        if (strcmp(str_buffer, "di") == 0) {
            int idxi;
            scanf("%d", &idxi);

            free_image(image_arr[idxi].img, image_arr[idxi].N, image_arr[idxi].M);

            for (int i = idxi; i < max_img_cnt - 1; i++) {
                image_arr[i] = image_arr[i + 1];
            }
            current_img_cnt--;
        }

    }
	
    return 0;
}
