

//** Code: Hard Timers (Lab 6)
//** Author: Umer Huzaifa
//** Date: 12/30/2021
//** Comments: Keeping a separate app for the hardware timers and counters


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"

#define WAVEFORM1 (16000000)/512
#define WAVEFORM2 (2*16000000)/512
#define WAVEFORM3 (4*16000000)/512


void timer_init(){
  // fill in this function to initialize a timer of your choice
  NRF_TIMER4->PRESCALER = 9;
  NRF_TIMER4->BITMODE = 3; //32-bit
  NRF_TIMER4->MODE = 0x00; //timer

  NRF_TIMER4->CC[0] = WAVEFORM1;//(16 MHz)/512
  NRF_TIMER4->CC[1] = WAVEFORM2;//(2*16 MHz)/512
  NRF_TIMER4->CC[2] = WAVEFORM3;//(4*16 MHz)/512

  // enable interrupts for COMPARE[0], COMPARE[1] and COMPARE[2]
  NRF_TIMER4->INTENSET |= (1 << 16) | (1 << 17) | (1 << 18);

  //start timer
  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;


  NVIC_EnableIRQ(TIMER4_IRQn);

}

void TIMER4_IRQHandler(void) {
  printf("inside handler\n");
  if (NRF_TIMER4->EVENTS_COMPARE[0]) {
    NRF_TIMER4->EVENTS_COMPARE[0] = 0;
    nrf_gpio_pin_toggle(BUCKLER_LED0);
  }

  if (NRF_TIMER4->EVENTS_COMPARE[1]) {
    NRF_TIMER4->EVENTS_COMPARE[1] = 0;
    nrf_gpio_pin_toggle(BUCKLER_LED1);
  }

  if (NRF_TIMER4->EVENTS_COMPARE[2]) {
    NRF_TIMER4->EVENTS_COMPARE[2] = 0;
    nrf_gpio_pin_toggle(BUCKLER_LED2);
  }
}


int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Board initialized!\n");

  // set 3 gpios as output
  nrf_gpio_pin_dir_set(BUCKLER_LED0, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(BUCKLER_LED1, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(BUCKLER_LED2, NRF_GPIO_PIN_DIR_OUTPUT);


  timer_init();  

 
  while (1) {
    nrf_delay_ms(1000);
   }
}
