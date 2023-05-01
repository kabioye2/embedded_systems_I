#include "gpio.h"


extern Set_GPIO_Struct* SETUP_GPIO;
extern Config_Pin_Struct* CONFIG_CNF_PIN;


// Inputs: 
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {


  if (dir == INPUT) {
    //to configure for pull up and input binary is 1100 which is 12
    //configure for inout
    CONFIG_CNF_PIN->CNF_PIN[gpio_num] &= 0;
    
  } else if (dir == OUTPUT) {
    CONFIG_CNF_PIN->CNF_PIN[gpio_num] |= 1;
  }
}

// Set gpio_num high
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num) {
    SETUP_GPIO->OUTSET = (1 << gpio_num);

}

// Set gpio_num low
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num) {
    SETUP_GPIO->OUTCLR = (1 << gpio_num);

}

// Inputs: 
//  gpio_num - gpio number 0-31
bool gpio_read(uint8_t gpio_num) {
    // should return pin state
    return (SETUP_GPIO->IN & (1 << gpio_num));
}
