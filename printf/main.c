// printf app
//
// Use RTT to print messages via printf

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // loop forever
  uint32_t i = 0;
  volatile uint32_t myglobal=1;
  printf("%p\n", &i);
  while (1) {
    nrf_delay_ms(1000);
    printf("Iteration: %lu\n", i++);
    if (i>5){
      myglobal = myglobal+i;
    }
  }
}

