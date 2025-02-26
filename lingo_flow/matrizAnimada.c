#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // Para strcmp
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

#define BUZZER_PIN 21 // Define GPIO21
#define NUM_PIXELS 25
#define OUT_PIN 7

// Botões de interrupção
const uint button;

// Definindo pinos das linhas e colunas do teclado matricial
#define R1 9
#define R2 8
#define R3 10
#define R4 6
#define C1 5
#define C2 4
#define C3 3
#define C4 2

#define INTENSIDADE_ALTA 1.0
#define INTENSIDADE_MEDIA 0.8
#define INTENSIDADE_BAIXA 0.5
#define INTENSIDADE_MINIMA 0.2

// Função para inicializar pinos do teclado
void init_teclado() {
    gpio_init(R1);
    gpio_init(R2);
    gpio_init(R3);
    gpio_init(R4);
    gpio_init(C1);
    gpio_init(C2);
    gpio_init(C3);
    gpio_init(C4);
    gpio_set_dir(R1, GPIO_OUT);
    gpio_set_dir(R2, GPIO_OUT);
    gpio_set_dir(R3, GPIO_OUT);
    gpio_set_dir(R4, GPIO_OUT);
    gpio_set_dir(C1, GPIO_IN);
    gpio_set_dir(C2, GPIO_IN);
    gpio_set_dir(C3, GPIO_IN);
    gpio_set_dir(C4, GPIO_IN);
    gpio_pull_up(C1);
    gpio_pull_up(C2);
    gpio_pull_up(C3);
    gpio_pull_up(C4);
}

// Leitura do teclado
char keypad_leitura() {
    char teclado[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };
    int linhas[] = {R1, R2, R3, R4};
    int colunas[] = {C1, C2, C3, C4};

    for (int lin = 0; lin < 4; lin++) {
        gpio_put(linhas[lin], 0);
        for (int col = 0; col < 4; col++) {
            if (!gpio_get(colunas[col])) {
                sleep_ms(100);
                gpio_put(linhas[lin], 1);
                return teclado[lin][col];
            }
        }
        gpio_put(linhas[lin], 1);
    }
    return '\0';
}

// Frequências das notas musicais (em Hz)
enum NotasMusicais {
    DO = 2640, // Dó
    RE = 2970, // Ré
    MI = 3300, // Mi
    FA = 3520, // Fá
    SOL = 3960, // Sol
    LA = 4400, // Lá
    SI = 4950  // Si
};

// Configura o PWM no pino do buzzer com uma frequência especificada
void set_buzzer_frequency(uint pin, uint frequency) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    gpio_set_function(pin, GPIO_FUNC_PWM);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (frequency * 2048));
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

// Função para tocar o buzzer por um tempo especificado (em milissegundos)
void play_buzzer(uint pin, uint frequency, uint duration_ms) {
    set_buzzer_frequency(pin, frequency);
    pwm_set_gpio_level(pin, 32768);
    sleep_ms(duration_ms);
    pwm_set_gpio_level(pin, 0);
}

// Função para controlar todos os LEDs
void controlar_todos_leds(PIO pio, uint sm, uint32_t cor) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, cor); // Envia a cor para cada LED
    }
}

int main() {
    PIO pio = pio0;
    bool ok;
    uint32_t cor;

    float itnsty = 0.5;

    char tecla = '\0';
    char buffer = '\0';

    // Buffer para comandos de string
    char comando[10] = {0}; // Armazena até 9 caracteres + terminador '\0'
    int comando_index = 0;  // Índice atual no buffer

    // Frequência de clock para 128 MHz
    ok = set_sys_clock_khz(128000, false);

    init_teclado();
    stdio_init_all();
    uart_init(uart0, 115200);

    printf("WELCOME TO YOUR ENGLISH TEST!\n");
    printf("                             \n");
    printf("1 - CLIMATE;\n");
    printf("2 - EMOTIONS;\n");
    printf("3 - FRUITS;\n");
    printf("4 - GEOMETRY;\n");
    printf("5 - TRANSPORT;\n");
    printf("                            \n");
    printf("For the color test write the firste letter of the colors!");
    printf("                            \n");
    printf("GO AHEAD AND GOOD LUCK!");

    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    while (true) {
        while (tecla == '\0') {
            tecla = keypad_leitura();
            sleep_ms(200);
            if (tecla == '\0') {
                itnsty = 0.05;
                tecla = getchar_timeout_us(50);
                if (tecla == PICO_ERROR_TIMEOUT) tecla = '\0';
            } else {
                itnsty = 1;
            }
        }

        // Verifica se a tecla é uma letra de cor
        if (tecla == 'R' || tecla == 'r') {
            cor = matrix_rgb(0.0, 1 * itnsty, 0.0); // Vermelho
            controlar_todos_leds(pio, sm, cor);      // Envia a cor para todos os LEDs
            printf("THIS IS RED\n\n");
        } else if (tecla == 'B' || tecla == 'b') {
            cor = matrix_rgb(1 * itnsty, 0.0, 0.0); // Azul
            controlar_todos_leds(pio, sm, cor);      // Envia a cor para todos os LEDs
            printf("THIS IS BLUE\n\n");
        } else if (tecla == 'G' || tecla == 'g') {
            cor = matrix_rgb(0.0, 0.0, 1 * itnsty); // Verde
            controlar_todos_leds(pio, sm, cor);      // Envia a cor para todos os LEDs
            printf("THIS IS GREEN!\n\n");
        } else if (tecla == 'Y' || tecla == 'y') {
            cor = matrix_rgb(0.0, 1 * itnsty, 1 * itnsty); // Amarelo
            controlar_todos_leds(pio, sm, cor);            // Envia a cor para todos os LEDs
            printf("THIS IS YELLOW!\n\n");
        } else if (tecla == 'P' || tecla == 'p') {
            cor = matrix_rgb(1 * itnsty, 1 * itnsty, 0.0); // Rosa
            controlar_todos_leds(pio, sm, cor);            // Envia a cor para todos os LEDs
            printf("THIS IS PINK!\n\n");
        } else if (tecla == 'W' || tecla == 'w') {
            cor = matrix_rgb(1 * itnsty, 1 * itnsty, 1 * itnsty); // Branco
            controlar_todos_leds(pio, sm, cor);                   // Envia a cor para todos os LEDs
            printf("THIS IS WHITE!\n\n");
        } else if (tecla == 'C' || tecla == 'c') {
            cor = matrix_rgb(0.5 * itnsty, 0.0, 1 * itnsty); // Ciano
            controlar_todos_leds(pio, sm, cor);            // Envia a cor para todos os LEDs
            printf("THIS IS CYAN!\n\n");
        } else if (tecla == 'V' || tecla == 'v') {
            cor = matrix_rgb(1 * itnsty, 0.5 * itnsty, 0.0); // Violeta
            controlar_todos_leds(pio, sm, cor);              // Envia a cor para todos os LEDs
            printf("THIS IS VIOLET!\n\n");
        } else if (tecla == 'T') {
            cor = matrix_rgb(0.0, 0.0, 0.0); // Desliga os LEDs
            controlar_todos_leds(pio, sm, cor); // Envia a cor para todos os LEDs
            printf("TURNING OFF THE LEDs\n\n");
        }

        if (('0' <= tecla && tecla <= '9')) printf("Tecla %c pressionada!\n", tecla);
        if (tecla == 'T') {
            cor = matrix_rgb(0.0, 0.0, 0.0);
            printf("TURNING OFF THE LEDs\n\n");
        } else if (tecla == '1') {
            if (buffer != tecla) {
                play_buzzer(BUZZER_PIN, MI, 200);
                frame_numbers(1, pio, sm, itnsty);
            }
            frame_climate(pio, sm, itnsty);
        } else if (tecla == '2') {
            if (buffer != tecla) {
                play_buzzer(BUZZER_PIN, FA, 200);
                frame_numbers(2, pio, sm, itnsty);
            }
            printf("PAY ATENTION TO THE FRAMES\n\n");
            frame_emotions(pio, sm, itnsty);
        } else if (tecla == '3') {
            if (buffer != tecla) {
                play_buzzer(BUZZER_PIN, SOL, 200);
                frame_numbers(3, pio, sm, itnsty);
            }
            printf("PAY ATENTION TO THE FRAMES\n\n");
            frame_fruits(pio, sm, itnsty == 1);
        } else if (tecla == '4') {
            if (buffer != tecla) {
                play_buzzer(BUZZER_PIN, LA, 200);
                frame_numbers(4, pio, sm, itnsty);
            }
            printf("PAY ATENTION TO THE FRAMES\n\n");
            frame_geometry(pio, sm, itnsty);
        } else if (tecla == '5') {
            if (buffer != tecla) {
                play_buzzer(BUZZER_PIN, SI, 200);
                frame_numbers(5, pio, sm, itnsty);
            }
            printf("PAY ATENTION TO THE FRAMES\n\n");
            frame_transport(pio, sm, itnsty);
        }

        buffer = tecla;
        tecla = '\0';
        sleep_ms(100); // Aguarda 100ms para evitar leituras repetidas
    }
}