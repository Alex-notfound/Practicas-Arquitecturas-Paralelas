/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mainAzul.c
 * Author: ap1
 *
 * Created on 29 de enero de 2018, 12:07
 */

#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* Img = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);

    // Always check if the program can find the image file
    if (!Img) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    // a visualization window is created with title: image file name
    cvNamedWindow(argv[1],  CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window
    cvShowImage(argv[1], Img);
    cvWaitKey(0);

    // Crea la imagen para la componete azul
    IplImage* ImgBLUE = cvCreateImage(cvSize(Img->width, Img->height), IPL_DEPTH_8U, 3);

    int fila, columna;

    for (fila = 0; fila < Img->height; fila++) {

        unsigned char *pImg = (unsigned char *) Img->imageData + fila * Img->widthStep;
        unsigned char *pImgBLUE = (unsigned char *) ImgBLUE->imageData + fila * ImgBLUE->widthStep;

        for (columna = 0; columna < Img->width; columna++) {

            //Imagen BLUE
            *pImgBLUE++ = *pImg++;
            *pImgBLUE++ = 0;
            pImg++;
            *pImgBLUE++ = 0;
            pImg++;
        }
    }
    
    
    // Crea la imagen para la componete verde
    IplImage* ImgGREEN = cvCreateImage(cvSize(Img->width, Img->height), IPL_DEPTH_8U, 3);

    for (fila = 0; fila < Img->height; fila++) {

        unsigned char *pImg = (unsigned char *) Img->imageData + fila * Img->widthStep;
        unsigned char *pImgGREEN = (unsigned char *) ImgGREEN->imageData + fila * ImgGREEN->widthStep;

        for (columna = 0; columna < Img->width; columna++) {

            //Imagen GREEN
            *pImgGREEN++ = 0;
            pImg++;
            *pImgGREEN++ = *pImg++;
            *pImgGREEN++ = 0;
            pImg++;
        }
    }
    

    // Crea la imagen para la componete roja
    IplImage* ImgRED = cvCreateImage(cvSize(Img->width, Img->height), IPL_DEPTH_8U, 3);

    for (fila = 0; fila < Img->height; fila++) {

        unsigned char *pImg = (unsigned char *) Img->imageData + fila * Img->widthStep;
        unsigned char *pImgRED = (unsigned char *) ImgRED->imageData + fila * ImgRED->widthStep;

        for (columna = 0; columna < Img->width; columna++) {

            //Imagen GREEN
            *pImgRED++ = 0;
            pImg++;
            *pImgRED++ = 0;
            pImg++;
            *pImgRED++ = *pImg++;
            
        }
    }
    
    
    // crea y muestras las ventanas con las imagenes
    cvNamedWindow("Componente BLUE", CV_WINDOW_AUTOSIZE);
    cvShowImage("Componente BLUE", ImgBLUE);
    
    cvNamedWindow("Componente GREEN", CV_WINDOW_AUTOSIZE);
    cvShowImage("Componente GREEN", ImgGREEN);
    
    cvNamedWindow("Componente RED", CV_WINDOW_AUTOSIZE);
    cvShowImage("Componente RED", ImgRED);

    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&Img);
    cvReleaseImage(&ImgBLUE);
    cvReleaseImage(&ImgGREEN);
    cvReleaseImage(&ImgRED);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Componente BLUE");
    cvDestroyWindow("Componente GREEN");
    cvDestroyWindow("Componente RED");

    return EXIT_SUCCESS;
}

