#ifndef __LEDS_AND_BUTTON_H
#define __LEDS_AND_BUTTON_H
#define HSE_VALUE 16000000
#define SWITCH_DELAY 7000000
#define LED1_PIN GPIO_Pin_12
#define LED2_PIN GPIO_Pin_13
#define LED3_PIN GPIO_Pin_14
#define LED4_PIN GPIO_Pin_15
#define RED8_PIN GPIO_Pin_8
#define GREEN9_PIN GPIO_Pin_9
#define BLUE10_PIN GPIO_Pin_10
#define ALL_LED5 (LED1_PIN) | (LED2_PIN) | (LED3_PIN) | (LED4_PIN)
#define ALL_LED6 (RED8_PIN) | (GREEN9_PIN) | (BLUE10_PIN)
#define BUTTON_READLEFT1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)
#define PRESCALER 84
#define PERIOD 1000000
#define BUTTON_DELAY 5000000
#define BUTTON_PIN GPIO_Pin_0

#include "stm32f4xx.h"


#if defined (USE_STM324xG_EVAL)
  #include "stm324xg_eval.h"

#elif defined (USE_STM32F4DISC_EVAL) 
  #include "stm32f4disc_eval.h"
  #include "stm32f4xx_tim.h"
#else
 #error "Please select first the Evaluation board used in your application (in Project Options)"
#endif

#endif
