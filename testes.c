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
    Image img1 = ImageLoad("pgm/medium/airfield-05_640x480.pgm");

    double start_time = cpu_time();

    ImageBlur(img1, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))

    double finish_time = cpu_time();

    double exec_time = finish_time - start_time;

    printf("Exec time (ImageBlur(1função)): %f\n", exec_time);

    Image img2 = ImageLoad("pgm/medium/ireland-03_640x480.pgm");
    int px,py;
    start_time = cpu_time();
    ImageLocateSubImage(img1, &px, &py,img2);
    finish_time = cpu_time();

    exec_time = finish_time - start_time;

    printf("Exec time (ImageLocateSubImage): %f\n", exec_time);

    start_time = cpu_time();
    ImageBlurv2(img1, 7, 7);
    finish_time = cpu_time();

    exec_time = finish_time - start_time;

    printf("Exec time (ImageBlur v2): %f\n", exec_time);

    return 0;
}