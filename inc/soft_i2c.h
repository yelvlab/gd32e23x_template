//
// Created by dell on 24-12-28.
//

#ifndef SOFT_I2C_H
#define SOFT_I2C_H

#include "gd32e23x_it.h"
#include "gd32e23x.h"
#include "systick.h"
#include "main.h"

#include "board_config.h"

/******************************************************************************/

#define I2C_SCL_HIGH()      gpio_bit_set(I2C_SCL_PORT, I2C_SCL_PIN)
#define I2C_SCL_LOW()       gpio_bit_reset(I2C_SCL_PORT, I2C_SCL_PIN)
#define I2C_SDA_HIGH()      gpio_bit_set(I2C_SDA_PORT, I2C_SDA_PIN)
#define I2C_SDA_LOW()       gpio_bit_reset(I2C_SDA_PORT, I2C_SDA_PIN)
#define I2C_SDA_READ()      gpio_input_bit_get(I2C_SDA_PORT, I2C_SDA_PIN)

/******************************************************************************/

#define SOFT_I2C_OK          1
#define SOFT_I2C_FAIL        0
#define SOFT_I2C_END         1

/******************************************************************************/

void soft_i2c_delay(void);

void soft_i2c_config(void);

void soft_i2c_start(void);

void soft_i2c_stop(void);

void soft_i2c_send_ack(void);

void soft_i2c_send_nack(void);

uint8_t soft_i2c_wait_ack(void);

void soft_i2c_send_byte(uint8_t data);

uint8_t soft_i2c_receive_byte(uint8_t ack);

uint8_t soft_i2c_write_16bits(uint8_t slave_addr, uint8_t reg_addr, uint8_t data[2]);

uint8_t soft_i2c_read_16bits(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data);

#endif //SOFT_I2C_H
