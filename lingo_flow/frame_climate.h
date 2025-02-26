#ifndef FRAME_CLIMATE_H
#define FRAME_CLIMATE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "matrix_utils.h"

#define NUM_PIXELS 25

// Definição de tipo da estrutura que irá controlar a cor dos LED's
typedef struct {
    double red;
    double green;
    double blue;
} config_climate;

typedef config_climate RGB_climate;

typedef config_climate Matriz_config_climate[5][5];

// Função para definição da intensidade de cores do led
extern uint32_t matrix_rgb(double b, double r, double g);

// Acionar a matrix de leds - ws2812b
void led_control_climate(PIO pio, uint sm, uint32_t cor)
{
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, cor);
    }
}

void print_climate(Matriz_config configuracao, PIO pio, uint sm){
    for (int contadorLinha = 4; contadorLinha >= 0; contadorLinha--){
        if(contadorLinha % 2){
            for (int contadorColuna = 0; contadorColuna < 5; contadorColuna++){
                uint32_t valor_cor_binario = matrix_rgb(
                    configuracao[contadorLinha][contadorColuna].blue,
                    configuracao[contadorLinha][contadorColuna].red,
                    configuracao[contadorLinha][contadorColuna].green
                );
                pio_sm_put_blocking(pio, sm, valor_cor_binario);
            }
        } else {
            for (int contadorColuna = 4; contadorColuna >= 0; contadorColuna--){
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

RGB_climate RGB_parameter_climate(int red, int green, int blue){
    RGB_climate cor_customizada = {red/255.0, green/255.0, blue/255.0};
    return cor_customizada;
}

void frame_climate(PIO pio, uint sm,float itnsty) {
    Matriz_config climate_frames[] = {
        //frame 1 - rainy
        {
            {{0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}}, 
            {{0.0, 0.0, 0.0}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}}, 
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}}, 
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0*itnsty}, {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0*itnsty}, {0.0, 0.0, 0.0}}, 
            {{0.0, 0.0, 1.0*itnsty}, {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0*itnsty}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}
        },

        // frame 2 - sunny
        {
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}}, 
            {{1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {0.0, 0.0, 0.0}}, 
            {{1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}}, 
            {{0.0, 0.0, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}}, 
            {{0.0, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}} 
        },

        // frame 3 - cloudy
        {
            {{0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {1.0*itnsty, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}}, 
            {{0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {0.0, 0.0, 0.0}}, 
            {{1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}}, 
            {{0.0, 0.0, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}}, 
            {{0.0, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}}  
        },

        // frame 4 - stormy
        {
            {{0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}}, 
            {{0.0, 0.0, 0.0}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}}, 
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}}, 
            {{0.0, 0.0, 1.0*itnsty}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}}, 
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}  
        },

        // frame 5 - clear
        {
            {{0.5*itnsty, 0.5*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}}, 
            {{0.5*itnsty, 0.5*itnsty, 1.0*itnsty}, {1.0*itnsty, 1.0*itnsty, 1.0*itnsty}, {1.0*itnsty, 1.0*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}}, 
            {{1.0*itnsty, 1.0*itnsty, 1.0*itnsty}, {1.0*itnsty, 1.0*itnsty, 1.0*itnsty}, {1.0*itnsty, 1.0*itnsty, 1.0*itnsty}, {1.0*itnsty, 1.0*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}}, 
            {{1.0*itnsty, 1.0*itnsty, 1.0*itnsty}, {1.0*itnsty, 1.0*itnsty, 1.0*itnsty}, {1.0*itnsty, 1.0*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}}, 
            {{0.5*itnsty, 0.5*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}, {0.5*itnsty, 0.5*itnsty, 1.0*itnsty}}  
        }

    };

    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 10; i++) {
            print_climate(climate_frames[i], pio, sm);
            sleep_ms(4000); 
        }
    }
}

#endif
