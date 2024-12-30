//
// Created by dell on 24-12-20.
//

#ifndef I2C_H
#define I2C_H

#include "gd32e23x_it.h"
#include "gd32e23x.h"
#include "systick.h"
#include "main.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "board_config.h"

/******************************************************************************/

#define I2C_SPEED       20000

#define I2C_TIME_OUT    (uint16_t)(5000)
#define I2C_OK          1
#define I2C_FAIL        0
#define I2C_END         1

/******************************************************************************/

typedef enum {
    I2C_START = 0,
    I2C_SEND_ADDRESS,
    I2C_CLEAR_ADDRESS_FLAG,
    I2C_TRANSMIT_DATA,
    I2C_STOP
} i2c_process_enum;

/******************************************************************************/

void i2c_gpio_config(void);

void i2c_config(void);

void i2c_bus_reset(void);

void i2c_scan(void);

uint8_t i2c_write_16bits(uint8_t slave_addr, uint8_t reg_addr, uint8_t data[2]);

uint8_t i2c_read_16bits(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data);

#endif //I2C_H
