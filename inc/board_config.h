//
// Created by dell on 24-12-28.
//

#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#define SOFTWARE_IIC

// #define DEBUG_VERBOES

/******************************************************************************/

#define RCU_GPIO_I2C         RCU_GPIOF
#define RCU_I2C              RCU_I2C0
#define I2C_SCL_PORT         GPIOF
#define I2C_SCL_PIN          GPIO_PIN_1
#define I2C_SDA_PORT         GPIOF
#define I2C_SDA_PIN          GPIO_PIN_0
#define I2C_GPIO_AF          GPIO_AF_1

/******************************************************************************/

#define RS485_RCU            RCU_USART0
#define RS485_GPIO_RCU       RCU_GPIOA
#define RS485_GPIO_PORT      GPIOA
#define RS485_TX_PIN         GPIO_PIN_2
#define RS485_RX_PIN         GPIO_PIN_3
#define RS485_PHY            USART0
#define RS485_BAUDRATE       115200U
#define RS485_EN_PIN         GPIO_PIN_1

/******************************************************************************/

#define LED_PORT             GPIOA
#define LED_PIN              GPIO_PIN_7
#define LED_RCU              RCU_GPIOA
#define LED_BLINK_TIMER_RCU  RCU_TIMER16
#define LED_BLINK_TIMER      TIMER16
#define LED_BLINK_IRQ        TIMER16_IRQn

/******************************************************************************/

#endif //BOARD_CONFIG_H
