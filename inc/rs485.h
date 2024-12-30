//
// Created by dell on 24-12-3.
//

#ifndef RS485_H
#define RS485_H

#include "gd32e23x_it.h"
#include "gd32e23x.h"
#include "board_config.h"

/******************************************************************************/

#define RX_BUFFER_SIZE            32

/******************************************************************************/

void usart_config(void);

void rs485_config(void);

#endif //RS485_H
