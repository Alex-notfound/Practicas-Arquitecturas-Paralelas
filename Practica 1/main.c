#define BLOCK_SIZE 16

#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


int compararBloques(int filaImg1Bloque1, int colBloque1, IplImage* imagen1, int filaImg1Bloque2, int colBloque2, IplImage* imagen2);
void sustituirBloque(int filaImg1Bloque1, int colBloque1, IplImage* imagen1, int filaImg1Bloque2, int colBloque2, IplImage* imagen2);
void crearMosaico(IplImage* img1, IplImage* img2);

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);

    if (!img1 || !img2) {
        printf("Error: file %s or %s not found\n", argv[1], argv[2]);
        return EXIT_FAILURE;
    }

    cvNamedWindow("Ventana", CV_WINDOW_AUTOSIZE);
    cvShowImage("Ventana", img1);
    cvWaitKey(0);
    cvDestroyWindow("Ventana");

    crearMosaico(img1, img2);

    cvNamedWindow("tachan", CV_WINDOW_AUTOSIZE);
    cvShowImage("tachan", img1);
    cvWaitKey(0);

    printf("H");

    cvReleaseImage(&img1);
    cvReleaseImage(&img2);
    cvDestroyWindow("tachan");

    return (EXIT_SUCCESS);
}

int compararBloques(int filaImg1Bloque1, int colBloque1, IplImage* imagen1, int filaImg1Bloque2, int colBloque2, IplImage* imagen2) {

    int i, col, diferencia = 0;

    for (i = 0; i < BLOCK_SIZE; i++) {
        unsigned char *pImg1 = (unsigned char *) imagen1->imageData + (filaImg1Bloque1 + i) * (imagen1->widthStep) + (colBloque1 * 3);
        unsigned char *pImg2 = (unsigned char *) imagen2->imageData + (filaImg1Bloque2 + i) * (imagen2->widthStep) + (colBloque2 * 3);
        for (col = 0; col < BLOCK_SIZE * 3; col++) {
            diferencia += abs(*pImg1++ - *pImg2++);
        }
    }
    return diferencia;
}

void sustituirBloque(int filaImg1Bloque1, int colBloque1, IplImage* imagen1, int filaImg1Bloque2, int colBloque2, IplImage* imagen2) {

    int fila, columna;

    for (fila = 0; fila < BLOCK_SIZE; fila++) {

        unsigned char *pImg1 = (unsigned char *) imagen1->imageData + ((filaImg1Bloque1 + fila) * imagen1->widthStep) + (colBloque1 * 3);
        unsigned char *pImg2 = (unsigned char *) imagen2->imageData + ((filaImg1Bloque2 + fila) * imagen2->widthStep) + (colBloque2 * 3);

        for (columna = 0; columna < BLOCK_SIZE; columna++) {
            *pImg1++ = *pImg2++;
            *pImg1++ = *pImg2++;
            *pImg1++ = *pImg2++;
        }
    }
}

void crearMosaico(IplImage* img1, IplImage* img2) {

    int filaImg1, columnaImg1, diferencia, filaSel, colSel, filaImg2, columnaImg2, diferenciaAcum;

    for (filaImg1 = 0; filaImg1 < (img1->height / BLOCK_SIZE); filaImg1++) {

        for (columnaImg1 = 0; columnaImg1 < (img1->width / BLOCK_SIZE); columnaImg1++) {

            diferenciaAcum = BLOCK_SIZE * BLOCK_SIZE * 255 * 3;

            for (filaImg2 = 0; filaImg2 < (img2->height / BLOCK_SIZE); filaImg2++) {


                for (columnaImg2 = 0; columnaImg2 < (img1->width / BLOCK_SIZE); columnaImg2++) {

                    diferencia = compararBloques(filaImg1 * BLOCK_SIZE, columnaImg1 * BLOCK_SIZE, img1, filaImg2 * BLOCK_SIZE, columnaImg2 * BLOCK_SIZE, img2);

                    if (diferenciaAcum > diferencia) {
                        diferenciaAcum = diferencia;
                        filaSel = filaImg2;
                        colSel = columnaImg2;
                    }
                }
            }
            sustituirBloque(filaImg1 * BLOCK_SIZE, columnaImg1 * BLOCK_SIZE, img1, filaSel * BLOCK_SIZE, colSel * BLOCK_SIZE, img2);
        }
    }
}