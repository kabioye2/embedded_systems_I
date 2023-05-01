// Display app
//
// Write messages to a Newhaven OLED display over SPI

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrfx_gpiote.h"
#include "nrf_drv_spi.h"

#include "buckler.h"
#include "display.h"
#include "lsm9ds1.h"
#include "opt3004.h"

#define RAD2DEG 57.13

// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

void display_data(float ax, float ay, float az, float gx, float gy, float gz) {
  char buf1[16];
  char buf2[16];
  char buf3[16];
  char buf4[16];

  snprintf(buf1, 16, "Ax: %.2f Ay:", ax);
  snprintf(buf2, 16, "%.2f Az: %.2f", ay, az);
  display_write(buf1, 0);
  display_write(buf2, 1);
  nrf_delay_ms(1000);

  snprintf(buf3, 16, "Gx: %.2f Gy:", gx);
  snprintf(buf4, 16, "%.2f Gz: %.2f", gy, gz);
  display_write(buf3, 0);
  display_write(buf4, 1);
  nrf_delay_ms(1000); 
}

void display_tilt_angle(float angle) {
  char buf[16];
  char buf1[16];
  char buf2[16];

  snprintf(buf1, 16, "!!! DANGER OF");
  snprintf(buf2, 16, "OVERTURN !!!");
  display_write(buf1, 0);
  display_write(buf2, 1);
  nrf_delay_ms(1000);

  snprintf(buf, 16, "Tilt Angle: %.2f", angle);
  display_write(buf, 0);
  display_write(" ",1);
  nrf_delay_ms(1000);
}

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized\n");

  // initialize spi master(controller)
  nrf_drv_spi_t spi_instance = NRF_DRV_SPI_INSTANCE(1);
  nrf_drv_spi_config_t spi_config = {
    .sck_pin = BUCKLER_LCD_SCLK,
    .mosi_pin = BUCKLER_LCD_MOSI,
    .miso_pin = BUCKLER_LCD_MISO,
    .ss_pin = BUCKLER_LCD_CS,
    .irq_priority = NRFX_SPI_DEFAULT_CONFIG_IRQ_PRIORITY,
    .orc = 0,
    .frequency = NRF_DRV_SPI_FREQ_4M,
    .mode = NRF_DRV_SPI_MODE_2,
    .bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
  };
  error_code = nrf_drv_spi_init(&spi_instance, &spi_config, NULL, NULL);
  APP_ERROR_CHECK(error_code);

  // initialize display driver
  display_init(&spi_instance);
  printf("Display initialized\n");
  nrf_delay_ms(1000);

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);

  // initialize LSM9DS1 driver
  lsm9ds1_init(&twi_mngr_instance);
  printf("lsm9ds1 initialized\n");

  // initialize opt3004 driver
  opt3004_init(&twi_mngr_instance);
  printf("opt3004 initialized: %ld\n", error_code);

  lsm9ds1_start_gyro_integration();

  //sample continuously
  opt3004_continuous();


  while(1) {

/*    // get measurements
    lsm9ds1_measurement_t acc_measurement = lsm9ds1_read_accelerometer();
    lsm9ds1_measurement_t gyr_measurement = lsm9ds1_read_gyro_integration();
    lsm9ds1_measurement_t mag_measurement = lsm9ds1_read_magnetometer();

    // print results
    printf("                      X-Axis\t    Y-Axis\t    Z-Axis\n");
    printf("                  ----------\t----------\t----------\n");
    printf("Acceleration (g): %10.3f\t%10.3f\t%10.3f\n", acc_measurement.x_axis, acc_measurement.y_axis, acc_measurement.z_axis);
    printf("Angle  (degrees): %10.3f\t%10.3f\t%10.3f\n", gyr_measurement.x_axis, gyr_measurement.y_axis, gyr_measurement.z_axis);
    printf("Magnetism   (uT): %10.3f\t%10.3f\t%10.3f\n", mag_measurement.x_axis, mag_measurement.y_axis, mag_measurement.z_axis);
    printf("\n");

    //display on LCD
    display_data(acc_measurement.x_axis, acc_measurement.y_axis, acc_measurement.z_axis, gyr_measurement.x_axis, gyr_measurement.y_axis, gyr_measurement.z_axis);

    float tilt_angle = atan2f(acc_measurement.x_axis, acc_measurement.y_axis) * RAD2DEG;

    // check if tilt angle is greater than 45 degrees
    if (tilt_angle > 45) {
      display_tilt_angle(tilt_angle);
    }
    nrf_delay_ms(100);*/

    //Get light sensor values
    printf("Illuminance (Lux): %.3f\n", opt3004_read_result());
    nrf_delay_ms(100);
  }
}

