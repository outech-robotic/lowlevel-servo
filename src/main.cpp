#include "TIMER/tim.h"
#include "CAN/can.h"
#include "GPIO/gpio.h"
#include "UTILITY/Metro.hpp"
#include "UTILITY/timing.h"
#include "USART/usart.hpp"
#include "COM/serial.hpp"
#include <stdio.h>
#include <cstdlib>
#include "UTILITY/macros.h"
#include "config.h"

can_rx_msg rx_msg;
Serial serial;

int main(void)
{
  /**********************************************************************
   *                             SETUP
   **********************************************************************/
  Metro can_wait(100);

  char str[USART_TX_BUFFER_SIZE]="";
  uint16_t data = 0;
  uint16_t servo_id = 0;
  bool servo_selected = false;

  // Initialize timing utility functions (delay, millis...)
  Timing_init();

  // Initialize all peripherals
  MX_CAN_Init();
  MX_TIM3_Init(); // PWM
  //MX_TIM16_Init();
//  MX_USART1_UART_Init();
  serial.init(115200);
  serial.set_timeout(2000);

  serial.printf("Setup done.\r\n");

  /**********************************************************************
   *                             MAIN LOOP
   **********************************************************************/
  while (1)
  {
    str[0]=0;
    if(serial.available()){
      if(serial.read(str, 10)){
        data = atoi(str);
        serial.printf("Recu : %u\r\n", data);
        if(!servo_selected){
          if(!data || data > 3){
            serial.printf("Servo ID peut etre 1, 2 ou 3\r\n");
          }
          else{
            serial.printf("Servo ID : %u\r\n", data);
            servo_id = data;
            servo_selected = true;
          }
        }
        else{
          if(data > 180){
            serial.printf("Entrer angle entre 0 et 180 inclus\r\n");
          }
          else{
            serial.printf("Angle = %u sur servo %u\r\n", data, servo_id);
            switch(servo_id){
              case 1 : PWM_write_angle(PIN_PWM_1, data); break;
              case 2 : PWM_write_angle(PIN_PWM_2, data); break;
              case 3 : PWM_write_angle(PIN_PWM_3, data); break;
            }
          }
          servo_selected = false;
        }
      }
    }
    if((CAN_receive_packet(&rx_msg)) == HAL_OK){
      printf("RECV: ");
      CAN_print_rx_pkt(&rx_msg);
      if(CAN_PKT_MESSAG_ID(rx_msg.header.StdId) == CAN_MSG_SERVO_POS){
        uint8_t servo_id = rx_msg.data.u8[0];
        uint8_t value = rx_msg.data.u8[1];
        serial.printf("SERVO %u POS %u\r\n", servo_id, value);
        uint16_t val_16b = ((uint32_t)value*CONST_PWM_MAX)/255;
        switch(servo_id){
          case 0 : PWM_write(PIN_PWM_1, val_16b); break;
          case 1 : PWM_write(PIN_PWM_2, val_16b); break;
          case 2 : PWM_write(PIN_PWM_3, val_16b); break;
          default: serial.printf("WRONG SERVO ID\r\n"); break;
        }
      }
    }
  }
}
