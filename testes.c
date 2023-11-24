#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "instrumentation.h"
#include "image8bit.h"
int main(void)
{
    printf("(ImageBlur: 1ª função(tempo de execução maior/mais lenta)\n");
    Image img = ImageLoad("pgm/small/art3_222x217.pgm");
    double start_time = cpu_time();
    ImageBlur(img, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))
    double finish_time = cpu_time();
    double exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem pequena): %f segundos\n", exec_time);
//

//
// fazer para as outras funcoes e para diferentes tamanhos

    Image img1 = ImageLoad("pgm/medium/airfield-05_640x480.pgm");
    start_time = cpu_time();
    ImageBlur(img1, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem média): %f segundos\n", exec_time);
//
    Image img3 = ImageLoad("pgm/large/airfield-05_1600x1200.pgm");
    start_time = cpu_time();
    ImageBlur(img3, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem larga): %f segundos\n", exec_time);
//

    printf("\n(ImageBlur: 1ª função(tempo de execução maior/mais lenta) com dimensões máximas da imagem\n");
    img = ImageLoad("pgm/small/art3_222x217.pgm");
    start_time = cpu_time();
    ImageBlur(img, 222, 217); //  valores só para 1 pixel(640,480(neste caso da img2))
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem pequena): %f segundos\n", exec_time);
//

//
// fazer para as outras funcoes e para diferentes tamanhos
/*
    img1 = ImageLoad("pgm/medium/airfield-05_640x480.pgm");
    start_time = cpu_time();
    ImageBlur(img1, 640, 480); //  valores só para 1 pixel(640,480(neste caso da img2))
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem média): %f segundos\n", exec_time);
//
    img3 = ImageLoad("pgm/large/airfield-05_1600x1200.pgm");
    start_time = cpu_time();
    ImageBlur(img3, 1600, 1200); //  valores só para 1 pixel(640,480(neste caso da img2))
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem larga): %f segundos\n", exec_time);
//
    
estas demorarm muito tempo por isso é que comentei
*/

    printf("\nImageLocateSubImage: \n");

    Image img2 = ImageLoad("pgm/small/art3_222x217.pgm");
    start_time = cpu_time();
    int px,py;
    ImageLocateSubImage(img1, &px, &py,img2);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução : %f segundos\n", exec_time);

    img2 = ImageLoad("pgm/medium/ireland-03_640x480.pgm");
    start_time = cpu_time();
    ImageLocateSubImage(img1, &px, &py,img2);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem média): %f segundos\n", exec_time);
//
    img2 = ImageLoad("pgm/large/airfield-05_1600x1200.pgm");
    start_time = cpu_time();
    ImageLocateSubImage(img1, &px, &py,img2);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem grande): %f segundos\n", exec_time);
//
    


    printf("\n(ImageBlurv2: 2ª função(tempo de execução menor/mais rápida))\n");
    img = ImageLoad("pgm/small/art3_222x217.pgm");
    start_time = cpu_time();
    ImageBlurv2(img, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem pequena): %f segundos\n", exec_time);
//

//

    img1 = ImageLoad("pgm/medium/airfield-05_640x480.pgm");
    start_time = cpu_time();
    ImageBlurv2(img1, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem média): %f segundos\n", exec_time);
//
    img3 = ImageLoad("pgm/large/airfield-05_1600x1200.pgm");
    start_time = cpu_time();
    ImageBlurv2(img3, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem larga): %f segundos\n", exec_time);
//


    printf("\n(ImageBlurv2: 2ª função(tempo de execução menor/mais rápida) com dimensões máximas da imagem\n");
    img = ImageLoad("pgm/small/art3_222x217.pgm");
    start_time = cpu_time();
    ImageBlurv2(img, 222, 217); //  valores só para 1 pixel(640,480(neste caso da img2))
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem pequena): %f segundos\n", exec_time);

    img = ImageLoad("pgm/medium/airfield-05_640x480.pgm");
    start_time = cpu_time();
    ImageBlurv2(img, 640, 480); //  valores só para 1 pixel(640,480(neste caso da img2))
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem média): %f segundos\n", exec_time);

    img = ImageLoad("pgm/large/airfield-05_1600x1200.pgm");
    start_time = cpu_time();
    ImageBlurv2(img, 1600, 1200); //  valores só para 1 pixel(640,480(neste caso da img2))
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem grande): %f segundos\n", exec_time);
    




   


    return 0;
}








