// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "images_numbers/numbers.h"

// #define OUT_PIN 7
// #define BUTTON_A 5 // Botão A = 5, Botão B = 6 , BotãoJoy = 22
// #define BUTTON_B 6 // Botão A = 5, Botão B = 6 , BotãoJoy = 22

// static volatile uint32_t last_time = 0; 
// static volatile uint a = 0;
// PIO pio; 
// uint sm;

// // Função de interrupção com debouncing
// void gpio_irq_handler(uint gpio, uint32_t events)
// {
//     // Obtém o tempo atual em microssegundos
//     uint32_t current_time = to_us_since_boot(get_absolute_time());

//     // Verifica se passou tempo suficiente desde o último evento
//     if (current_time - last_time > 50000) // 50 ms de debouncing
//     {
//         if (gpio == BUTTON_A && a != 9) {
//             printf("A = %d\n", ++a);
//         } else if (gpio == BUTTON_B && a != 0) {
//             printf("A = %d\n", --a);
//         }

//         last_time = current_time; // Atualiza o tempo do último evento
//         main_animacao(a, pio, sm);
//     }
// }

// void pinos_config() 
// {
//     stdio_init_all();

//     gpio_init(BUTTON_A);
//     gpio_set_dir(BUTTON_A, GPIO_IN); // Configura o pino como entrada
//     gpio_pull_up(BUTTON_A);          // Habilita o pull-up interno

//     gpio_init(BUTTON_B);
//     gpio_set_dir(BUTTON_B, GPIO_IN); // Configura o pino como entrada
//     gpio_pull_up(BUTTON_B);          // Habilita o pull-up interno

//     // Configuração da interrupção com callback
//     gpio_set_irq_enabled(BUTTON_A, GPIO_IRQ_EDGE_FALL, true);
//     gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);
//     gpio_set_irq_callback(&gpio_irq_handler);
//     irq_set_enabled(IO_IRQ_BANK0, true);
// }

// uint pio_config(PIO pio) 
// {
//     bool ok;

//     //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
//     ok = set_sys_clock_khz(128000, false);

//     printf("iniciando a transmissão PIO");
//     if (ok) printf("clock set to %ld\n", clock_get_hz(clk_sys));

//     //configurações da PIO
//     uint offset = pio_add_program(pio, &pio_matrix_program);
//     uint sm = pio_claim_unused_sm(pio, true);
//     pio_matrix_program_init(pio, sm, offset, OUT_PIN);

//     return sm;
// }

// int main()
// {
//     pio = pio0; 
//     pinos_config();
//     sm = pio_config(pio);

//     main_animacao(a, pio, sm);

//     while (true) 
//     {
//     }
// }

//------------------------------------------------------------------

/* Primeiro Código - Configuração dos Botões e PIO */
#include <stdio.h>
#include "pico/stdlib.h"
#include "images_numbers/numbers.h"

#define OUT_PIN 7
#define OUT_PIN_RED 13

#define BUTTON_A 5
#define BUTTON_B 6

static volatile uint32_t last_time = 0; 
static volatile uint a = 0;
PIO pio;
uint sm;

// Função de interrupção com debouncing aprimorado
void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - last_time > 250000) // 150 ms para evitar pulos
    {
        if (gpio == BUTTON_A && a < 9) {
            a++;
        } else if (gpio == BUTTON_B && a > 0) {
            a--;
        }
        printf("A = %d\n", a);
        last_time = current_time;
        main_animacao(a, pio, sm);
    }
}

void pinos_config() 
{
    stdio_init_all();

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    gpio_init(OUT_PIN_RED);
    gpio_set_dir(OUT_PIN_RED, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}

uint pio_config(PIO pio) 
{
    set_sys_clock_khz(128000, false);
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);
    return sm;
}

int main()
{
    pio = pio0; 
    pinos_config();
    sm = pio_config(pio);

    main_animacao(a, pio, sm);

    while (true) 
    {
        // Liga o LED vermelho
        gpio_put(OUT_PIN_RED, true);
        sleep_ms(100); // Mantém ligado por 100ms (metade do período para 5Hz)

        // Desliga o LED vermelho
        gpio_put(OUT_PIN_RED, false);          
        sleep_ms(100); // Mantém desligado por 100ms
    }
}
