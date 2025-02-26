#ifndef FRAME_TRANSPORT_H
#define FRAME_TRANSPORT_H

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
} config_transport;

typedef config_transport RGB_transport;

typedef config_transport Matriz_config[5][5];

// Função para definição da intensidade de cores do led
extern uint32_t matrix_rgb(double b, double r, double g);

// Acionar a matrix de leds - ws2812b
void led_control_transport(PIO pio, uint sm, uint32_t cor)
{
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, cor);
    }
}

void print_transport(Matriz_config configuracao, PIO pio, uint sm){
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

RGB_transport RGB_parameter_transport(int red, int green, int blue){
    RGB_transport cor_customizada = {red/255.0, green/255.0, blue/255.0};
    return cor_customizada;
}

void frame_transport(PIO pio, uint sm,float itnsty) {
    Matriz_config transport_frames[] = {
        //frame 1 - boat 
        {
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
            {{1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
        },

        // frame 2 - hot air balloon 
        {
            {{1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 0.0, 0.0}},
            {{1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.0}, {0.6*itnsty, 0.3*itnsty, 0.0}, {0.0, 0.0, 0.0}, {0.6*itnsty, 0.3*itnsty, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {0.6*itnsty, 0.3*itnsty, 0.0}, {0.6*itnsty, 0.3*itnsty, 0.0}, {0.6*itnsty, 0.3*itnsty, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
        },

        // frame 3 - bus
        {
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
            {{1.0*itnsty, 1.0*itnsty, 0.0}, {0.6*itnsty, 0.3*itnsty, 0.0}, {0.0, 1.0*itnsty, 1.0*itnsty}, {0.6*itnsty, 0.3*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}}, // Linha 1
            {{1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 0.5*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}}, // Linha 2
            {{1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 1.0*itnsty}, {1.0*itnsty, 1.0*itnsty, 1.0*itnsty}, {1.0*itnsty, 1.0*itnsty, 1.0*itnsty}, {0.0, 0.0, 0.0}}  // Linha 4
        },

        // frame 4 -truck
        {
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}}, // Linha 1
            {{0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 1.0*itnsty, 0.0}},
            {{0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {1.0*itnsty, 0.0, 0.0}, {1.0*itnsty, 0.0, 0.0}}, // Linha 3
            {{0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}}  // Linha 4
        },

        // frame 5 - skate
        {
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
            {{0.0, 0.0, 0.5*itnsty}, {0.0, 0.0, 0.5*itnsty}, {0.0, 0.0, 0.5*itnsty}, {0.0, 0.0, 0.5*itnsty}, {0.0, 0.0, 0.5*itnsty}}, // Linha 2
            {{0.0, 0.0, 0.0}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.0, 0.0, 0.0}, {0.5*itnsty, 0.5*itnsty, 0.5*itnsty}, {0.0, 0.0, 0.0}}, // Linha 3
            {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
        }
    };

    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 10; i++) {
            print_transport(transport_frames[i], pio, sm);
            sleep_ms(4000); 
        }
    }
}

#endif
