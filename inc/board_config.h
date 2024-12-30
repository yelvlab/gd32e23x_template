//
// Created by dell on 24-12-28.
//

#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

// #define SOFTWARE_IIC

// #define DEBUG_VERBOES

/******************************************************************************/

#define I2C_GPIO_RCU         RCU_GPIOF
#define I2C_RCU              RCU_I2C0
#define I2C_PHY              I2C0
#define I2C_SCL_PORT         GPIOF
#define I2C_SCL_PIN          GPIO_PIN_1
#define I2C_SDA_PORT         GPIOF
#define I2C_SDA_PIN          GPIO_PIN_0
#define I2C_GPIO_AF          GPIO_AF_1

/******************************************************************************/

#define USART_GPIO_RCU       RCU_GPIOA
#define USART_RCU            RCU_USART0
#define USART_GPIO_PORT      GPIOA
#define USART_GPIO_AF        GPIO_AF_1
#define USART_TX_PIN         GPIO_PIN_2
#define USART_RX_PIN         GPIO_PIN_3
#define USART_PHY            USART0
#define USART_PHY_BAUDRATE   115200U
#define RS485_EN_PORT        GPIOA
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
