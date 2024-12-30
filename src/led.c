//
// Created by yelv1 on 24-12-30.
//

#include "led.h"

/*!
    \brief      led blink configuration
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_blink_config(void)
{
    rcu_periph_clock_enable(LED_RCU);

    gpio_mode_set(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
    gpio_output_options_set(LED_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, LED_PIN);
    gpio_bit_write(LED_PORT, LED_PIN, SET);

    rcu_periph_clock_enable(LED_BLINK_TIMER_RCU);
    timer_deinit(LED_BLINK_TIMER);

    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler        =7199;
    timer_initpara.alignedmode      =TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection =TIMER_COUNTER_UP;
    timer_initpara.period           =9999;
    timer_initpara.clockdivision    =TIMER_CKDIV_DIV1;
    timer_init(LED_BLINK_TIMER, &timer_initpara);

    timer_auto_reload_shadow_enable(LED_BLINK_TIMER);
    timer_interrupt_enable(LED_BLINK_TIMER, TIMER_INT_UP);

    timer_enable(LED_BLINK_TIMER);

    nvic_irq_enable(LED_BLINK_IRQ, 2);
}