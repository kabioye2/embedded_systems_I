
// Blink app
//
// Blinks the LEDs on Buckler

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"
#include "gpio.h"

#define LED0 23
#define LED1 24
#define LED2 25
#define BUTTON0 28
#define SWITCH0 22 

Set_GPIO_Struct* SETUP_GPIO = (Set_GPIO_Struct*) (GPIO_BASE + GPIO_OFFSET);
Config_Pin_Struct* CONFIG_CNF_PIN = (Config_Pin_Struct*) CNF_PIN_OFFSET;


int main(void) {
  ret_code_t error_code = NRF_SUCCESS;


  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");



  // Configure LED0 and LED1 as outputs
  gpio_config(LED0, OUTPUT);
  gpio_config(LED1, OUTPUT);
  gpio_config(LED2, OUTPUT);

  // Configure BUTTON0 and SWITCH0 as inputs
  gpio_config(BUTTON0, INPUT);


  // loop forever
 while (1) {
        if (gpio_read(BUTTON0)) {
            gpio_set(LED0);
            gpio_set(LED1);
            gpio_set(LED2);
        } else {
            gpio_clear(LED0);
            gpio_clear(LED1);
            gpio_clear(LED2);
        }
    }

    return 0;
}

