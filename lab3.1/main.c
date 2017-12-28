#include "main.h"
#define BUTTON_L GPIO_Pin_0
#define BUTTON_R GPIO_Pin_1
#define LED_RED GPIO_Pin_8
#define LED_GREEN GPIO_Pin_9
#define LED_BLUE GPIO_Pin_10
static int8_t flag;
static int8_t counter;
static int16_t LED[]={LED_RED,LED_GREEN,LED_BLUE};



static
int all_leds1 ()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = ALL_LED6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, ALL_LED6 );

  return 0;


}
static
int button_init ()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitTypeDef GPIO_Init_Button;
  GPIO_Init_Button.GPIO_Pin = BUTTON_L | BUTTON_R;
  GPIO_Init_Button.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init_Button.GPIO_OType = GPIO_OType_PP;
  GPIO_Init_Button.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init_Button.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_Init_Button);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig (EXTI_PortSourceGPIOE,EXTI_PinSource0);
  EXTI_InitTypeDef exti;
  exti.EXTI_Line = EXTI_Line0;
  exti.EXTI_Mode = EXTI_Mode_Interrupt;
  exti.EXTI_Trigger = EXTI_Trigger_Rising;
  exti.EXTI_LineCmd = ENABLE;

  EXTI_Init(&exti);

  NVIC_InitTypeDef nvic;
  nvic.NVIC_IRQChannel = EXTI0_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 0x01;
  nvic.NVIC_IRQChannelSubPriority=0x01;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
  return 0;
 
}

static
int timers(){
    TIM_TimeBaseInitTypeDef timer;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    timer.TIM_Prescaler = 84;
    timer.TIM_CounterMode = TIM_CounterMode_Up;
    timer.TIM_Period = 1000000;
    timer.TIM_ClockDivision = TIM_CKD_DIV1;
    timer.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timer);

    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    TIM_Cmd(TIM2, ENABLE);

    return 0;

    
}

  void TIM2_IRQHandler(void){
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
	TIM_ClearITPendingBit(TIM2 ,TIM_IT_Update);

	GPIO_SetBits(GPIOA,LED[flag]);
	flag = (3+ (flag - counter) %3 ) %3;
	GPIO_ResetBits(GPIOA,LED[flag]);
  }
}
  void EXTI0_IRQHandler(void){
    if(EXTI_GetITStatus(BUTTON_L) != RESET);
	    {
	      EXTI_ClearITPendingBit(BUTTON_L);
	      counter = -counter;
	    }
}
int main(void){
  flag=0;
  counter = 1;
  all_leds1();
  button_init();
  timers();
  while(1){
  }
}
