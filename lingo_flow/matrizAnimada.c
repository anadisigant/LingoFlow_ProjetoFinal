#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "hardware/pwm.h"
#include "frame_transport.h"
#include "frame_geometry.h"
#include "frame_fruits.h"
#include "frame_emotions.h"
#include "frame_climate.h"
#include "frame_numbers.h"
#include "frame_color_chaser.h"
#include "matrix_utils.h"
#include "pio_matrix.pio.h"

#define BUZZER_PIN 21
#define NUM_PIXELS 25
#define OUT_PIN 7

enum NotasMusicais {
    DO = 2640,
    RE = 2970,
    MI = 3300,
    FA = 3520,
    SOL = 3960,
    LA = 4400,
    SI = 4950
};

void set_buzzer_frequency(uint pin, uint frequency) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    gpio_set_function(pin, GPIO_FUNC_PWM);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (frequency * 2048));
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

void play_buzzer(uint pin, uint frequency, uint duration_ms) {
    set_buzzer_frequency(pin, frequency);
    pwm_set_gpio_level(pin, 32768);
    sleep_ms(duration_ms);
    pwm_set_gpio_level(pin, 0);
}

void controlar_todos_leds(PIO pio, uint sm, uint32_t cor) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, cor);
    }
}

int main() {
    PIO pio = pio0;
    bool ok;
    uint32_t cor;

    float itnsty = 0.5;

    char tecla = '\0';
    char buffer = '\0';

    char comando[10] = {0};
    int comando_index = 0;

    ok = set_sys_clock_khz(128000, false);

    stdio_init_all(); // Inicializa a comunicação serial (USB)
    uart_init(uart0, 115200);

    printf("WELCOME TO YOUR ENGLISH TEST!\n");
    printf("                             \n");
    printf("1 - CLIMATE;\n");
    printf("2 - EMOTIONS;\n");
    printf("3 - FRUITS;\n");
    printf("4 - GEOMETRY;\n");
    printf("5 - TRANSPORT;\n");
    printf("                            \n");
    printf("For the color test write the first letter of the colors!");
    printf("                            \n");
    printf("GO AHEAD AND GOOD LUCK!");

    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    bool system_busy = false;

    while (true) {
        if (!system_busy) {
            // Lê uma tecla do teclado do computador
            int input = getchar_timeout_us(0);
            if (input != PICO_ERROR_TIMEOUT) {
                tecla = (char)input; // Converte corretamente para char
                printf("Tecla %c pressionada!\n", tecla);
                system_busy = true;
            }
        }

        if (system_busy) {
            if (tecla == 'R' || tecla == 'r') {
                cor = matrix_rgb(0.0, 1 * itnsty, 0.0);
                controlar_todos_leds(pio, sm, cor);
                printf("THIS IS RED\n\n");
            } else if (tecla == 'B' || tecla == 'b') {
                cor = matrix_rgb(1 * itnsty, 0.0, 0.0);
                controlar_todos_leds(pio, sm, cor);
                printf("THIS IS BLUE\n\n");
            } else if (tecla == 'G' || tecla == 'g') {
                cor = matrix_rgb(0.0, 0.0, 1 * itnsty);
                controlar_todos_leds(pio, sm, cor);
                printf("THIS IS GREEN!\n\n");
            } else if (tecla == 'Y' || tecla == 'y') {
                cor = matrix_rgb(0.0, 1 * itnsty, 1 * itnsty);
                controlar_todos_leds(pio, sm, cor);
                printf("THIS IS YELLOW!\n\n");
            } else if (tecla == 'P' || tecla == 'p') {
                cor = matrix_rgb(1 * itnsty, 1 * itnsty, 0.0);
                controlar_todos_leds(pio, sm, cor);
                printf("THIS IS PINK!\n\n");
            } else if (tecla == 'W' || tecla == 'w') {
                cor = matrix_rgb(1 * itnsty, 1 * itnsty, 1 * itnsty);
                controlar_todos_leds(pio, sm, cor);
                printf("THIS IS WHITE!\n\n");
            } else if (tecla == 'C' || tecla == 'c') {
                cor = matrix_rgb(0.5 * itnsty, 0.0, 1 * itnsty);
                controlar_todos_leds(pio, sm, cor);
                printf("THIS IS CYAN!\n\n");
            } else if (tecla == 'V' || tecla == 'v') {
                cor = matrix_rgb(1 * itnsty, 0.5 * itnsty, 0.0);
                controlar_todos_leds(pio, sm, cor);
                printf("THIS IS VIOLET!\n\n");
            } else if (tecla == 'T') {
                cor = matrix_rgb(0.0, 0.0, 0.0);
                controlar_todos_leds(pio, sm, cor);
                printf("TURNING OFF THE LEDs\n\n");
            } else if (tecla == '1') {
                play_buzzer(BUZZER_PIN, MI, 200);
                frame_numbers(1, pio, sm, itnsty);
                frame_climate(pio, sm, itnsty);
            } else if (tecla == '2') {
                play_buzzer(BUZZER_PIN, FA, 200);
                frame_numbers(2, pio, sm, itnsty);
                frame_emotions(pio, sm, itnsty);
            } else if (tecla == '3') {
                play_buzzer(BUZZER_PIN, SOL, 200);
                frame_numbers(3, pio, sm, itnsty);
                frame_fruits(pio, sm, itnsty);
            } else if (tecla == '4') {
                play_buzzer(BUZZER_PIN, LA, 200);
                frame_numbers(4, pio, sm, itnsty);
                frame_geometry(pio, sm, itnsty);
            } else if (tecla == '5') {
                play_buzzer(BUZZER_PIN, SI, 200);
                frame_numbers(5, pio, sm, itnsty);
                frame_transport(pio, sm, itnsty);
            }

            system_busy = false;
            tecla = '\0';
        }

        sleep_ms(100);
    }
}