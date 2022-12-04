#include "driver_max7219.h"
#include "driver_max7219_basic.h"
#include "hardware/gpio.h"
#include "hardware/address_mapped.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "boards/adafruit_qtpy_rp2040.h"
#include <stdio.h>

#include "binary.h"
#include "LedControl.h"

#ifndef PICO_DEFAULT_I2C_SDA_PIN
#define PICO_DEFAULT_I2C_SDA_PIN 24
#endif

#ifndef PICO_DEFAULT_I2C_SCL_PIN
#define PICO_DEFAULT_I2C_SCL_PIN 25
#endif

int main() {
    
    // Pins
    const uint sda_pin = PICO_DEFAULT_I2C_SDA_PIN;
    const uint scl_pin = PICO_DEFAULT_I2C_SCL_PIN;

    // Ports
    i2c_inst_t *i2c = i2c1;

    // Buffer to store raw reads
    uint8_t data[8];

    // Initialize chosen serial port
    stdio_init_all();

    //Initialize I2C port at 100 kHz
    i2c_init(i2c, 400 * 1000);

    // Initialize I2C pins
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);


   // max7219_basic_init();
   // max7219_basic_set_matrix(data);

    return 0;
}

