#include "main.h"


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

int main(void)
{
  
  All_led6();
  long ledArr[]= {~GPIO_Pin_8,~GPIO_Pin_9,~GPIO_Pin_10};
 

 
  int numA=0;
  
  while (1)
  {
    int i;
   
	 
	  
	      GPIO_ResetBits(GPIOA, ledArr[numA%3]);
	     
	      numA++;
	      
	      GPIO_SetBits(GPIOA,ledArr[numA%3]);
	      for (i=0;i < SWITCH_DELAY;i++)
		{
		}
	      GPIO_ResetBits(GPIOA, ledArr[numA%3]);
	     
	      numA++;
	      
	      GPIO_SetBits(GPIOA,ledArr[numA%3]);
	      for (i=0;i < SWITCH_DELAY;i++)
		{
		}
  }
}
