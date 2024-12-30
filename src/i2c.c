//
// Created by dell on 24-12-20.
//

#include "i2c.h"

/*!
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_gpio_config(void) {
    /* enable IIC GPIO clock */
    rcu_periph_clock_enable(I2C_GPIO_RCU);

    /* connect I2C_SCL_PIN to I2C_SCL */
    gpio_af_set(I2C_SCL_PORT, I2C_GPIO_AF, I2C_SCL_PIN);
    /* connect I2C_SDA_PIN to I2C_SDA */
    gpio_af_set(I2C_SDA_PORT, I2C_GPIO_AF, I2C_SDA_PIN);
    /* configure GPIO pins of I2C */
    gpio_mode_set(I2C_SCL_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, I2C_SCL_PIN);
    gpio_output_options_set(I2C_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);
    gpio_mode_set(I2C_SDA_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, I2C_SDA_PIN);
    gpio_output_options_set(I2C_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);
}

/*!
    \brief      configure the I2CX interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_config(void) {
    /* configure I2C GPIO */
    i2c_gpio_config();
    /* enable I2C clock */
    rcu_periph_clock_enable(I2C_RCU);
    /* configure I2C clock */
    i2c_clock_config(I2C_PHY, I2C_SPEED, I2C_DTCY_2);
    /* configure I2C address */
    i2c_mode_addr_config(I2C_PHY, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0xA0);
    /* enable I2CX */
    i2c_enable(I2C_PHY);
    /* enable acknowledge */
    i2c_ack_config(I2C_PHY, I2C_ACK_ENABLE);
}

/*!
    \brief      reset I2C bus
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_bus_reset(void) {
    i2c_deinit(I2C_PHY);
    /* configure SDA/SCL for GPIO */
    GPIO_BC(I2C_SCL_PORT) |= I2C_SCL_PIN;
    GPIO_BC(I2C_SDA_PORT) |= I2C_SDA_PIN;
    gpio_output_options_set(I2C_SCL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);
    gpio_output_options_set(I2C_SDA_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    GPIO_BOP(I2C_SCL_PORT) |= I2C_SCL_PIN;
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    GPIO_BOP(I2C_SDA_PORT) |= I2C_SDA_PIN;
    /* connect I2C_SCL_PIN to I2C_SCL */
    /* connect I2C_SDA_PIN to I2C_SDA */
    gpio_output_options_set(I2C_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);
    gpio_output_options_set(I2C_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);
    /* configure the I2CX interface */
    i2c_config();
}

/**
 * @brief 扫描I2C总线，查找连接的设备
 *
 * 该函数会扫描I2C总线上的所有地址（1到126），并尝试与每个地址进行通信。
 * 如果在某个地址上发现了设备，则会打印出该设备的地址。
 * 最后会打印出找到的设备总数。
 */
void i2c_scan(void) {
    uint32_t timeout;
    uint8_t address;
    int found_devices = 0;

    printf("Scanning I2C bus...\r\n");

    for (address = 1; address < 127; address++) {
        timeout = 0;

        // 生成起始条件
        while (i2c_flag_get(I2C_PHY, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT))
            timeout++;
        if (timeout >= I2C_TIME_OUT) {
            continue; // 超时，跳过该地址
        }
        i2c_start_on_bus(I2C_PHY);
        timeout = 0;

        // 等待起始条件发送完成
        while (!i2c_flag_get(I2C_PHY, I2C_FLAG_SBSEND) && (timeout < I2C_TIME_OUT))
            timeout++;
        if (timeout >= I2C_TIME_OUT) {
            continue; // 超时，跳过该地址
        }
        i2c_master_addressing(I2C_PHY, (address << 1), I2C_TRANSMITTER);
        timeout = 0;

        // 等待地址发送完成
        while (!i2c_flag_get(I2C_PHY, I2C_FLAG_ADDSEND) && (timeout < I2C_TIME_OUT))
            timeout++;
        if (timeout < I2C_TIME_OUT) {
            i2c_flag_clear(I2C_PHY, I2C_FLAG_ADDSEND);
            printf("Found device at 0x%02X\r\n", address);
            found_devices++;
        }

        // 生成停止条件
        i2c_stop_on_bus(I2C_PHY);

        timeout = 0;

        while (i2c_flag_get(I2C_PHY, I2C_FLAG_STPDET) && (timeout < I2C_TIME_OUT))
            timeout++;
    }

    if (found_devices == 0) {
        printf("No I2C devices found.\r\n");
    } else {
        printf("Total %d I2C devices found.\r\n", found_devices);
    }
}

uint8_t i2c_write_16bits(uint8_t slave_addr, uint8_t reg_addr, uint8_t data[2]) {
    uint8_t state = I2C_START;
    uint16_t timeout = 0;
    uint8_t i2c_timeout_flag = 0;

    /* enable acknowledge */
    i2c_ack_config(I2C_PHY, I2C_ACK_ENABLE);
    while (!(i2c_timeout_flag)) {
        switch (state) {
            case I2C_START:
                /* i2c master sends start signal only when the bus is idle */
                while (i2c_flag_get(I2C_PHY, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if (timeout < I2C_TIME_OUT) {
                    i2c_start_on_bus(I2C_PHY);
                    timeout = 0;
                    state = I2C_SEND_ADDRESS;
                } else {
                    timeout = 0;
                    state = I2C_START;
#ifdef DEBUG_VERBOES
                    printf("i2c bus is busy in WRITE BYTE!\n");
#endif
                }
                break;
            case I2C_SEND_ADDRESS:
                /* i2c master sends START signal successfully */
                while ((!i2c_flag_get(I2C_PHY, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if (timeout < I2C_TIME_OUT) {
                    i2c_master_addressing(I2C_PHY, slave_addr, I2C_TRANSMITTER);
                    timeout = 0;
                    state = I2C_CLEAR_ADDRESS_FLAG;
                } else {
                    timeout = 0;
                    state = I2C_START;
#ifdef DEBUG_VERBOES
                    printf("i2c master sends start signal timeout in WRITE BYTE!\n");
#endif
                }
                break;
            case I2C_CLEAR_ADDRESS_FLAG:
                /* address flag set means i2c slave sends ACK */
                while ((!i2c_flag_get(I2C_PHY, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if (timeout < I2C_TIME_OUT) {
                    i2c_flag_clear(I2C_PHY, I2C_FLAG_ADDSEND);
                    timeout = 0;
                    state = I2C_TRANSMIT_DATA;
                } else {
                    timeout = 0;
                    state = I2C_START;
#ifdef DEBUG_VERBOES
                    printf("i2c master clears address flag timeout in WRITE BYTE!\n");
#endif
                }
                break;
            case I2C_TRANSMIT_DATA:
                /* wait until the transmit data buffer is empty */
                while ((!i2c_flag_get(I2C_PHY, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if (timeout < I2C_TIME_OUT) {
                    /* send IIC register address */
                    i2c_data_transmit(I2C_PHY, reg_addr);
                    timeout = 0;
                } else {
                    timeout = 0;
                    state = I2C_START;
#ifdef DEBUG_VERBOES
                    printf("i2c master sends data timeout in WRITE BYTE!\n");
#endif
                }

            /* wait until BTC bit is set */
                while ((!i2c_flag_get(I2C_PHY, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if (timeout < I2C_TIME_OUT) {
                    /* send register MSB value */
                    i2c_data_transmit(I2C_PHY, data[0]);
                    timeout = 0;
                } else {
                    timeout = 0;
                    state = I2C_START;
#ifdef DEBUG_VERBOES
                    printf("i2c master sends MSB data timeout in WRITE BYTE!\n");
#endif
                }

            /* wait until BTC bit is set */
                while ((!i2c_flag_get(I2C_PHY, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if (timeout < I2C_TIME_OUT) {
                    /* send register LSB value */
                    i2c_data_transmit(I2C_PHY, data[1]);
                    timeout = 0;
                    state = I2C_STOP;
                } else {
                    timeout = 0;
                    state = I2C_START;
#ifdef DEBUG_VERBOES
                    printf("i2c master sends LSB data timeout in WRITE BYTE!\n");
#endif
                }

            /* wait until BTC bit is set */
                while ((!i2c_flag_get(I2C_PHY, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if (timeout < I2C_TIME_OUT) {
                    state = I2C_STOP;
                    timeout = 0;
                } else {
                    timeout = 0;
                    state = I2C_START;
#ifdef DEBUG_VERBOES
                    printf("i2c master sends data timeout in WRITE BYTE!\n");
#endif
                }
                break;
            case I2C_STOP:
                /* send a stop condition to I2C bus */
                i2c_stop_on_bus(I2C_PHY);
            /* i2c master sends STOP signal successfully */
                while ((I2C_CTL0(I2C_PHY) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if (timeout < I2C_TIME_OUT) {
                    timeout = 0;
                    state = I2C_END;
                    i2c_timeout_flag = I2C_OK;
                } else {
                    timeout = 0;
                    state = I2C_START;
#ifdef DEBUG_VERBOES
                    printf("i2c master sends stop signal timeout in WRITE BYTE!\n");
#endif
                }
                break;
            default:
                state = I2C_START;
                i2c_timeout_flag = I2C_OK;
                timeout = 0;
#ifdef DEBUG_VERBOES
                printf("i2c master sends start signal in WRITE BYTE.\n");
#endif
                break;
        }
    }
    return I2C_END;
}

uint8_t i2c_read_16bits(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data) {
    uint8_t state = I2C_START;
    uint8_t read_cycle = 0;
    uint16_t timeout = 0;
    uint8_t i2c_timeout_flag = 0;
    uint8_t number_of_byte = 2;

    /* enable acknowledge */
    i2c_ack_config(I2C_PHY, I2C_ACK_ENABLE);

    while (!(i2c_timeout_flag)) {
        switch (state) {
            case I2C_START:
                if (RESET == read_cycle) {
                    /* i2c master sends start signal only when the bus is idle */
                    while (i2c_flag_get(I2C_PHY, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT)) {
                        timeout++;
                    }
                    if (timeout < I2C_TIME_OUT) {
                        /* whether to send ACK or not for the next byte */
                        i2c_ackpos_config(I2C_PHY, I2C_ACKPOS_NEXT);
                    } else {
                        // i2c_bus_reset();
                        timeout = 0;
                        state = I2C_START;
#ifdef DEBUG_VERBOES
                        printf("i2c bus is busy in READ!\n");
#endif
                    }
                }
            /* send the start signal */
                i2c_start_on_bus(I2C_PHY);
                timeout = 0;
                state = I2C_SEND_ADDRESS;
                break;
            case I2C_SEND_ADDRESS:
                /* i2c master sends START signal successfully */
                while ((!i2c_flag_get(I2C_PHY, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if (timeout < I2C_TIME_OUT) {
                    if (RESET == read_cycle) {
                        i2c_master_addressing(I2C_PHY, slave_addr, I2C_TRANSMITTER);
                        state = I2C_CLEAR_ADDRESS_FLAG;
                    } else {
                        i2c_master_addressing(I2C_PHY, slave_addr, I2C_RECEIVER);
                        i2c_ack_config(I2C_PHY, I2C_ACK_DISABLE);
                        state = I2C_CLEAR_ADDRESS_FLAG;
                    }
                    timeout = 0;
                } else {
                    timeout = 0;
                    state = I2C_START;
                    read_cycle = RESET;
#ifdef DEBUG_VERBOES
                    printf("i2c master sends start signal timeout in READ!\n");
#endif
                }
                break;
            case I2C_CLEAR_ADDRESS_FLAG:
                /* address flag set means i2c slave sends ACK */
                while ((!i2c_flag_get(I2C_PHY, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if (timeout < I2C_TIME_OUT) {
                    i2c_flag_clear(I2C_PHY, I2C_FLAG_ADDSEND);
                    if ((SET == read_cycle) && (1 == number_of_byte)) {
                        /* send a stop condition to I2C bus */
                        i2c_stop_on_bus(I2C_PHY);
                    }
                    timeout = 0;
                    state = I2C_TRANSMIT_DATA;
                } else {
                    timeout = 0;
                    state = I2C_START;
                    read_cycle = RESET;
#ifdef DEBUG_VERBOES
                    printf("i2c master clears address flag timeout in READ!\n");
#endif
                }
                break;
            case I2C_TRANSMIT_DATA:
                if (RESET == read_cycle) {
                    /* wait until the transmit data buffer is empty */
                    while ((!i2c_flag_get(I2C_PHY, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT)) {
                        timeout++;
                    }
                    if (timeout < I2C_TIME_OUT) {
                        /* send the EEPROM's internal address to write to : only one byte address */
                        i2c_data_transmit(I2C_PHY, reg_addr);
                        timeout = 0;
                    } else {
                        timeout = 0;
                        state = I2C_START;
                        read_cycle = RESET;
#ifdef DEBUG_VERBOES
                        printf("i2c master wait data buffer is empty timeout in READ!\n");
#endif
                    }
                    /* wait until BTC bit is set */
                    while ((!i2c_flag_get(I2C_PHY, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                        timeout++;
                    }
                    if (timeout < I2C_TIME_OUT) {
                        timeout = 0;
                        state = I2C_START;
                        read_cycle = SET;
                    } else {
                        timeout = 0;
                        state = I2C_START;
                        read_cycle = RESET;
#ifdef DEBUG_VERBOES
                        printf("i2c master sends register address timeout in READ!\n");
#endif
                    }
                } else {
                    while (number_of_byte) {
                        timeout++;
                        if (2 == number_of_byte) {
                            /* wait until BTC bit is set */
                            while (!i2c_flag_get(I2C_PHY, I2C_FLAG_BTC));
                            /* send a stop condition to I2C bus */
                            i2c_stop_on_bus(I2C_PHY);
                        }
                        /* wait until RBNE bit is set */
                        if (i2c_flag_get(I2C_PHY, I2C_FLAG_RBNE)) {
                            /* read a byte from the EEPROM */
                            *data = i2c_data_receive(I2C_PHY);
                            /* point to the next location where the byte read will be saved */
                            data++;
                            /* decrement the read bytes counter */
                            number_of_byte--;
                            timeout = 0;
                        }
                        if (timeout > I2C_TIME_OUT) {
                            timeout = 0;
                            state = I2C_START;
                            read_cycle = 0;
#ifdef DEBUG_VERBOES
                            printf("i2c master sends data timeout in READ!\n");
#endif
                        }
                    }
                    timeout = 0;
                    state = I2C_STOP;
                }
                break;
            case I2C_STOP:
                /* i2c master sends STOP signal successfully */
                while ((I2C_CTL0(I2C_PHY) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if (timeout < I2C_TIME_OUT) {
                    timeout = 0;
                    state = I2C_END;
                    i2c_timeout_flag = I2C_OK;
                } else {
                    timeout = 0;
                    state = I2C_START;
                    read_cycle = 0;
#ifdef DEBUG_VERBOES
                    printf("i2c master sends stop signal timeout in READ!\n");
#endif
                }
                break;
            default:
                state = I2C_START;
                read_cycle = 0;
                i2c_timeout_flag = I2C_OK;
                timeout = 0;
#ifdef DEBUG_VERBOES
                printf("i2c master sends start signal in READ.\n");
#endif
                break;
        }
    }
    return I2C_END;
}
