#include "iwdg.h"

//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//rlr:�Զ���װ��ֵ,0~0XFFF.
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).

//IWDG_Init(4,100); //���Ƶ��Ϊ64,����ֵΪ100,���ʱ��Ϊ200ms	
//IWDG_Init(4,500); //���Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ1s	
//IWDG_Init(6,500); //���Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ4s
//IWDG_Init(6,1000); //���Ƶ��Ϊ64,����ֵΪ1000,���ʱ��Ϊ8s
//IWDG_Init(6,2000); //���Ƶ��Ϊ64,����ֵΪ2000,���ʱ��Ϊ16s
void IWDG_Init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶ�IWDG->PR IWDG->RLR��д
	IWDG_SetPrescaler(6); //����IWDG��Ƶϵ��
	IWDG_SetReload(1000);  //����IWDGװ��ֵ
	IWDG_ReloadCounter(); //reload
	IWDG_Enable();        //ʹ�ܿ��Ź�
}
//ι�������Ź�
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();//reload
}
