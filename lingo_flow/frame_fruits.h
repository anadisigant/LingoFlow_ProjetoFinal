#ifndef FRAME_FRUITS_H
#define FRAME_FRUITS_H

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
} config_fruits;

typedef config_fruits RGB_fruits;

typedef config_fruits Matriz_config_fruits[5][5];

// Função para definição da intensidade de cores do led
extern uint32_t matrix_rgb(double b, double r, double g);

// Acionar a matrix de leds - ws2812b
void led_control_fruits(PIO pio, uint sm, uint32_t cor)
{
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, cor);
    }
}

void print_fruits(Matriz_config configuracao, PIO pio, uint sm){
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

RGB_fruits RGB_parameter_fruits(int red, int green, int blue){
    RGB_fruits cor_customizada = {red/255.0, green/255.0, blue/255.0};
    return cor_customizada;
}

void frame_fruits(PIO pio, uint sm,float itnsty) {
    Matriz_config fruits_frames[] = {
     // Frame 1 - apple
    {
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5*itnsty, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}
    },
    
    // Frame 2 - orange
    {
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5*itnsty, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}
    },
    
    // Frame 3 - grape
    {
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5*itnsty, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0}, {0.5*itnsty, 0.0, 0.5*itnsty}, {0.5*itnsty, 0.0, 0.5*itnsty}, {0.5*itnsty, 0.0, 0.5*itnsty}, {0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0}, {0.5*itnsty, 0.0, 0.5*itnsty}, {0.5*itnsty, 0.0, 0.5*itnsty}, {0.5*itnsty, 0.0, 0.5*itnsty}, {0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5*itnsty, 0.0, 0.5*itnsty}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}
    },
    // Frame 4 - Banana
    {
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.35*itnsty, 0.16*itnsty, 0.14*itnsty}}, 
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}}, 
    {{1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, 
    {{0.0, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}} 
    },

    // Frame 5 - Abacaxi
    {
    {{0.0, 0.0, 0.0}, {0.0, 1.0*itnsty, 0.0}, {0.0, 1.0*itnsty, 0.0}, {0.0, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}}, 
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, 
    {{0.0, 0.0, 0.0}, {0.35*itnsty, 0.16*itnsty, 0.14*itnsty}, {0.35*itnsty, 0.16*itnsty, 0.14*itnsty}, {0.35*itnsty, 0.16*itnsty, 0.14*itnsty}, {0.0, 0.0, 0.0}}, 
    {{0.0, 0.0, 0.0}, {0.35*itnsty, 0.16*itnsty, 0.14*itnsty}, {0.35*itnsty, 0.16*itnsty, 0.14*itnsty}, {0.35*itnsty, 0.16*itnsty, 0.14*itnsty}, {0.0, 0.0, 0.0}}, 
    {{0.0, 0.0, 0.0}, {0.35*itnsty, 0.16*itnsty, 0.14*itnsty}, {0.35*itnsty, 0.16*itnsty, 0.14*itnsty}, {0.35*itnsty, 0.16*itnsty, 0.14*itnsty}, {0.0, 0.0, 0.0}} 
    }
    };

    for (int i = 0; i < 5; i++) {
        print_fruits(fruits_frames[i], pio, sm);
        sleep_ms(4000); 
    }
   
}

#endif
