/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example

    \version 2024-02-22, V2.1.0, firmware for GD32E23x
*/
#include "main.h"

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void) {
    // setbuf(stdout, NULL);
    /* configure systick */
    systick_config();
    /* configure USART */
    // rs485_config();
    /* configure LED */
    // led_config();

    printf("system start!\r\n");

    rcu_periph_clock_enable(RCU_GPIOA);

    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    gpio_bit_write(GPIOA, GPIO_PIN_7, SET);

    while(1){
    printf("hello world!\r\n");
    delay_ms(1000);
    }
}

/* retarget the C library printf function to the USART */
int _write (int fd, char *pBuffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        usart_data_transmit(USART0, (uint8_t)pBuffer[i]);
        while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    }
    return size;
}
