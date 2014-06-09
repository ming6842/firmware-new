#include "led.h"
#include "delay.h"
void cycle_led(uint8_t times){

LED_OFF(LED4);
LED_OFF(LED3);
LED_OFF(LED4);
LED_OFF(LED2);
Delay_1us(1000);


LED_ON(LED4);
LED_OFF(LED3);
LED_OFF(LED1);
LED_OFF(LED2);
while(times--){

LED_TOGGLE(LED4);
LED_TOGGLE(LED1);
Delay_1us(30000);

LED_TOGGLE(LED1);
LED_TOGGLE(LED2);
Delay_1us(30000);

LED_TOGGLE(LED3);
LED_TOGGLE(LED2);
Delay_1us(30000);

LED_TOGGLE(LED3);
LED_TOGGLE(LED4);
Delay_1us(30000);


}

	LED_OFF(LED4);
	LED_OFF(LED3);
	LED_OFF(LED4);
	LED_OFF(LED2);
	Delay_1us(2000);

}