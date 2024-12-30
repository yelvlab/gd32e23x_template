//
// Created by yelv1 on 24-9-22.
//
#include "peripheral.h"
#include "gd32e23x.h"

void usart_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);

    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);

    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);

    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

    usart_enable(USART0);

    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);

    gpio_bit_write(GPIOA, GPIO_PIN_4, SET);
}

/*!
    \brief      led blink configuration
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_blink_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);

    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_bit_write(GPIOB, GPIO_PIN_1, SET);

    rcu_periph_clock_enable(RCU_TIMER13);
    timer_deinit(RCU_TIMER13);

    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler        =7199;
    timer_initpara.alignedmode      =TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection =TIMER_COUNTER_UP;
    timer_initpara.period           =999;
    timer_initpara.clockdivision    =TIMER_CKDIV_DIV1;
    timer_init(TIMER13, &timer_initpara);

    timer_auto_reload_shadow_enable(TIMER13);
    timer_interrupt_enable(TIMER13, TIMER_INT_UP);
    nvic_irq_enable(TIMER13_IRQn, 0);
    timer_enable(TIMER13);
}
