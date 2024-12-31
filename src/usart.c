//
// Created by yelv1 on 24-12-30.
//

#include "usart.h"

/**
 * @brief  configure the USART
 * @param  none
 * @retval none
 */
void usart_config(void)
{
    rcu_periph_clock_enable(USART_GPIO_RCU);
    rcu_periph_clock_enable(USART_RCU);

    gpio_af_set(USART_GPIO_PORT, USART_GPIO_AF, USART_RX_PIN);
    gpio_af_set(USART_GPIO_PORT, USART_GPIO_AF, USART_TX_PIN);

    gpio_mode_set(USART_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_RX_PIN);
    gpio_output_options_set(USART_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, USART_RX_PIN);
    gpio_mode_set(USART_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_TX_PIN);
    gpio_output_options_set(USART_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, USART_TX_PIN);

    usart_deinit(USART_PHY);
    usart_baudrate_set(USART_PHY, USART_PHY_BAUDRATE);
    usart_receive_config(USART_PHY, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART_PHY, USART_TRANSMIT_ENABLE);

    usart_enable(USART_PHY);

    nvic_irq_enable(USART0_IRQn, 0);
    usart_interrupt_enable(USART_PHY, USART_INT_RBNE);
    usart_interrupt_enable(USART_PHY, USART_INT_IDLE);
}

/**
 * @brief  configure the RS485(MAX13487) driver
 * @param  none
 * @retval none
 */
void rs485_config(void)
{
    usart_config();

    gpio_mode_set(RS485_EN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RS485_EN_PIN);
    gpio_output_options_set(RS485_EN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RS485_EN_PIN);

    gpio_bit_write(RS485_EN_PORT, RS485_EN_PIN, SET); //auto dircetion control
}