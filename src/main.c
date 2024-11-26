#include <stdbool.h>
#include <stm8s.h>
#include "main.h"
#include "milis.h"

#define BTN

void init(void) {
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // Taktování MCU na 16MHz

    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT); // Inicializace tlačítka

    GPIO_Init(LED1_PORT, LED1_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW); // Inicializace pinů
    GPIO_Init(LED2_PORT, LED2_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW); // OUT_PP - Push-Pull (Pin může řídit připojené zařízení jak do úrovně HIGH (logická 1), tak LOW (logická 0).)
    GPIO_Init(LED3_PORT, LED3_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW); // HIGH - Pin je inicializován na vysokou úroveň (logická 1) při spuštění.
    GPIO_Init(LED4_PORT, LED4_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW); // Slow - Pin je nastaven na nízkou rychlost spínání

    init_milis();
}

int main(void) {
    uint8_t BTN_aktualni_stav = 1;
    uint8_t BTN_posledni_stav = 1;
    uint8_t LED_stav = 1;

    init();

    while (1) {
        BTN_aktualni_stav = GPIO_ReadInputPin(BTN_PORT, BTN_PIN); 

        if (BTN_posledni_stav == 0 && BTN_aktualni_stav == 1) { //Reakce na zmáčnutí tlačítka (když projede while tak se změní hodnota, if se změní až BTN_aktualni_stav skočí z 1 na 0)
            LED_stav += 1;                                      //Při prvním cyklu se nevykoná, protože BTN_posledni_stav = 1, až diký poslednímu příkazu se změní na 0
        }

        // Nastavení výstupů pro LED
        if (LED_stav == 1) {
            GPIO_WriteLow(LED1_PORT, LED1_PIN);
            GPIO_WriteHigh(LED4_PORT, LED4_PIN);
        }
        else if (LED_stav == 2) {
            GPIO_WriteHigh(LED1_PORT, LED1_PIN);
            GPIO_WriteLow(LED2_PORT, LED2_PIN);
        }
        else if (LED_stav == 3) {
            GPIO_WriteHigh(LED2_PORT, LED2_PIN);
            GPIO_WriteLow(LED3_PORT, LED3_PIN);
        }
        else if (LED_stav == 4) {
            GPIO_WriteHigh(LED3_PORT, LED3_PIN);
            GPIO_WriteLow(LED4_PORT, LED4_PIN);
        }
        else if (LED_stav == 5) { //Jakmile dosáhne stavu 5 přepne se opět na 1, to umožní nekonečný cyklus měnění 4 LEDek
            LED_stav = 1;
        }
        BTN_posledni_stav = BTN_aktualni_stav; //Aktualizace posledního stavu tlačítka
    }
}