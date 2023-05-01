// Implementing INterrupts on GPIO Events
//
// - To generate interrupt as a an input event occurs
// - To implement a second interrupt with conflicting priority
// - To implement the same second interrupt wiht a lower priority

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_gpio.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "software_interrupt.h"

#include "buckler.h"
#include "gpio.h"

#define LED0 23
#define LED1 24
#define LED2 25
#define BUTTON0 28
#define SWITCH0 22 

Set_GPIO_Struct* SETUP_GPIO = (Set_GPIO_Struct*) (GPIO_BASE + GPIO_OFFSET);
Config_Pin_Struct* CONFIG_CNF_PIN = (Config_Pin_Struct*) CNF_PIN_OFFSET;

//void SWI1_EGU1_IRQHandler(void) {
//  printf("SWI1_EGU1 interrupt received\n");
//    NRF_EGU1->EVENTS_TRIGGERED[0] = 0;
//
//    nrf_delay_ms(100);
//
//    printf("SWI1_EGU1 interrupt finished\n");
//}

void GPIOTE_IRQHandler(void) {

// check which event was executed, and clear that interrupt
  
  // Turn ON LED0 for Button0


  // Turn ON LED1 for Switch0

  if (NRF_GPIOTE->EVENTS_IN[0]) {
    NRF_GPIOTE->EVENTS_IN[0] = 0;
    gpio_clear(LED0);
     printf("Button0 LED on\n");
     nrf_delay_ms(1000);
     gpio_set(LED0);
  }

  if (NRF_GPIOTE->EVENTS_IN[1]) {
    NRF_GPIOTE->EVENTS_IN[1] = 0;
    gpio_clear(LED1);
    printf("Switch0 LED on\n");
    nrf_delay_ms(1000);
    gpio_set(LED1);
  }
  
}

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  gpio_set(LED0);
  gpio_set(LED1);

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");


  // Q5, Q6 - configure a GPIOTE Event to occur for a Button press
  
  gpio_config(LED0, OUTPUT);

  gpio_config(LED1, OUTPUT);

  gpio_config(BUTTON0, INPUT);

  gpio_config(SWITCH0, INPUT);

  NRF_GPIOTE->CONFIG[0] = 0x11c01;
  NRF_GPIOTE->CONFIG[1] = 0x11601;

  NRF_GPIOTE->INTENSET = 0x03;

  NVIC_EnableIRQ(GPIOTE_IRQn);

  // Q7, Q8 - initialize a software interrupt and set its priority
  //software_interrupt_init(); 
  //NVIC_SetPriority(SWI1_EGU1_IRQn, 1);
  //NVIC_SetPriority(GPIOTE_IRQn, 0);


  // loop forever
  while (1) {
    printf("Looping\n");
    nrf_delay_ms(1000);
    //software_interrupt_generate();
  }
}

