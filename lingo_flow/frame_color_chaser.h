#ifndef FRAME_COLOR_CHASER_H
#define FRAME_COLOR_CHASER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include <string.h>
#include "matrix_utils.h"

//arquivo .pio
#include "pio_matrix.pio.h"

// Definição de tipo da estrutura que irá controlar a cor dos LED's
typedef struct {
    double red;
    double green;
    double blue;
}Led_config_6;

// Definição de tipo da matriz de leds
typedef Led_config_6 Matriz_leds_config_6[5][5];

Matriz_leds_config_6 default_frame_6 = {
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
};

//função para definição da intensidade de cores do led
extern uint32_t matrix_rgb(double b, double r, double g);

void chaser_printer(Matriz_leds_config_6 configuracao, PIO pio, uint sm){
    for (int contadorLinha = 4; contadorLinha >= 0; contadorLinha--){
        if(contadorLinha % 2){
            for (int contadorColuna = 0; contadorColuna < 5; contadorColuna ++){
                uint32_t valor_cor_binario = matrix_rgb(
                    configuracao[contadorLinha][contadorColuna].blue,
                    configuracao[contadorLinha][contadorColuna].red,
                    configuracao[contadorLinha][contadorColuna].green
                );

                pio_sm_put_blocking(pio, sm, valor_cor_binario);
            }
        }else{
            for (int contadorColuna = 4; contadorColuna >= 0; contadorColuna --){
                uint32_t valor_cor_binario = matrix_rgb(
                    configuracao[contadorLinha][contadorColuna].blue,
                    configuracao[contadorLinha][contadorColuna].red,
                    configuracao[contadorLinha][contadorColuna].green
                );

                pio_sm_put_blocking(pio, sm, valor_cor_binario);
            }
        }
    }
}

void configurar_matriz_6(Matriz_leds_config_6* frame,float red, float blue, float green, int row, int col) {
    (*frame)[row][col].red = red;
    (*frame)[row][col].blue = blue;
    (*frame)[row][col].green = green;
}

void chaser_frame(Matriz_leds_config_6* frame,Matriz_leds_config_6* base_frame,Matriz_leds_config_6* frames,float r[5], float b[5], float g[5], int row[5], int col[5], float itnsty) {
    memcpy(*frame, *base_frame, sizeof(Matriz_leds_config_6));
    for(int i = 0; i < 5; i++) {
        configurar_matriz_6(frame,r[i]*itnsty,b[i]*itnsty,g[i]*itnsty,row[i],col[i]);
        memcpy(frames, frame, sizeof(Matriz_leds_config_6));    
    }
}

void chaser_animation(PIO pio, uint sm, bool isActive) {
    float itnsty = isActive ? 1 : 0.1;
    
    Matriz_leds_config_6 frame, base_frame;
    Matriz_leds_config_6 frames[24*3+4];

    memcpy(base_frame, default_frame_6, sizeof(Matriz_leds_config_6));

    int count = 0;

    for(int i = 0;i < 3;i++) {
        chaser_frame(&frame,&base_frame,&frames[0+count],(float []){0.0625,0.125,0.25,0.5,1},(float []){0,0,0,0,0},(float []){0.0625,0.125,0.25,0.5,0},(int []){0,0,0,1,2},(int []){2,1,0,0,0},itnsty);

        base_frame[2][2].blue = 1*itnsty;

        chaser_frame(&frame,&base_frame,&frames[1+count],(float []){0.0625,0.125,0.25,0.5,1},(float []){0,0,0,0,0},(float []){0.0625,0.125,0.25,0,0},(int []){0,0,1,2,3},(int []){1,0,0,0,0},itnsty);
        chaser_frame(&frame,&base_frame,&frames[2+count],(float []){0.0625,0.125,0.25,0.5,1},(float []){0,0,0,0,0},(float []){0.0625,0.125,0,0,0},(int []){0,1,2,3,4},(int []){0,0,0,0,0},itnsty);
        chaser_frame(&frame,&base_frame,&frames[3+count],(float []){0.0625,0.125,0.25,0.5,1},(float []){0,0,0,0,0},(float []){0.0625,0,0,0,0},(int []){1,2,3,4,4},(int []){0,0,0,0,1},itnsty);
        chaser_frame(&frame,&base_frame,&frames[4+count],(float []){0.0625,0.125,0.25,0.5,1},(float []){0,0,0,0,0},(float []){0,0,0,0,0},(int []){2,3,4,4,4},(int []){0,0,0,1,2},itnsty);
        chaser_frame(&frame,&base_frame,&frames[5+count],(float []){0.0625,0.125,0.25,0.5,1},(float []){0,0,0,0,0},(float []){0,0,0,0,0},(int []){3,4,4,4,3},(int []){0,0,1,2,2},itnsty);
        chaser_frame(&frame,&base_frame,&frames[6+count],(float []){0.0625,0.125,0.25,0.5,0},(float []){0,0,0,0,1},(float []){0,0,0,0,0},(int []){4,4,4,3,2},(int []){0,1,2,2,2},itnsty);

        base_frame[2][2].blue = 0;
        base_frame[2][4].green = 1*itnsty;

        chaser_frame(&frame,&base_frame,&frames[7+count],(float []){0.0625,0.125,0.25,0,0},(float []){0,0,0,0.5,1},(float []){0,0,0,0,0},(int []){4,4,3,2,1},(int []){1,2,2,2,2},itnsty);
        chaser_frame(&frame,&base_frame,&frames[8+count],(float []){0.0625,0.125,0,0,0},(float []){0,0,0.25,0.5,1},(float []){0,0,0,0,0},(int []){4,3,2,1,0},(int []){2,2,2,2,2},itnsty);
        chaser_frame(&frame,&base_frame,&frames[9+count],(float []){0.0625,0,0,0,0},(float []){0,0.125,0.25,0.5,1},(float []){0,0,0,0,0},(int []){3,2,1,0,0},(int []){2,2,2,2,3},itnsty);
        chaser_frame(&frame,&base_frame,&frames[10+count],(float []){0,0,0,0,0},(float []){0.0625,0.125,0.25,0.5,1},(float []){0,0,0,0,0},(int []){2,1,0,0,0},(int []){2,2,2,3,4},itnsty);
        chaser_frame(&frame,&base_frame,&frames[11+count],(float []){0,0,0,0,0},(float []){0.0625,0.125,0.25,0.5,1},(float []){0,0,0,0,0},(int []){1,0,0,0,1},(int []){2,2,3,4,4},itnsty);
        chaser_frame(&frame,&base_frame,&frames[12+count],(float []){0,0,0,0,0},(float []){0.0625,0.125,0.25,0.5,0},(float []){0,0,0,0,1},(int []){0,0,0,1,2},(int []){2,3,4,4,4},itnsty);

        base_frame[2][4].green = 0;
        base_frame[2][2].green = 1*itnsty;
        base_frame[2][2].red = 1*itnsty;

        chaser_frame(&frame,&base_frame,&frames[13+count],(float []){0,0,0,0,0},(float []){0.0625,0.125,0.25,0,0},(float []){0,0,0,0.5,1},(int []){0,0,1,2,3},(int []){3,4,4,4,4},itnsty);
        chaser_frame(&frame,&base_frame,&frames[14+count],(float []){0,0,0,0,0},(float []){0.0625,0.125,0,0,0},(float []){0,0,0.25,0.5,1},(int []){0,1,2,3,4},(int []){4,4,4,4,4},itnsty);
        chaser_frame(&frame,&base_frame,&frames[15+count],(float []){0,0,0,0,0},(float []){0.0625,0,0,0,0},(float []){0,0.125,0.25,0.5,1},(int []){1,2,3,4,4},(int []){4,4,4,4,3},itnsty);
        chaser_frame(&frame,&base_frame,&frames[16+count],(float []){0,0,0,0,0},(float []){0,0,0,0,0,0},(float []){0.0625,0.125,0.25,0.5,1},(int []){2,3,4,4,4},(int []){4,4,4,3,2},itnsty);
        chaser_frame(&frame,&base_frame,&frames[17+count],(float []){0,0,0,0,0},(float []){0,0,0,0,0,0},(float []){0.0625,0.125,0.25,0.5,1},(int []){3,4,4,4,3},(int []){4,4,3,2,2},itnsty);
        chaser_frame(&frame,&base_frame,&frames[18+count],(float []){0,0,0,0,1},(float []){0,0,0,0,0,0},(float []){0.0625,0.125,0.25,0.5,1},(int []){4,4,4,3,2},(int []){4,3,2,2,2},itnsty);

        base_frame[2][2].red = 0;
        base_frame[2][2].green = 0;
        base_frame[2][0].red = 1*itnsty;

        chaser_frame(&frame,&base_frame,&frames[19+count],(float []){0,0,0,0.5,1},(float []){0,0,0,0,0,0},(float []){0.0625,0.125,0.25,0.5,1},(int []){4,4,3,2,1},(int []){3,2,2,2,2},itnsty);
        chaser_frame(&frame,&base_frame,&frames[20+count],(float []){0,0,0.25,0.5,1},(float []){0,0,0,0,0,0},(float []){0.0625,0.125,0.25,0.5,1},(int []){4,3,2,1,0},(int []){2,2,2,2,2},itnsty);
        chaser_frame(&frame,&base_frame,&frames[21+count],(float []){0,0.125,0.25,0.5,1},(float []){0,0,0,0,0,0},(float []){0.0625,0.125,0.25,0.5,1},(int []){3,2,1,0,0},(int []){2,2,2,2,1},itnsty);
        chaser_frame(&frame,&base_frame,&frames[22+count],(float []){0.0625,0.125,0.25,0.5,1},(float []){0,0,0,0,0,0},(float []){0.0625,0.125,0.25,0.5,1},(int []){2,1,0,0,0},(int []){2,2,2,1,0},itnsty);
        chaser_frame(&frame,&base_frame,&frames[23+count],(float []){0.0625,0.125,0.25,0.5,1},(float []){0,0,0,0,0,0},(float []){0.0625,0.125,0.25,0.5,1},(int []){1,0,0,0,1},(int []){2,2,1,0,0},itnsty);
        
        base_frame[2][0].red = 0;

        count = count + 24;
    }

    chaser_frame(&frame,&base_frame,&frames[72],(float []){0.0,0.0625,0.125,0.25,0.5},(float []){0,0,0,0,0},(float []){0.0,0.0625,0.125,0.25,0},(int []){0,0,0,1,2},(int []){2,1,0,0,0},itnsty);
    chaser_frame(&frame,&base_frame,&frames[73],(float []){0.0,0.0,0.0625,0.125,0.25},(float []){0,0,0,0,0},(float []){0.0,0.0,0.0625,0.125,0},(int []){0,0,0,1,2},(int []){2,1,0,0,0},itnsty);
    chaser_frame(&frame,&base_frame,&frames[74],(float []){0.0,0.0,0.0,0.0625,0.125},(float []){0,0,0,0,0},(float []){0.0,0.0,0.0,0.0625,0},(int []){0,0,0,1,2},(int []){2,1,0,0,0},itnsty);
    chaser_frame(&frame,&base_frame,&frames[75],(float []){0.0,0.0,0.0,0.0,0.0},(float []){0,0,0,0,0},(float []){0.0,0.0,0.0,0.0,0},(int []){0,0,0,1,2},(int []){2,1,0,0,0},itnsty);

    for (int i = 0; i < 24*3+4; i++) {
        chaser_printer(frames[i], pio, sm);
        sleep_ms(100); 
    }
}

#endif