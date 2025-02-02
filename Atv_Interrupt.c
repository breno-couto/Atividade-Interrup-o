#include <stdio.h>
#include "pico/stdlib.h"
#include <hardware/clocks.h>
#include "pico/bootrom.h"
#include "ws2818b.pio.h"

//definicao do numero de leds e pin
#define LED_COUNT 25
#define LED_PIN 7

//definicao dos botoes e leds
#define BUTTON_A 5
#define BUTTON_B 6
#define LED_VERMELHO 13
#define LED_VERDE 11
#define LED_AZUL 12

//variavel controladora da matriz de leds
int contador = 1;

//variaveis de controle
static volatile uint a = 1;
static volatile uint32_t last_time = 0;

//numeros para matriz 5x5 
////////////////////////////////////////////////
int ZERO[5][5][3] = {
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}}
};

int ONE[5][5][3] = {
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}}
};

int TWO[5][5][3] = {
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}}
};

int THREE[5][5][3] = {
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}}
};

int FOUR[5][5][3] = {
    {{0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}}
};

int FIVE[5][5][3] = {
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}}
};

int SIX[5][5][3] = {
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}}
};

int SEVEN[5][5][3] = {
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}}
};

int EIGHT[5][5][3] = {
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}}
};

int NINE[5][5][3] = {
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 249}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 249}, {255, 0, 249}, {255, 0, 249}, {0, 0, 0}}
};

///////////////////////////////////////////////

//definicao dos pixels
struct pixel_t{
    uint8_t G, R, B;
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;

//definicao do buffer de pixels
npLED_t leds[LED_COUNT];

PIO np_pio;
uint sm;

void inoutput_init();
void npInit(uint pin);
void npSetLed(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npClear();
void npWrite();
int gentleIndex(int x, int y);
void npSetPattern(int matriz[5][5][3]);
static void gpio_irq_handler(uint gpio, uint32_t events);

int main()
{
    stdio_init_all();
    inoutput_init();

    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL,true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL,true, &gpio_irq_handler);

    npInit(LED_PIN);
    npSetPattern(ZERO);

    while(true){
        gpio_put(LED_VERMELHO,true);
        sleep_ms(100);
        gpio_put(LED_VERMELHO,false);
        sleep_ms(100);
    }
}

void inoutput_init()
{
    //Input
    gpio_init(LED_VERMELHO);
    gpio_init(LED_AZUL);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERMELHO, false);
    gpio_put(LED_AZUL, false);
    gpio_put(LED_VERDE, false);

    // Configura os pinos dos botões como entrada
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);  // Ativa o resistor pull-up interno para o botão A

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);  // Ativa o resistor pull-up interno para o botão B
}

static void gpio_irq_handler(uint gpio, uint32_t events)
{
    //tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    printf("A = %d\n", a);

        //passou tempo suficiente desde o último evento?
    if (current_time - last_time > 200000) //200 ms debouncing
    {
        last_time = current_time; //atualiza o tempo do último evento
        printf("Mudanca de Estado do Led. A = %d\n", a);
        a++;                                     //incrementa a variavel de verificação
    

if (gpio_get(BUTTON_A) == 0) {
        if (contador < 11) {
            contador++;
        }
    }

    // Se o botão B for pressionado, decrementa o índice (não abaixo de 0)
    if (gpio_get(BUTTON_B) == 0) {
        if (contador > 0) {
            contador--;
        }
    }
    }
    // Escolha o padrão a ser exibido com base no índice
        switch (contador) {
            case 1:
                npSetPattern(ZERO);  //exibe ZERO
                break;
            case 2:
                npSetPattern(ONE);  //exibe UM
                break;
            case 3:
                npSetPattern(TWO);  //exibe DOIS
                break;
            case 4:
                npSetPattern(THREE);  //exibe TRES
                break;
            case 5:
                npSetPattern(FOUR);  //exibe QUATRO
                break;
            case 6:
                npSetPattern(FIVE);  //exibe CINCO
                break;
            case 7:
                npSetPattern(SIX);  //exibe SEIS
                break;
            case 8:
                npSetPattern(SEVEN);  //exibe SETE
                break;
            case 9:
                npSetPattern(EIGHT);  //exibe OITO
                break;
            case 10:
                npSetPattern(NINE);  //exibe NOVE
                break;
            default:
                break;
        }
}

void npInit(uint pin) {

 //programa pio
 uint offset = pio_add_program(pio0, &ws2818b_program);
 np_pio = pio0;

 //encontra pio
 sm = pio_claim_unused_sm(np_pio, false);
 if (sm < 0) {
   np_pio = pio1;
   sm = pio_claim_unused_sm(np_pio, true);
 }

 ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

 for (uint i = 0; i < LED_COUNT; ++i) {
   leds[i].R = 0;
   leds[i].G = 0;
   leds[i].B = 0;
 }
}

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
 leds[index].R = r;
 leds[index].G = g;
 leds[index].B = b;
}

void npClear() {
 for (uint i = 0; i < LED_COUNT; ++i)
   npSetLED(i, 0, 0, 0);
}

void npWrite() {
 for (uint i = 0; i < LED_COUNT; ++i) {
   pio_sm_put_blocking(np_pio, sm, leds[i].G);
   pio_sm_put_blocking(np_pio, sm, leds[i].R);
   pio_sm_put_blocking(np_pio, sm, leds[i].B);
 }
}

int getIndex(int x, int y) {
    if (y % 2 == 0) {
        return y * 5 + x;
    } else {
        return y * 5 + (4 - x);
    }
}

void npSetPattern(int matriz[5][5][3]) {
    int index = 0;
int conversao[5][5][3]=
{
    {{matriz[4][4][0],matriz[4][4][1], matriz[4][4][2]},{matriz[4][3][0],matriz[4][3][1], matriz[4][3][2]}, {matriz[4][2][0],matriz[4][2][1], matriz[4][2][2]}, {matriz[4][1][0],matriz[4][1][1], matriz[4][1][2]}, {matriz[4][0][0],matriz[4][0][1], matriz[4][0][2]}},
    {{matriz[3][0][0],matriz[3][0][1], matriz[3][0][2]}, {matriz[3][1][0],matriz[3][1][1], matriz[3][1][2]}, {matriz[3][2][0],matriz[3][2][1], matriz[3][2][2]}, {matriz[3][3][0],matriz[3][3][1], matriz[3][3][2]}, {matriz[3][4][0],matriz[3][4][1], matriz[3][4][2]}},
    {{matriz[2][4][0],matriz[2][4][1], matriz[2][4][2]}, {matriz[2][3][0],matriz[2][3][1], matriz[2][3][2]}, {matriz[2][2][0],matriz[2][2][1], matriz[2][2][2]}, {matriz[2][1][0],matriz[2][1][1], matriz[2][1][2]}, {matriz[2][0][0],matriz[2][0][1], matriz[2][0][2]}},
    {{matriz[1][0][0],matriz[1][0][1], matriz[1][0][2]},{matriz[1][1][0],matriz[1][1][1], matriz[1][1][2]}, {matriz[1][2][0],matriz[1][2][1], matriz[1][2][2]}, {matriz[1][3][0],matriz[1][3][1], matriz[1][3][2]}, {matriz[1][4][0],matriz[1][4][1], matriz[1][4][2]}},
    {{matriz[0][4][0],matriz[0][4][1], matriz[0][4][2]}, {matriz[0][3][0],matriz[0][3][1], matriz[0][3][2]}, {matriz[0][2][0],matriz[0][2][1], matriz[0][2][2]}, {matriz[0][1][0],matriz[0][1][1], matriz[0][1][2]}, {matriz[0][0][0],matriz[0][0][1], matriz[0][0][2]}}
};


    for (int linha = 0; linha < 5; linha++) {
        for (int coluna = 0; coluna < 5; coluna++) {
            int r = conversao[linha][coluna][0]; //R
            int g = conversao[linha][coluna][1]; //G
            int b = conversao[linha][coluna][2]; //B

            //acende o led
            npSetLED(index, r, g, b);
            index++;
        }
    }
    npWrite(); //envia dados
}
