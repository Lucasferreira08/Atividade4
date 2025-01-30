#include <stdio.h>
#include "pico/stdlib.h"
#include "images_numbers/numbers.h"

// Define os pinos de saída para o LED e o LED vermelho
#define OUT_PIN 7
#define OUT_PIN_RED 13

// Define os pinos dos botões A e B
#define BUTTON_A 5
#define BUTTON_B 6

// Variáveis globais para controle de tempo e estado
static volatile uint32_t last_time = 0; // Armazena o último tempo de interrupção
static volatile uint a = 0; // Variável de estado que será incrementada/decrementada pelos botões

// Variáveis para o PIO (Programmable I/O) e state machine (máquina de estados)
PIO pio;
uint sm;

// Função de interrupção com debouncing aprimorado
void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // Obtém o tempo atual em microssegundos

    // Verifica se passaram pelo menos 250ms desde a última interrupção (debouncing)
    if (current_time - last_time > 250000) // 250 ms para evitar pulos
    {
        // Se o botão A foi pressionado e 'a' é menor que 9, incrementa 'a'
        if (gpio == BUTTON_A && a < 9) {
            a++;
        } 
        // Se o botão B foi pressionado e 'a' é maior que 0, decrementa 'a'
        else if (gpio == BUTTON_B && a > 0) {
            a--;
        }

        // Exibe o valor atual de 'a' no console
        printf("A = %d\n", a);

        // Atualiza o último tempo de interrupção
        last_time = current_time;

        // Chama a função de animação principal com o valor atual de 'a'
        main_animacao(a, pio, sm);
    }
}

// Função para configurar os pinos
void pinos_config() 
{
    stdio_init_all(); // Inicializa a comunicação serial (para printf)

    // Configura o pino do botão A como entrada com pull-up
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Configura o pino do botão B como entrada com pull-up
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Configura o pino do LED vermelho como saída
    gpio_init(OUT_PIN_RED);
    gpio_set_dir(OUT_PIN_RED, GPIO_OUT);

    // Habilita a interrupção para os botões A e B na borda de descida (quando o botão é pressionado)
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}

// Função para configurar o PIO (Programmable I/O)
uint pio_config(PIO pio) 
{
    set_sys_clock_khz(128000, false); // Configura o clock do sistema para 128 MHz

    // Adiciona o programa PIO para controle da matriz de LEDs
    uint offset = pio_add_program(pio, &pio_matrix_program);

    // Obtém uma state machine (máquina de estados) não utilizada
    uint sm = pio_claim_unused_sm(pio, true);

    // Inicializa o programa PIO na state machine com o pino de saída definido
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    return sm; // Retorna a state machine configurada
}

// Função principal
int main()
{
    pio = pio0; // Usa o PIO0 para controle da matriz de LEDs

    // Configura os pinos e o PIO
    pinos_config();
    sm = pio_config(pio);

    // Inicia a animação principal com o valor inicial de 'a'
    main_animacao(a, pio, sm);

    // Loop infinito para piscar o LED vermelho
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