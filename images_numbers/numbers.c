#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "pio_matrix.pio.h"

#include "numbers.h"

//número de LEDs
#define NUM_PIXELS 25

//pino de saída
#define OUT_PIN 7

//vetor para criar imagem na matriz de led - 1
double desenho0[25] =   {0.0, 1.0, 1.0, 1.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 1.0, 
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 1.0, 1.0, 1.0, 0.0};

double desenho1[25] =   {0.0, 0.0, 1.0, 0.0, 0.0,
                         0.0, 0.0, 1.0, 1.0, 0.0, 
                         1.0, 0.0, 1.0, 0.0, 0.0,
                         0.0, 0.0, 1.0, 0.0, 0.0,
                         0.0, 0.0, 1.0, 0.0, 0.0};

double desenho2[25] =   {0.0, 1.0, 1.0, 1.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 1.0, 
                        0.0, 0.0, 1.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0, 0.0,
                        1.0, 1.0, 1.0, 1.0, 1.0};

double desenho3[25] =   {0.0, 1.0, 1.0, 1.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 1.0, 1.0, 0.0};

double desenho4[25] =   {0.0, 1.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0, 
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 1.0, 0.0};

double desenho5[25] =   {0.0, 1.0, 1.0, 1.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 1.0, 
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 0.0,
                        1.0, 1.0, 1.0, 1.0, 0.0};

double desenho6[25] =   {1.0, 1.0, 1.0, 1.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0};

double desenho7[25] =   {0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 1.0, 0.0};

double desenho8[25] =   {0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0, 
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 1.0, 1.0, 0.0};

double desenho9[25] =   {0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0, 
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 1.0, 1.0, 0.0};

//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(b=0.0, r=0.0, desenho[24-i]);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

//função principal
void main_animacao(uint num, PIO pio, uint sm)
{
    uint32_t valor_led;
    double r = 0.0, b = 0.0 , g = 0.0;
    switch (num) {
        case 0:
            desenho_pio(desenho0, valor_led, pio, sm, r, g, b);
            break;
        case 1:
            desenho_pio(desenho1, valor_led, pio, sm, r, g, b);
            break;
        case 2:
            desenho_pio(desenho2, valor_led, pio, sm, r, g, b);
            break;
        case 3:
            desenho_pio(desenho3, valor_led, pio, sm, r, g, b);
            break;
        case 4:
            desenho_pio(desenho4, valor_led, pio, sm, r, g, b);
            break;
        case 5:
            desenho_pio(desenho5, valor_led, pio, sm, r, g, b);
            break;
        case 6:
            desenho_pio(desenho6, valor_led, pio, sm, r, g, b);
            break;
        case 7:
            desenho_pio(desenho7, valor_led, pio, sm, r, g, b);
            break;
        case 8:
            desenho_pio(desenho8, valor_led, pio, sm, r, g, b);
            break;
        case 9:
            desenho_pio(desenho9, valor_led, pio, sm, r, g, b);
            break;
    }
}