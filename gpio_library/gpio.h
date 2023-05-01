#pragma once

#include "nrf.h"
#include "stdbool.h"

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


typedef enum {
    INPUT = 0,
    OUTPUT,
} gpio_direction_t;


// Inputs: 
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir);

// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num);

// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num);

// Inputs: 
//  gpio_num - gpio number 0-31
// Returns:
//  current state of the specified gpio pin
bool gpio_read(uint8_t gpio_num);
