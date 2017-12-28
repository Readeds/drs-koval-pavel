#include "main.h"

void All_led5 ()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = ALL_LED5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);


}
void All_led6 ()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = ALL_LED6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


}

void Button_inil (void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitTypeDef GPIO_Init_Button;
  GPIO_Init_Button.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init_Button.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init_Button.GPIO_OType = GPIO_OType_PP;
  GPIO_Init_Button.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init_Button.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_Init_Button);
 
}
void init_Timer (){

  RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2,ENABLE);
  TIM_TimeBaseInitTypeDef tim_struct;
  tim_struct.TIM_Period = PERIOD -1;
  tim_struct.TIM_Prescaler = PRESCALER -1;
  tim_struct.TIM_ClockDivision =0;
  tim_struct.TIM_CounterMode =TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2,&tim_struct);
  TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
}
int main(void)
{
  //SystemInit();
  Button_inil();
  init_Timer();
  All_led5();
  All_led6();
  long ledArr[]= {~GPIO_Pin_8,~GPIO_Pin_9,~GPIO_Pin_10};
  long ledArrD[]={GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15};

 
  int numA=0;
  int numD=0;
  int order =1;
  int delay =0;
  while (1)
  {
    int i;
    // GPIO_SetBits(GPIOD,ALL_LED5);
    // GPIO_SetBits (GPIOD,LED1_PIN);
	 
	  if(!TIM_GetCounter(TIM2))
	    {
	      GPIO_ResetBits(GPIOA, ledArr[numA]);
	      GPIO_ResetBits(GPIOD, ledArrD[numD]);
	      numA=(numA+order+3)%3;
	      numD=(numD+order+4)%4;
	      GPIO_SetBits(GPIOA,ledArr[numA]);
	      GPIO_SetBits(GPIOD,ledArrD[numD]);
	      /////
	    
	        
	    }

	  if(!GPIO_ReadInputDataBit(GPIOE, BUTTON_PIN) && !delay)
	    {
	      order =-order;
	      delay = BUTTON_DELAY;
	    }
	  if(delay>0){
	    delay--;
	  }
    }
}
