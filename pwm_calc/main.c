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
#include "app_pwm.h"
#include "nrfx_gpiote.h"

#include "buckler.h"


#define PWM_GEN NRF_GPIO_PIN_MAP(0,18)
#define PWM_READ NRF_GPIO_PIN_MAP(0,14)
#define DUTYCYCLE 60


 volatile float freq = 0;
 volatile float period_T = 0;
 volatile uint32_t pulse_width = 0;
 volatile float duty_cycle = 0;


// Create the instance "PWM1" using TIMER1.
APP_PWM_INSTANCE(PWM1, 1);


void timer_init(){
  //init timer
  NRF_TIMER4->PRESCALER = 9;
  NRF_TIMER4->BITMODE = 3; //32-bit
  NRF_TIMER4->MODE = 0x00; //timer

  //start timer
  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01; 

  //init counter
  NRF_TIMER3->BITMODE = 3;
  NRF_TIMER3->MODE = 0x01; //counter

  //start counter
  NRF_TIMER3->TASKS_CLEAR = 0x01;
  NRF_TIMER3->TASKS_START = 0x01;
}

 int read_timer(){
  NRF_TIMER4->TASKS_CAPTURE[0] = 0x01;
  return NRF_TIMER4->CC[0];
}

uint32_t read_counter(){
  NRF_TIMER3->TASKS_CAPTURE[0] = 0x01;
  uint32_t value = NRF_TIMER3->CC[0];
  return value;
}


static uint32_t last_falling = 0;
static uint32_t last_rising = 0;


void myHandler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {

   uint32_t current_time = read_timer();

    //active high so first edge condition is met when pin is 0
    bool is_falling = (nrf_gpio_pin_read(pin) == 0);

    //if edge condition met
    if (is_falling) {
        //increment count
        NRF_TIMER3->TASKS_COUNT = 0X01; //increment counter

        //so last time isnt overwritten before calculation
        if(read_counter() < 2){
            last_falling = current_time;
        }
        else{
            printf("Last Falling: %d\n", last_falling);
            period_T = (float)((current_time - last_falling));//* (1/31250));
            printf("period: %f\n", period_T);
            pulse_width = last_rising - last_falling;
            freq = 1.0/period_T;
            duty_cycle = ((float)pulse_width / (current_time - last_falling)) * 100.0f;
            printf("duty_cycle: %.4f\t Freq: %f Hz\n", duty_cycle, freq);
            nrf_delay_ms(100);
            //no longer need previos time so overwrite
            last_falling = current_time;

           // Reset counter
            NRF_TIMER3->TASKS_CLEAR = 0x01;//restart counter;
        }
    } else {
        // Falling edge detected
        last_rising = current_time;
    }   
}



int main(void)
{
    ret_code_t error_code;
    ret_code_t err_code;

    // initialize RTT library
    error_code = NRF_SUCCESS;
    error_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(error_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    printf("Board initialized!\n");


    nrfx_gpiote_init();
    nrfx_gpiote_evt_handler_t gpiote_handler = myHandler;

    //configure pin
    nrfx_gpiote_in_config_t in_config;
    in_config.sense = NRF_GPIOTE_POLARITY_TOGGLE;
    in_config.pull = NRF_GPIO_PIN_NOPULL;
    in_config.is_watcher = false;
    in_config.skip_gpio_setup = false;

    //initialize handler
    nrfx_gpiote_in_init(PWM_READ, &in_config, gpiote_handler);

    //enable handler
    nrfx_gpiote_in_event_enable(PWM_READ, true);


    /*PWM CONFIG*/
    app_pwm_config_t pwm_config = APP_PWM_DEFAULT_CONFIG_1CH(5000, PWM_GEN);
    
    err_code = app_pwm_init(&PWM1, &pwm_config, NULL);
    
    APP_ERROR_CHECK(err_code);
    app_pwm_enable(&PWM1);

    //catch errors
    err_code = app_pwm_channel_duty_set(&PWM1, 0, DUTYCYCLE);
    APP_ERROR_CHECK(err_code);
    err_code = app_pwm_busy_check(&PWM1);
    APP_ERROR_CHECK(err_code);

    //initilaize timer
    timer_init();
    
 
    while (read_timer() < 5000000)
    {
        __WFI();
        
    }

}


/** @} */

