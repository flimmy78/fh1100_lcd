//////////////////////////////////////////////////////////////////////////////////	 
//��������:2016/4/13
//�汾��V1.0
//zx							  
////////////////////////////////////////////////////////////////////////////////// 	

#ifndef __LED_H
#define __LED_H
#include "sys.h"
//LED�˿ڶ���
#define LED1  PCout(15)		// 	1����
#define LED2  PCout(14)		// 	2����
#define LED3  PCout(13)		// 	3�澯
#define LED4  PEout(6)		// 	4ͬ��
#define LED5  TIM_Cmd(TIM9, ENABLE)
#define LED6  PEout(4)		// 	6����
#define LED7  PEout(3)		// 	7GPS
#define LED8  PEout(2)		// 	8IRIG-B1
#define LED9  PBout(5)		// 	9IRIG-B2
#define LED10 PEout(1)		// 10_1
#define LED11 PEout(0)		// 10_2
#define LED12 PBout(9)		// 11PTP
void LED_Init(void);//��ʼ��		 			
#endif
