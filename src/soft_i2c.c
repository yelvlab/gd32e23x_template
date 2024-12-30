//
// Created by dell on 24-12-28.
//

#include "soft_i2c.h"

/*!
    \brief      delay
    \param[in]  none
    \param[out] none
    \retval     none
*/
void soft_i2c_delay(void) {
    delay_us(20); // Adjust delay as needed
    /* delay to freq
     *  15KHz: delay_us(20);
     *  65KHz: delay_us(1);
     */
}

/*!
    \brief      configure the software IIC GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
void soft_i2c_config(void) {
    rcu_periph_clock_enable(I2C_GPIO_RCU);

    gpio_mode_set(I2C_SCL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, I2C_SCL_PIN);
    gpio_output_options_set(I2C_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);

    gpio_mode_set(I2C_SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, I2C_SDA_PIN);
    gpio_output_options_set(I2C_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);

    I2C_SCL_HIGH();
    I2C_SDA_HIGH();
}

/*!
    \brief      generate I2C start signal
    \param[in]  none
    \param[out] none
    \retval     none
*/
void soft_i2c_start(void) {
    I2C_SDA_HIGH();
    I2C_SCL_HIGH();
    soft_i2c_delay();
    I2C_SDA_LOW();
    soft_i2c_delay();
    I2C_SCL_LOW();
}

/*!
    \brief      generate I2C stop signal
    \param[in]  none
    \param[out] none
    \retval     none
*/
void soft_i2c_stop(void) {
    // sda_out();
    I2C_SCL_LOW();
    I2C_SDA_LOW();
    soft_i2c_delay();
    I2C_SCL_HIGH();
    soft_i2c_delay();
    I2C_SDA_HIGH();
}

/*!
    \brief      send I2C ACK signal
    \param[in]  none
    \param[out] none
    \retval     none
*/
void soft_i2c_send_ack(void) {
    // sda_out();
    I2C_SDA_LOW();
    soft_i2c_delay();
    I2C_SCL_HIGH();
    soft_i2c_delay();
    I2C_SCL_LOW();
    soft_i2c_delay();
    I2C_SDA_HIGH();
}

/*!
    \brief      send I2C NACK signal
    \param[in]  none
    \param[out] none
    \retval     none
*/
void soft_i2c_send_nack(void) {
    I2C_SDA_HIGH();
    soft_i2c_delay();
    I2C_SCL_HIGH();
    soft_i2c_delay();
    I2C_SCL_LOW();
    soft_i2c_delay();
    I2C_SDA_HIGH();
}

/*!
    \brief      wait I2C ACK signal
    \param[in]  none
    \param[out] none
    \retval     0: ACK received, 1: ACK not received
*/
uint8_t soft_i2c_wait_ack(void) {
    I2C_SDA_HIGH();
    soft_i2c_delay();
    I2C_SCL_HIGH();
    soft_i2c_delay();
    uint8_t ack = !I2C_SDA_READ();
    I2C_SCL_LOW();
    return ack;
}

/*!
    \brief      send a byte via I2C
    \param[in]  byte: byte to be sent
    \param[out] none
    \retval     none
*/
void soft_i2c_send_byte(uint8_t byte) {
    // sda_out();
    for (int i = 0; i < 8; i++) {
        if (byte & 0x80) {
            I2C_SDA_HIGH();
        } else {
            I2C_SDA_LOW();
        }
        byte <<= 1;
        soft_i2c_delay();
        I2C_SCL_HIGH();
        soft_i2c_delay();
        I2C_SCL_LOW();
        soft_i2c_delay();
    }
}

/*!
    \brief      receive a byte via I2C
    \param[in]  ack: 1: send ACK, 0: send NACK
    \param[out] none
    \retval     received byte
*/
uint8_t soft_i2c_receive_byte(uint8_t ack) {
    uint8_t byte = 0;
    I2C_SDA_HIGH();
    for (int i = 0; i < 8; i++) {
        byte <<= 1;
        I2C_SCL_HIGH();
        soft_i2c_delay();
        if (I2C_SDA_READ()) {
            byte |= 0x01;
        }
        I2C_SCL_LOW();
        soft_i2c_delay();
    }
    if (ack) {
        soft_i2c_send_ack();
    } else {
        soft_i2c_send_nack();
    }
    return byte;
}

uint8_t soft_i2c_write_16bits(uint8_t slave_addr, uint8_t reg_addr, uint8_t data[2]) {
    soft_i2c_start();
    soft_i2c_send_byte(slave_addr);
    if (!soft_i2c_wait_ack()) {
        soft_i2c_stop();
        return SOFT_I2C_FAIL;
    }
    soft_i2c_send_byte(reg_addr);
    if (!soft_i2c_wait_ack()) {
        soft_i2c_stop();
        return SOFT_I2C_FAIL;
    }
    soft_i2c_send_byte(data[0]);
    if (!soft_i2c_wait_ack()) {
        soft_i2c_stop();
        return SOFT_I2C_FAIL;
    }
    soft_i2c_send_byte(data[1]);
    if (soft_i2c_wait_ack()){}
    soft_i2c_stop();
    return SOFT_I2C_OK;
}

uint8_t soft_i2c_read_16bits(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data)
{
    soft_i2c_start();
    soft_i2c_send_byte(slave_addr);
    if (!soft_i2c_wait_ack()) {
        soft_i2c_stop();
        return SOFT_I2C_FAIL;
    }
    soft_i2c_send_byte(reg_addr);
    if (!soft_i2c_wait_ack()) {
        soft_i2c_stop();
        return SOFT_I2C_FAIL;
    }
    soft_i2c_start();
    soft_i2c_send_byte(slave_addr | 0x01);
    if (!soft_i2c_wait_ack()) {
        soft_i2c_stop();
        return SOFT_I2C_FAIL;
    }
    soft_i2c_delay();
    data[0] = soft_i2c_receive_byte(1);
    data[1] = soft_i2c_receive_byte(0);
    soft_i2c_stop();
    return SOFT_I2C_OK;
}