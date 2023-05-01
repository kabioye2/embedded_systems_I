

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

#define BUTTON0 28


void timer_init(){
  // fill in this function to initialize a timer of your choice
  NRF_TIMER4->PRESCALER = 9;
  NRF_TIMER4->BITMODE = 3; //32-bit
  NRF_TIMER4->MODE = 0x00; //timer

  //start timer
  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;

  //init counter
  NRF_TIMER3->BITMODE = 3;
  NRF_TIMER3->MODE = 0x01; //counter
  NRF_TIMER4->CC[1] = 93750; //(3*16 MHz)/512

  NRF_TIMER4->INTENSET = (1 << 17);

  //start counter
  NRF_TIMER3->TASKS_CLEAR = 0x01;
  NRF_TIMER3->TASKS_START = 0x01;

}


uint32_t read_timer(){

  // fill in this function for reading the timer value on calling this function
  NRF_TIMER4->TASKS_CAPTURE[0] = 0x01;
  uint32_t value = NRF_TIMER4->CC[0];
  uint32_t elapsed_secs = value/31250;
  return elapsed_secs;
}

uint32_t read_counter(){

  // fill in this function for reading the timer value on calling this function
  NRF_TIMER3->TASKS_CAPTURE[0] = 0x01;
  uint32_t value = NRF_TIMER3->CC[0];
  return value;
}

void TIMER4_IRQHandler(void){
  NRF_TIMER3->TASKS_COUNT = 0X01;
  printf("Counter readings are: %ld \n", read_counter());
  NRF_TIMER4->EVENTS_COMPARE[1]=0;//clear compare
  NRF_TIMER4->TASKS_CLEAR = 0x01;//restart timer
}

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Board initialized!\n");

  // You can use the NRF GPIO library to test your timers
  nrf_gpio_pin_dir_set(BUCKLER_LED0, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(BUCKLER_LED1, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(BUCKLER_LED2, NRF_GPIO_PIN_DIR_OUTPUT);

  NVIC_EnableIRQ(TIMER4_IRQn);

  timer_init();  

  //nrf_delay_ms(3000);
  
  // loop forever
  while (1) {
    printf("Timer 4 readings are: %ld \n", read_timer());
    
    
    // for better readability on the terminal window, adding some delay is useful
    nrf_delay_ms(1000);
  }
}