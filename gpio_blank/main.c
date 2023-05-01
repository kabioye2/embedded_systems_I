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

#define GPIO_BASE 0x50000000
#define GPIO_OFFSET 0x504
#define CNF_PIN_OFFSET 0x50000700
#define LEDS 0x3800000 //for all LEDS at same time 0x3800000;

typedef struct{
  uint32_t OUT;
  uint32_t OUTSET; 
  uint32_t OUTCLR;
  uint32_t IN;
  uint32_t DIR;
  uint32_t DIRSET;
  uint32_t DIRCLR; 
}Set_GPIO_Struct;


typedef struct{
  uint32_t CNF_PIN[32];
}Config_Pin_Struct;

//Declare and setup struct pointer
Set_GPIO_Struct* SET_GPIO = (Set_GPIO_Struct*) (GPIO_BASE + GPIO_OFFSET);
Config_Pin_Struct* CONFIG_PIN = (Config_Pin_Struct*) CNF_PIN_OFFSET;



int main(void) {
  ret_code_t error_code = NRF_SUCCESS;


  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");


  //Base address
  //uint32_t base_add = 0x50000000;
  //uint32_t pCNF = 0x700;
  //uint32_t base_CNF = base_add + pCNF;
  //uint32_t numBytes = 4;

  //GPIO port addresses
  //uint32_t OUTSET_add = 0x50000000 + 0x508;
  //uint32_t* pOutset = (uint32_t*)( base_add  + 0x508);
  //uint32_t* pOutclr = (uint32_t*)( base_add  + 0x50C);                                                                                     
  //uint32_t* pCNF_0 = (uint32_t*)(base_add + 0x700);
   

  //LED pin addresses
  //uint32_t* pCNF_23 = (uint32_t*)(base_CNF + numBytes * 23);
  //uint32_t* pCNF_24 = (uint32_t*)(base_CNF + numBytes * 24);
  //uint32_t* pCNF_25 = (uint32_t*)(base_CNF + numBytes * 25);

  //Configuration for pin (sets direction to output and disconnects input buffer)
  //*pCNF_23 = 0x3;
  //*pCNF_24 = 0x3;
  //*pCNF_25 = 0x3;

  //set input output
  CONFIG_PIN->CNF_PIN[23] = 1, //LED
  CONFIG_PIN->CNF_PIN[24] = 1, //LED
  CONFIG_PIN->CNF_PIN[25] = 1; //LED
  CONFIG_PIN->CNF_PIN[22] = 0;//switch
  CONFIG_PIN->CNF_PIN[28] = 0;//button


   


//print address
  printf("OUTSET: %p\n", &SET_GPIO->OUTSET);
  printf("OUTCLR: %p\n", &SET_GPIO->OUTCLR);
  printf("CNF 0: %p\n", &CONFIG_PIN->CNF_PIN[0]);


  

  // loop forever
  while (1) 
  {
    //set switch/button to input
    uint32_t button_value = (SET_GPIO->IN >> 28) & 0x1;
    uint32_t switch_value = (SET_GPIO->IN >> 22) & 0x1;
    printf("Button value: %ld\n", button_value);
    printf("Switch value: %ld\n", switch_value);


    //Turn on LEDs
    SET_GPIO->OUTCLR = LEDS;
    nrf_delay_ms(500);


    //Turn ff LEDs
    SET_GPIO->OUTSET = LEDS;
    nrf_delay_ms(500);

  }
}

