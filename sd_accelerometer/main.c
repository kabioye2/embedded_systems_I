// Analog accelerometer app
//
// Reads data from the ADXL327 analog accelerometer

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>  // needed for tilt angle calculation


#include "timestamp.h"

#define RAD2DEG 57.13 

// ADC channels
#define X_CHANNEL 0
#define Y_CHANNEL 1
#define Z_CHANNEL 2


float timestamp=0.0;



// callback for SAADC events
void saadc_callback (nrfx_saadc_evt_t const * p_event) {
  // don't care about adc callbacks
}

// sample a particular analog channel in blocking mode
 nrf_saadc_value_t sample_value (uint8_t channel) {
   nrf_saadc_value_t val;
   ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
   APP_ERROR_CHECK(error_code);
   return val;
 }

 float get_inclination_angle(float x, float y, float z, int axis) {
  float inclination_angle;
  if (axis == 1){
    inclination_angle = atan2(x, sqrt((y*y)+(z*z)))*RAD2DEG;
  }else if (axis == 2){
    inclination_angle = atan2(y, sqrt((x*x)+(z*z)))*RAD2DEG;
  }else if (axis == 3){
    inclination_angle= atan2(sqrt((x*x)+(y*y)),z)*RAD2DEG;
  }
    return inclination_angle;
}


int main (void) {

  ret_code_t error_code = NRF_SUCCESS;

  // // initialize RTT ENDS_INIT();

  // initialize analog to digital converter
  nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
  saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
  error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
  APP_ERROR_CHECK(error_code);

  // // initialize analog inputs
  // // configure with 0 as input pin for now
  nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
  channel_config.gain = NRF_SAADC_GAIN1_6; // input gain of 1/6 Volts/Volt, multiply incoming signal by (1/6)
  channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // 0.6 Volt reference, input after gain can be 0 to 0.6 Volts

  // // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_X;
  error_code = nrfx_saadc_channel_init(X_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Y;
  error_code = nrfx_saadc_channel_init(Y_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Z;
  error_code = nrfx_saadc_channel_init(Z_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // initialization complete
  printf("Buckler initialized!\n");

  
  
  ////////////////////

  // calibration of the voltage output from the ADC

  float LSB = 0.000879; // 3.6V/ 2^12 -- where 3.6V = 0.6/(1/6) 
  float supplyV = 2.91;  // Volts
  float vx = 0.0;
  float vy = 0.0; 
  float vz = 0.0;
  
  float ax, ay, az;

  float mx = .42/3 * supplyV;
  float my = .42/3 * supplyV;
  float mz = .42/3 * supplyV;

  float bx = 1.43;// from the observation//1.5/3 * supplyV;
  float by = 1.45;// from the observation//1.5/3 * supplyV;
  float bz = 1.48;// from the observation //1.5/3 * supplyV;


  // tilt variables

  float phi = 0.0; // angle about x-axis
  float theta = 0.0;  // angle about z-axis
  float psi = 0.0; // angle about y-axis


   nrf_saadc_value_t x_val = sample_value(X_CHANNEL);
    nrf_saadc_value_t y_val = sample_value(Y_CHANNEL);
    nrf_saadc_value_t z_val = sample_value(Z_CHANNEL);
  
  // Initialize the SD Card for logging data in TESTFILE.csv
  init_SDCard();


  // loop forever
  while (1) {
    // sample analog inputs
   x_val = sample_value(X_CHANNEL);
   y_val = sample_value(Y_CHANNEL);
   z_val = sample_value(Z_CHANNEL);

    vx = LSB*x_val;
    vy = LSB*y_val;
    vz = LSB*z_val;

	// Calculation of the acceleration from the calibrated voltage output of the accelerometer
    ax = (vx - bx)/mx;
    ay = (vy - by)/my;
    az = (vz - bz)/mz;
    // display results

    //Tilt/inclination angle
    phi = get_inclination_angle(ax,ay,az,3);
    theta = get_inclination_angle(ax,ay,az,1);
    psi = get_inclination_angle(ax,ay,az,2);
     
    printf("a_x: %f\ta_y: %f\ta_z: %f\n", ax, ay, az);
    printf("phi: %f\ttheta: %f\tpsi: %f\n", phi, theta, psi);
	    
      if (!gpio_read(BUCKLER_BUTTON0)) {
       
        timestamp = get_timestamp();      
        simple_logger_log("%f,%f,%f,%f,%f,%f,%f\n", timestamp, ax, ay, az, phi, theta, psi);
      
        printf("%f - Wrote line to SD card\n", timestamp);
        // Signal that lines were written
        
        gpio_clear(BUCKLER_LED0);      
        nrf_delay_ms(100);
      }
      else
        gpio_set(BUCKLER_LED0);  
  }   
}


