/**
 * Copyright (c) 2015 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 * @defgroup pwm_example_main main.c
 * @{
 * @ingroup pwm_example
 *
 * @brief  PWM Example Application main file.
 *
 * This file contains the source code for a sample application using PWM.
 *
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_error.h"
#include "bsp.h"
#include "nrf_delay.h"
#include "app_pwm.h"


// Create the instance "PWM1" using TIMER1.
APP_PWM_INSTANCE(PWM1, 1);
 

int main(void)
{
    ret_code_t err_code;

    /* 2-channel PWM, 200Hz, output on Buckler LED pins. */
    app_pwm_config_t pwm_config = APP_PWM_DEFAULT_CONFIG_2CH(5000, BUCKLER_LED0, BUCKLER_LED1);

    /* Switch the polarity of the second channel. */
    pwm_config.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;

    /* Initialize PWM. */
    err_code = app_pwm_init(&PWM1, &pwm_config, NULL);
    APP_ERROR_CHECK(err_code);
    /* Enable PWM. */
    app_pwm_enable(&PWM1);

    uint32_t value;
    while (true)
    {
        /* Your code for modifying the duty cycle value */
        for (value = 0; value <= 100; value++)
        {
            err_code = app_pwm_channel_duty_set(&PWM1, 0, value);
            APP_ERROR_CHECK(err_code);
            err_code = app_pwm_channel_duty_set(&PWM1, 1, 100 - value);
            APP_ERROR_CHECK(err_code);
            
            // for catching any errors returned
            err_code = app_pwm_busy_check(&PWM1);
            APP_ERROR_CHECK(err_code);
            nrf_delay_ms(100);
        }
    }

}


/** @} */

