#include "../include/windowLogic.h"

unsigned char *** createMatriz(ALLEGRO_BITMAP *image){
	const int width  = al_get_bitmap_width(image);
	const int height = al_get_bitmap_height(image);
	printf("ancho :  %d\n", al_get_bitmap_width(image));
	printf("alto  :  %d\n", al_get_bitmap_height(image));
	unsigned char *** ptr_image = allocateMemorySpaceForImage(&height, &width);
	ALLEGRO_COLOR colorpixel =  al_get_pixel(image,0,0);
	unsigned char r,g,b;
	al_unmap_rgb(colorpixel,&r, &g, &b);
	printf("%d\n", r);
	 
	for (int i = height - 1; i >= 0 ; i--)
       	for (int j = width - 1; j >= 0 ; j--)
        {
            colorpixel = al_get_pixel(image,j,i);
            al_unmap_rgb(colorpixel,&r, &g, &b);
            ptr_image[i][j][0] = (unsigned char) r;
            ptr_image[i][j][1] = (unsigned char) g;
            ptr_image[i][j][2] = (unsigned char) b;
            printf("%d\n", r);
        }
	return ptr_image;
}

unsigned char *** allocateMemorySpaceForImage(const int * ptr_height, const  int * ptr_width){
    unsigned char *** img = (unsigned char ***)calloc(*ptr_height, sizeof(unsigned char **));

    if (img == NULL) {
        fprintf(stderr, "Out of memory");
        exit(0);
    }

    for (int i = 0; i < *ptr_height; i++)
    {
        img[i] = (unsigned char **)calloc(*ptr_width, sizeof(unsigned char*));

        if (img[i] == NULL)
        {
            fprintf(stderr, "Out of memory");
            exit(0);
        }

        for (int j = 0; j < *ptr_width; j++)
        {
            img[i][j] = (unsigned char *)calloc(3, sizeof(unsigned char));

            if (img[i][j] == NULL)
            {
                fprintf(stderr, "Out of memory");
                exit(0);
            }
        }
    }
    return img;
}



void medianFilter(ALLEGRO_BITMAP *image){
	int * ptr_height;
	int * ptr_width;
    unsigned char *** input_img = createMatriz(image);
    unsigned char *** output_img;

	*ptr_height = al_get_bitmap_height(image);
    *ptr_width  = al_get_bitmap_width(image);


    int scaled_filter_size = WINDOW_FILTER_SIZE / 2;
    int neighborhood_size = WINDOW_FILTER_SIZE * WINDOW_FILTER_SIZE;
    unsigned char * neighborhood = calloc(neighborhood_size, sizeof(unsigned char));

    //Go throughout the pixels of the image
    for (int i = 0; i < *ptr_height; i++)
        for (int j = 0; j < *ptr_width; j++)
        {
            if ((i >= scaled_filter_size && i <= *ptr_height - scaled_filter_size - 1) &&
                (j >= scaled_filter_size && j <= *ptr_width - scaled_filter_size - 1))
            {
                //Go throughout the neighborhood of the current pixel
                for (int x = i - scaled_filter_size, nx = 0; nx < WINDOW_FILTER_SIZE; x++, nx++)
                    for (int y = j - scaled_filter_size, ny = 0; ny < WINDOW_FILTER_SIZE; y++, ny++)
                        //Add the pixel to the current neighborhood
                        *(neighborhood + nx * WINDOW_FILTER_SIZE + ny) = input_img[x][y][R_CHANNEL];

                int median = calculateMedian(neighborhood, neighborhood_size);

                //Set the median value of the neighborhood of the current pixel in RGB channels
                output_img[i][j][R_CHANNEL] = median;
                output_img[i][j][G_CHANNEL] = median;
                output_img[i][j][B_CHANNEL] = median;
            }
            //Keep border pixels value unchanged
            else
            {
                output_img[i][j][R_CHANNEL] = input_img[i][j][R_CHANNEL];
                output_img[i][j][G_CHANNEL] = input_img[i][j][R_CHANNEL];
                output_img[i][j][B_CHANNEL] = input_img[i][j][R_CHANNEL];
            }
        }

    free(neighborhood);
}


int calculateMedian(unsigned char * arr, int length){
    //Sort the elements of the array
    for (int i = 0; i < length; i++)
        for (int j = i + 1; j < length; j++)
        {
            if (*(arr + j) < *(arr + i))
            {
                int temp = *(arr + i);
                *(arr + i) = *(arr + j);
                *(arr + j) = temp;
            }
        }

    return *(arr + (length / 2));
}