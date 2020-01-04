/*
 * config.h
 *
 *  Created on: 4 déc. 2019
 *      Author: Tic-Tac
 */

#ifndef UTILITY_CONFIG_H_
#define UTILITY_CONFIG_H_

#define MAKE_MASK(length) ((1<<length)-1)
/*
 * MOTOR CONTROL
 */
// TIMER PWM
#define CONST_PWM_PRESCALER  240 // 5us resolution : 48MHz/240
#define CONST_PWM_AUTORELOAD 4000 // 20ms PWM period : 4000*5us
#define CONST_PWM_REPETITION 1
#define CONST_PWM_MAX        4000
#define CONST_PWM_PERIOD_US  20000

/*
 * COMMUNICATIONS
 */
//CAN CONFIG
#define CONST_CAN_MODE_LOOPBACK
#define CONST_CAN_SPEED_100K
//#define CONST_CAN_SPEED_1M

//USART CONFIG
#define CONST_USART_BAUDRATE (9600)

//BUFFER SIZE USED IN ISR
#define CONST_CAN_BUFFER_SIZE (16)

//CAN IDS
#define CAN_PIPE_WIDTH        (2)
#define CAN_MESSAGE_WIDTH     (9)
#define CAN_PIPE_MASK         (MAKE_MASK(CAN_PIPE_WIDTH)<<CAN_MESSAGE_WIDTH)
#define CAN_MESSAGE_MASK       MAKE_MASK(CAN_MESSAGE_WIDTH)
#define CAN_STDID_SHIFT       (5)

//PIPE IDs
#define CAN_PIPE_MOTOR       (0b00)
#define CAN_PIPE_HL          (0b01)
#define CAN_PIPE_SENSOR      (0b10)
#define CAN_PIPE_SERVO       (0b11)

#define CAN_BOARD_ID         (0b00000) // used with in message id for sensor or servo messages (5bits out of 9)
#define CAN_BOARD_ID_WIDTH   (5)
#define CAN_BOARD_ID_MASK     MAKE_MASK(CAN_BOARD_ID_WIDTH)

//MESSAGE IDs
//PROPULSION MESSAGES
#define CAN_MSG_MOT_STOP        (0b000000)
#define CAN_MSG_MOT_MOVE_END    (0b000001)
#define CAN_MSG_MOT_MOVE        (0b000010)
#define CAN_MSG_MOT_COD_POS     (0b000011)
//HL MESSAGES
#define CAN_MSG_HEARTBEAT       (0b101010)
//SERVO MESSAGES
#define CAN_MSG_SERVO_POS       (0b0000)
#define CAN_MSG_SERVO_POS_WIDTH (4)


/**
 * Project Specific Pins
 */
// ENCODERS
#define PIN_COD_L_A PB4
#define PIN_COD_L_B PB5
#define PIN_COD_R_A PA15
#define PIN_COD_R_B PB3

// CAN BUS
#define PIN_CAN_RX PA11
#define PIN_CAN_TX PA12

// DEBUG USART1 PORT
#define PIN_USART1_TX PA9
#define PIN_USART1_RX PA10

// PA6 - TIM3_CH1
// PA7 - TIM3_CH2
// PB0 - TIM3_CH3
#define PIN_PWM_1 PA6
#define PIN_PWM_2 PA7
#define PIN_PWM_3 PB0

#endif /* UTILITY_CONFIG_H_ */
