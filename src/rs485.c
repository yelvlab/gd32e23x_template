//
// Created by dell on 24-12-3.
//

#include "rs485.h"

/**
 * @brief  configure the USART0
 * @param  none
 * @retval none
 */
void usart_config(void) {
    rcu_periph_clock_enable(RS485_GPIO_RCU);
    rcu_periph_clock_enable(RS485_RCU);

    gpio_af_set(RS485_GPIO_PORT, GPIO_AF_1, GPIO_PIN_2 | GPIO_PIN_3);

    /* configure USART Tx&Rx as alternate function push-pull */
    gpio_mode_set(RS485_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, RS485_TX_PIN | RS485_RX_PIN);
    gpio_output_options_set(RS485_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, RS485_TX_PIN | RS485_RX_PIN);

    /* USART configure */
    usart_deinit(RS485_PHY);
    usart_baudrate_set(RS485_PHY, RS485_BAUDRATE);
    usart_receive_config(RS485_PHY, USART_RECEIVE_ENABLE);
    usart_transmit_config(RS485_PHY, USART_TRANSMIT_ENABLE);

    usart_enable(RS485_PHY);

    nvic_irq_enable(USART0_IRQn, 0);
    usart_interrupt_enable(RS485_PHY, USART_INT_RBNE);
    usart_interrupt_enable(RS485_PHY, USART_INT_IDLE);
}

/**
 * @brief  configure the RS485(MAX13487EESA)
 * @param  none
 * @retval none
 */
void rs485_config(void) {
    usart_config();

    rcu_periph_clock_enable(RS485_GPIO_RCU);

    /* configure RS485 EN Pin */
    gpio_mode_set(RS485_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RS485_EN_PIN);
    gpio_output_options_set(RS485_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RS485_EN_PIN);
    gpio_bit_write(RS485_GPIO_PORT, RS485_EN_PIN, SET); // auto direction control
}
