#include "TIMER/tim.h"
#include "CAN/can.h"
#include "GPIO/gpio.h"
#include "UTILITY/Metro.hpp"
#include "UTILITY/timing.h"
#include "USART/usart.hpp"
#include "stdio.h"
#include "UTILITY/macros.h"
#include "config.h"
#ifdef CARTE_MOTEUR
#include "MOTION/MotionController.h"
#endif

bool pwm_state = false; // PWM test state

volatile uint32_t mesure_t_irq = 0;
uint32_t mesure_t_irq_last = 0;

can_rx_msg rx_msg;

#ifdef CARTE_MOTEUR
MotionController mcs;
#endif

int printf (const char *__restrict, ...){
  return 0;
}
int main(void)
{
  /**********************************************************************
   *                             SETUP
   **********************************************************************/
  Metro can_wait(2000);
  // Initialize timing utility functions (delay, millis...)
  Timing_init();

  // Initialize all peripherals
  MX_CAN_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM16_Init();
  MX_USART1_UART_Init();
#ifdef CARTE_MOTEUR
  MX_TIM14_Init();
  mcs.init();
#endif
  printf("Setup done.\r\n");

  /**********************************************************************
   *                             MAIN LOOP
   **********************************************************************/
  while (1)
  {
    if((CAN_receive_packet(&rx_msg)) == HAL_OK){
      printf("RECV: ");
      CAN_print_rx_pkt(&rx_msg);
      if(CAN_PKT_MESSAG_ID(rx_msg.header.StdId) == CAN_MSG_SERVO_POS){
        uint8_t servo_id = rx_msg.data.u8[0];
        uint8_t value = rx_msg.data.u8[1];
        printf("SERVO %u POS %u\r\n", servo_id, value);
        uint16_t val_16b = ((uint32_t)value*CONST_PWM_MAX)/255;
        switch(servo_id){
          case 0 : PWM_write(PIN_PWM_L_FIN, val_16b); break;
          case 1 : PWM_write(PIN_PWM_L_RIN, val_16b); break;
          case 2 : PWM_write(PIN_PWM_R_FIN, val_16b); break;
          case 3 : PWM_write(PIN_PWM_R_RIN, val_16b); break;
          default: printf("WRONG SERVO ID\r\n"); break;
        }
      }
    }
    if(can_wait.check()){
      if(mesure_t_irq != mesure_t_irq_last){
        printf("T IRQ %lu\r\n", mesure_t_irq);
        mesure_t_irq_last = mesure_t_irq;
      }
#ifdef CARTE_MOTEUR
      if(pwm_state){
        PWM_write(PIN_PWM_L_FIN, 1000);
        PWM_write(PIN_PWM_L_RIN, 0);
        PWM_write(PIN_PWM_R_FIN, 1000);
        PWM_write(PIN_PWM_R_RIN, 0);
      }
      else{
        PWM_write(PIN_PWM_L_FIN, 0);
        PWM_write(PIN_PWM_L_RIN, 1000);
        PWM_write(PIN_PWM_R_FIN, 0);
        PWM_write(PIN_PWM_R_RIN, 1000);
      }
      pwm_state=!pwm_state;
      if((CAN_send_encoder_pos(mcs.get_COD_left(), mcs.get_COD_right())) != CAN_ERROR_STATUS::CAN_PKT_OK){
        printf("ERR: SENDING ENCODER\r\n");
      }
#endif
    }
  }
}

#ifdef CARTE_MOTEUR
#ifdef __cplusplus
extern "C"{
#endif
void TIM14_IRQHandler(void){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM14)){
		LL_TIM_ClearFlag_UPDATE(TIM14);
		mesure_t_irq = micros();
//    mcs.update();
//    mcs.control();
		mesure_t_irq = micros()-mesure_t_irq;
	}
}
#ifdef __cplusplus
}
#endif
#endif
