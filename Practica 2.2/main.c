/* 
 * File:   main.c
 * Author: matias
 *
 * Created on 17 de marzo de 2015, 21:59
 */
#define BILLION 1000000000L

#define ANCHO 16
#define ALTO 16


#include <stdint.h> /* for uint64 definition */
#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <time.h>  

unsigned int diferenciaBloque(IplImage* img1, int img1f, int img1c, IplImage* img2, int img2f, int img2c, int alto, int ancho) {

    unsigned int diferencia = 0;

    int fila, columna;

    for (fila = 0; fila < alto; fila++) {

        unsigned char* pImg1 = (unsigned char*) (img1->imageData + (img1f + fila) * img1->widthStep + img1c * img1->nChannels);
        unsigned char* pImg2 = (unsigned char*) (img2->imageData + (img2f + fila) * img2->widthStep + img2c * img2->nChannels);

        int dif;

        for (columna = 0; columna < ancho; columna++) {

            dif = *pImg1++ - *pImg2++;
            if (dif < 0)
                dif = -dif;
            diferencia += dif;
            dif = *pImg1++ - *pImg2++;
            if (dif < 0)
                dif = -dif;
            diferencia += dif;
            dif = *pImg1++ - *pImg2++;
            if (dif < 0)
                dif = -dif;
            diferencia += dif;
        }
    }
    return (diferencia);
}

void copiarBloque(int i, int j, IplImage *imgOrigen, int k, int l, IplImage *imgDestino) {

    int fila, columna;

    for (fila = 0; fila < ALTO; fila++) {
        unsigned char * bloqueDestino = (unsigned char *) (imgDestino->imageData + (k * ALTO + fila) * imgDestino->widthStep + l * ANCHO * imgDestino->nChannels);
        unsigned char * bloqueOrigen = (unsigned char *) (imgOrigen->imageData + (i * ALTO + fila) * imgOrigen->widthStep + j * ANCHO * imgOrigen->nChannels);
        for (columna = 0; columna < ANCHO; columna++) {
            *bloqueDestino++ = *bloqueOrigen++;
            *bloqueDestino++ = *bloqueOrigen++;
            *bloqueDestino++ = *bloqueOrigen++;
        }
    }
}

void degradadoBlack(IplImage *img1) {
    int i, j;
    for (i = 0; i < img1->height; i++) {
        unsigned char * pImg1 = (unsigned char *) (img1->imageData + i * img1->widthStep);
        //        unsigned char * pImg2 = (unsigned char *)(img2->imageData + i*img2->widthStep);
        for (j = 0; j < img1->width * 3; j++) {
            if (*pImg1 > 0)
                (*pImg1)--;
            *pImg1++;
        }
    }

}

void alumbrado(IplImage *img1, IplImage *img2) {
    int i, j;
    for (i = 0; i < img1->height; i++) {
        unsigned char * pImg1 = (unsigned char *) (img1->imageData + i * img1->widthStep);
        unsigned char * pImg2 = (unsigned char *) (img2->imageData + i * img2->widthStep);
        for (j = 0; j < img1->width * 3; j++) {
            if (*pImg1 < *pImg2)
                (*pImg1)++;
            *pImg1++;
            *pImg2++;
        }
    }

}

void degradadoDirecto(IplImage *img1, IplImage *img2) {
    int i, j;
    for (i = 0; i < img1->height; i++) {
        unsigned char * pImg1 = (unsigned char *) (img1->imageData + i * img1->widthStep);
        unsigned char * pImg2 = (unsigned char *) (img2->imageData + i * img2->widthStep);
        for (j = 0; j < img1->width * 3; j++) {
            if (*pImg1 < *pImg2)
                (*pImg1)++;
            if (*pImg1 > *pImg2)
                (*pImg1)--;
            *pImg1++;
            *pImg2++;
        }
    }
}

void degradar_pixel(IplImage* image1) {
    
    int i;
    unsigned char *pImg1 = (unsigned char *) image1->imageData;
    for (i = 0; i < image1->imageSize; i++) {
        if (*pImg1 > 0)
            (*pImg1)--;
        *pImg1++;
    }
}

int main(int argc, char** argv) {

    if (!((argc == 3) || (argc == 1))) {
        printf("Usage: %s \n", argv[0]);
        printf("or\n", argv[0]);
        printf("       %s image2Modificar imagePatron\n", argv[0]);
        return EXIT_FAILURE;
    }

    char variableChar;
    int variableInt;
    float variableFloat;

    char * pchar;
    int * pint;
    float * pfloat;

    double *pdouble;
    double variableDouble = 2.0;

    printf("\npdouble es un puntero que contiene la direccion de memoria %d", pdouble);
    printf("\nvariableDouble es una variable que contiene el real %f", variableDouble);
    printf("\nvariableDouble esta en memoria en la direccion %d", variableDouble);

    pdouble = &variableDouble;
    printf("\n\npdouble apunta ahora a la direccion donde esta variableDouble: %d", pdouble);
    printf("\n\nNo es lo mismo %f que %d", variableDouble, pdouble);
    printf("\nEs lo mismo %d que %d", &variableDouble, pdouble);
    printf("\nEs lo mismo %f que %f", variableDouble, *pdouble);

    printf("\n");

    IplImage* imgModificada;

    if (argc == 3) {
        imgModificada = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
    } else {
        printf("Imagen %s no cargada /n", argv[1]);
        return EXIT_FAILURE;
    }

    if (!imgModificada) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }
    //a visualization window is created with title 'image'
    cvNamedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);
    cvShowImage("imgOriginal", imgModificada);
    cvWaitKey(0);

    IplImage* imgPatron;
    if (argc == 3) {
        imgPatron = cvLoadImage(argv[2], CV_LOAD_IMAGE_COLOR);
    } else {
        printf("Imagen %s no cargada /n", argv[2]);
        return EXIT_FAILURE;
    }

    // Always check if the program can find a file
    if (!imgPatron) {
        printf("Error: fichero %s no leido\n", argv[2]);
        return EXIT_FAILURE;
    }
    int i;

    //    for (i = 0; i < 255; i++) {
    //        degradadoBlack(imgModificada);
    //        cvShowImage("imgOriginal", imgModificada);
    //        cvWaitKey(1);
    //    }

    //    for (i = 0; i < 255; i++) {
    //        alumbrado(imgModificada, imgPatron);
    //        cvShowImage("imgOriginal", imgModificada);
    //        cvWaitKey(1);
    //    }

    for (i = 0; i < 255; i++) {
        degradar_pixel(imgModificada);
        //        degradadoDirecto(imgModificada, imgPatron);
        cvShowImage("imgOriginal", imgModificada);
        cvWaitKey(1);
    }

    //    a visualization window is created with title 'image'
    //    cvNamedWindow("imgPatron", CV_WINDOW_AUTOSIZE);
    //    cvShowImage("imgPatron", imgPatron);
    //    cvWaitKey(0);
    //
    //
    //    struct timespec start, finish;
    //    float elapsed;
    //
    //    clock_gettime(CLOCK_MONOTONIC, &start);
    //
    //    int fila1, columna1;
    //    int fila2, columna2;
    //
    //    //recorre filas de la imagen a modificar
    //    for (fila1 = 0; fila1 < imgModificada->height; fila1 += ALTO) {
    //
    //        for (columna1 = 0; columna1 < imgModificada->width; columna1 += ANCHO) {
    //
    //            unsigned int diferenciaProvisional = 0xFFFFFFFF;
    //            unsigned int diferencia;
    //            int filaProvisional, columnaProvisional;
    //
    //            //recorre filas de la imagen patron
    //            for (fila2 = 0; fila2 < imgPatron->height; fila2 += ALTO) {
    //
    //                for (columna2 = 0; columna2 < imgPatron->width; columna2 += ANCHO) {
    //
    //                    //printf("\nComparando Imagen 1 %d fila, %d columna y Imagen2 %d fila, %d columna", fila1, columna1, fila2, columna2);
    //
    //                    diferencia = diferenciaBloque(imgModificada, fila1, columna1, imgPatron, fila2, columna2, ALTO, ANCHO);
    //                    if (diferencia < diferenciaProvisional) {
    //                        diferenciaProvisional = diferencia;
    //                        filaProvisional = fila2;
    //                        columnaProvisional = columna2;
    //                    }
    //                    //printf("\n Procesando el bloque %d fila, %d columna de la imagen 1, diferencia %d", fila1, columna1, diferencia);
    //                }
    //
    //            }
    //
    //            copiarBloque(filaProvisional / ALTO, columnaProvisional / ANCHO, imgPatron, fila1 / ALTO, columna1 / ANCHO, imgModificada);
    //
    //        }
    //    }
    //
    //
    //    clock_gettime(CLOCK_MONOTONIC, &finish);
    //    elapsed = (finish.tv_sec - start.tv_sec);
    //    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    //
    //    printf("MOSAICO: Tiempo transcurrido: %f", elapsed);
    //
    //    cvNamedWindow("MOSAICO", CV_WINDOW_AUTOSIZE);
    //    cvShowImage("MOSAICO", imgModificada);
    //    cvWaitKey(0);
    //
    //
    //    int p[3];
    //    p[0] = CV_IMWRITE_JPEG_QUALITY;
    //    p[1] = 100;
    //    p[2] = 0;


    return (EXIT_SUCCESS);
}