#ifndef FRAME_NUMBERS_H
#define FRAME_NUMBERS_H

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

// Definição de tipo da estrutura que irá controlar a cor dos LED's
typedef struct {
    double red;
    double green;
    double blue;
}Led_config_4;

typedef Led_config_4 RGB_cod_4;

// Definição de tipo da matriz de leds
typedef Led_config_4 Matriz_leds_config_4[5][5];

Matriz_leds_config_4 default_frame_2 = {
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
};


//função para definição da intensidade de cores do led
extern uint32_t matrix_rgb(double b, double r, double g);


void print_number(Matriz_leds_config_4 configuracao, PIO pio, uint sm){
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

void configurar_matriz_2(Matriz_leds_config_4* frame,float r, float g, float b, int row, int col) {
    (*frame)[row][col].red = r;
    (*frame)[row][col].blue = b;
    (*frame)[row][col].green = g;
}

void framer_2(Matriz_leds_config_4* frame,Matriz_leds_config_4* base_frame,float r, float g, float b, int row, int col, bool clear,PIO pio,uint sm,bool print,int sleep,float itnsty) {
    if(clear) memcpy(*frame, *base_frame, sizeof(Matriz_leds_config_4));
    configurar_matriz_2(frame,r*itnsty,g*itnsty,b*itnsty,row,col);
    if(print) {
        print_number(*frame, pio, sm);
        sleep_ms(sleep);    
    }
}

void frame_numbers(int number, PIO pio, uint sm, float itnsty) {
    Matriz_leds_config_4 frame, base_frame;

    memcpy(base_frame, default_frame_2, sizeof(Matriz_leds_config_4));
    bool g1 = 0, g2 = 1;

    if(number == 1) {
        for(int i = 0; i < 8; i++) {
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,1,1,true,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,1,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,3,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,4,false,pio,sm,true,250,itnsty);
            g1 = !g1;
            g2 = !g2;
        }
    } else if(number == 2) {
        for(int i = 0; i < 8; i++) {
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,0,true,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,1,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,3,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,4,false,pio,sm,true,250,itnsty);

            g1 = !g1;
            g2 = !g2;
        }
    } else if(number == 3) {
        for(int i = 0; i < 8; i++) {
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,0,true,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,1,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,3,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,4,false,pio,sm,true,250,itnsty);

            g1 = !g1;
            g2 = !g2;
        }

    } else if(number == 4) {
        for(int i = 0; i < 2; i++) {
            framer_2(&frame,&base_frame,0,0,0.5,0,0,true,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,0,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,1,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,2,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,2,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,2,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,2,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,2,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,1,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,3,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,4,4,false,pio,sm,true,500,itnsty);

            framer_2(&frame,&base_frame,0,0,0.5,1,1,true,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,2,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,2,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,2,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,1,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,3,3,false,pio,sm,true,500,itnsty);
        }
    } else if(number == 5) {
        for(int i = 0; i < 8; i++) {
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,0,true,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,1,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,3,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,4,false,pio,sm,true,250,itnsty);

            g1 = !g1;
            g2 = !g2;
        }
    } else if(number == 6) {
        for(int i = 0; i < 8; i++) {
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,0,true,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,1,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,3,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,3,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,4,false,pio,sm,true,250,itnsty);

            g1 = !g1;
            g2 = !g2;
        }
    } else if(number == 7) {
        for(int i = 0; i < 8; i++) {
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,0,true,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,1,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,3,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,3,false,pio,sm,true,250,itnsty);

            g1 = !g1;
            g2 = !g2;
        }
    } else if(number == 8) {
        for(int i = 0; i < 8; i++) {
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,0,true,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,1,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,1,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,3,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,3,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,4,false,pio,sm,true,250,itnsty);

            g1 = !g1;
            g2 = !g2;
        }
    } else if(number == 9) {
        for(int i = 0; i < 8; i++) {
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,0,true,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,0,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,0,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,1,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,1,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,2,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,2,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,3,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g1-0.75,0.5,4,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,g2-0.75,0.5,4,4,false,pio,sm,true,250,itnsty);

            g1 = !g1;
            g2 = !g2;
        }
    } else if(number == 0) {
        for(int i = 0; i < 2; i++) {
            framer_2(&frame,&base_frame,0,0,0.5,0,0,true,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,0,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,0,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,0,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,0,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,1,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,1,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,2,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,2,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,3,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,3,4,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,4,0,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,4,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,4,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,4,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,4,4,false,pio,sm,true,500,itnsty);

            framer_2(&frame,&base_frame,0,0,0.5,1,1,true,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,1,2,false,pio,sm,false,0,itnsty);
             framer_2(&frame,&base_frame,0,0,0.5,1,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,2,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,2,3,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,3,1,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,3,2,false,pio,sm,false,0,itnsty);
            framer_2(&frame,&base_frame,0,0,0.5,3,3,false,pio,sm,true,500,itnsty);
        }
    } 

    base_frame[0][0].red = 0;
}

#endif