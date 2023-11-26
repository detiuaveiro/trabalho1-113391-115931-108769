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
    MULTIPLICACOES = 0;
    SOMAS = 0;
    printf("(ImageBlur: 1ª função(tempo de execução maior/mais lenta)\n");
    Image img = ImageLoad("pgm/small/art3_222x217.pgm");
    double start_time = cpu_time();
    ImageBlur(img, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))
    printf("Multiplicações: %ld\n",MULTIPLICACOES);
    printf("Somas: %ld\n",SOMAS);
    double finish_time = cpu_time();
    double exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem pequena): %f segundos\n", exec_time);
    
//

//
// fazer para as outras funcoes e para diferentes tamanhos
    MULTIPLICACOES = 0;
    SOMAS = 0;
    Image img1 = ImageLoad("pgm/medium/airfield-05_640x480.pgm");
    start_time = cpu_time();
    ImageBlur(img1, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))
    printf("Multiplicações: %ld\n",MULTIPLICACOES);
    printf("Somas: %ld\n",SOMAS);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem média): %f segundos\n", exec_time);
//
    MULTIPLICACOES = 0;
    SOMAS = 0;
    Image img3 = ImageLoad("pgm/large/airfield-05_1600x1200.pgm");
    start_time = cpu_time();
    ImageBlur(img3, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))
    printf("Multiplicações: %ld\n",MULTIPLICACOES);
    printf("Somas: %ld\n",SOMAS);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem larga): %f segundos\n", exec_time);
//

    MULTIPLICACOES = 0;
    SOMAS = 0;
    printf("\n(ImageBlur: 1ª função(tempo de execução maior/mais lenta) com dimensões máximas da imagem\n");
    img = ImageLoad("pgm/small/art3_222x217.pgm");
    start_time = cpu_time();
    ImageBlur(img, 222, 217); //  valores só para 1 pixel(640,480(neste caso da img2))
    printf("Multiplicações: %ld\n",MULTIPLICACOES);
    printf("Somas: %ld\n",SOMAS);
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

    Image small = ImageLoad("pgm/small/art3_222x217.pgm");
    Image small2 = ImageLoad("pgm/small/art4_300x300.pgm");
    Image medium = ImageLoad("pgm/medium/ireland-03_640x480.pgm");
    Image medium2 = ImageLoad("pgm/medium/tac-pulmao_512x512.pgm");
    Image large = ImageLoad("pgm/large/airfield-05_1600x1200.pgm");
    Image large2 = ImageLoad("pgm/large/einstein_940x940.pgm");

  
    ImagePaste(medium,0,0,small);
    start_time = cpu_time();
    int px,py;
    COMP = 0;
    //ImageLocateSubImage(small, &px, &py,medium);
    printf("\n%d\n",ImageLocateSubImage(medium, &px, &py,small));
    printf("Comparaçoes: %ld\n",COMP);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução(Pixels de uma média para uma pequena)  : %f segundos\n", exec_time);


    //Se não dermos paste, e tentarmos com outra imagem média, o tempo de execução
// será maior e não irá localizar a subimagem.

    start_time = cpu_time();
    //ImageLocateSubImage(medium2, &px, &py,small);
    COMP = 0;
    printf("\n%d\n",ImageLocateSubImage(medium2, &px, &py,small));
    printf("Comparações: %ld\n",COMP);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução(Pixels de uma média diferente para uma pequena), mas desta vez não encontrará a subimagem  : %f segundos\n", exec_time);


    ImagePaste(large,0,0,small);
    COMP = 0;
    start_time = cpu_time();
    //ImageLocateSubImage(medium2, &px, &py,small);
   
    printf("\n%d\n",ImageLocateSubImage(large, &px, &py,small));
    printf("Comparaçoes: %ld\n",COMP);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução(Pixels de uma larga para uma pequena) : %f segundos\n", exec_time);

    start_time = cpu_time();
    COMP = 0;
    //ImageLocateSubImage(medium2, &px, &py,small);
    printf("\n%d\n",ImageLocateSubImage(large, &px, &py,medium));
    printf("Comparaçoes: %ld\n",COMP);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução(Pixels de uma larga para uma média) : %f segundos\n\n", exec_time);

    ImagePaste(small2,0,0,small);
    start_time = cpu_time();
    //ImageLocateSubImage(medium2, &px, &py,small);
    COMP = 0;
    printf("\n%d\n",ImageLocateSubImage(small2, &px, &py,small));
    printf("Comparaçoes: %ld\n",COMP);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução(Pixels de uma pequena para outra pequena) : %f segundos\n", exec_time);


    //não dá para fazer ImageLocateSubImage com duas inagens médias, dado que a largura de uma imagem é 
    //maior que a outra e a altura é menor, ou vice-versa.

    ImagePaste(large,50,50,large2);
    start_time = cpu_time();
    //ImageLocateSubImage(medium2, &px, &py,small);
    COMP = 0;
    printf("\n%d\n",ImageLocateSubImage(large, &px, &py,large2));
    printf("Comparaçoes: %ld\n",COMP);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução(Pixels de uma grande para outra grande) : %f segundos\n", exec_time);

    MULTIPLICACOES = 0;
    SOMAS = 0;
    printf("\n(ImageBlurv2: 2ª função(tempo de execução menor/mais rápida))\n");
    img = ImageLoad("pgm/small/art3_222x217.pgm");
    start_time = cpu_time();
    ImageBlurv2(img, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("\nTempo de execução (imagem pequena): %f segundos\n", exec_time);
    printf("Multiplicações: %ld\n",MULTIPLICACOES);
    printf("Somas: %ld\n",SOMAS);
//

//
    MULTIPLICACOES = 0;
    SOMAS = 0;
    img1 = ImageLoad("pgm/medium/airfield-05_640x480.pgm");
    start_time = cpu_time();
    ImageBlurv2(img1, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))
    printf("Multiplicações: %ld\n",MULTIPLICACOES);
    printf("Somas: %ld\n",SOMAS);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem média): %f segundos\n", exec_time);
//

    MULTIPLICACOES = 0;
    SOMAS = 0;
    img3 = ImageLoad("pgm/large/airfield-05_1600x1200.pgm");
    start_time = cpu_time();
    ImageBlurv2(img3, 7, 7); //  valores só para 1 pixel(640,480(neste caso da img2))
    printf("Multiplicações: %ld\n",MULTIPLICACOES);
    printf("Somas: %ld\n",SOMAS);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem larga): %f segundos\n", exec_time);
//

    MULTIPLICACOES = 0;
    SOMAS = 0;
    printf("\n(ImageBlurv2: 2ª função(tempo de execução menor/mais rápida) com dimensões máximas da imagem\n");
    img = ImageLoad("pgm/small/art3_222x217.pgm");
    start_time = cpu_time();
    ImageBlurv2(img, 222, 217); //  valores só para 1 pixel(640,480(neste caso da img2))
    printf("Multiplicações: %ld\n",MULTIPLICACOES);
    printf("Somas: %ld\n",SOMAS);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem pequena): %f segundos\n", exec_time);



    MULTIPLICACOES = 0;
    SOMAS = 0;
    img = ImageLoad("pgm/medium/airfield-05_640x480.pgm");
    start_time = cpu_time();
    ImageBlurv2(img, 640, 480); //  valores só para 1 pixel(640,480(neste caso da img2))
    printf("Multiplicações: %ld\n",MULTIPLICACOES);
    printf("Somas: %ld\n",SOMAS);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem média): %f segundos\n", exec_time);


    MULTIPLICACOES = 0;
    SOMAS = 0;
    img = ImageLoad("pgm/large/airfield-05_1600x1200.pgm");
    start_time = cpu_time();
    ImageBlurv2(img, 1600, 1200); //  valores só para 1 pixel(640,480(neste caso da img2))
    printf("Multiplicações: %ld\n",MULTIPLICACOES);
    printf("Somas: %ld\n",SOMAS);
    finish_time = cpu_time();
    exec_time = finish_time - start_time;
    printf("Tempo de execução (imagem grande): %f segundos\n", exec_time);
    




   


    return 0;
}








