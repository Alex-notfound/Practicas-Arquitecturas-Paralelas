#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <emmintrin.h>

void FundidoABytes(IplImage* Img1, IplImage* Img2);

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }
    IplImage* Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* Img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);

    // Always check if the program can find the image file
    if (!Img1 || !Img2) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    FundidoABytes(Img1, Img2);

    cvNamedWindow("Ventana2", CV_WINDOW_AUTOSIZE);
    cvShowImage("Ventana2", Img1);
    cvWaitKey(0);

    
    //Fundido a negro con paralelismo:
    __m128i unos = _mm_set1_epi8(1);
    IplImage* ImgAux = cvCloneImage(Img1);
    int i, byte;

    for (i = 0; i <= 255; i++) {
        __m128i *pImgAux = (__m128i *) (ImgAux->imageData);
        for (byte = 0; byte < ImgAux->imageSize; byte += 16) {
            __m128i A = _mm_loadu_si128(pImgAux);
            __m128i B = _mm_subs_epu8(A, unos);
            _mm_storeu_si128(pImgAux, B);
            pImgAux++;
        }
        cvShowImage("Ventana2", ImgAux);
        cvWaitKey(1);
    }

    cvReleaseImage(&Img1);
    cvReleaseImage(&Img2);
    cvReleaseImage(&ImgAux);
    cvDestroyWindow("Ventana2");

    return EXIT_SUCCESS;
}

void FundidoABytes(IplImage* Img1, IplImage* Img2) {

    cvNamedWindow("Ventana", CV_WINDOW_AUTOSIZE);
    cvShowImage("Ventana", Img1);
    cvWaitKey(0);

    int fila;
    int i;
    for (i = 0; i < 255; i++) {

        unsigned char *pImg1 = (unsigned char *) Img1->imageData;
        for (fila = 0; fila < Img1->imageSize; fila++) {
            if (*pImg1 > 0)
                (*pImg1)--;
            *pImg1++;
        }
        cvShowImage("Ventana", Img1);
        cvWaitKey(1);
    }

    for (i = 0; i < 255; i++) {

        unsigned char *pImg1 = (unsigned char *) Img1->imageData;
        unsigned char *pImg2 = (unsigned char *) Img2->imageData;
        for (fila = 0; fila < Img1->imageSize; fila++) {
            if (*pImg1 < *pImg2)
                (*pImg1)++;
            *pImg1++;
            *pImg2++;
        }
        cvShowImage("Ventana", Img1);
        cvWaitKey(1);
    }

    cvDestroyWindow("Ventana");
}
