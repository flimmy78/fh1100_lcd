#include "includes.h"
#include "hadware.h"
#include "mainmenu.h"
#include "flash_conf.h"
void bsp_init(void)
{
	int i;
	uint32_t readbuff[3];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	delay_init(168);  	//ʱ�ӳ�ʼ��
	uart_init(115200);    //���ڲ���������
	LED_Init();           //LED��ʼ��
	//������������Դ����
	LED11=0;
	TFTLCD_Init();
	W25QXX_Init();				//��ʼ��W25Q128
	bsp_InitKey(); 	
  CAN1_Mode_Init();     //������1M CAN��ʼ��	
	USERPASS = 0001;
	#if defined PPSTICK__RTC
		My_RTC_Init();		 		//��ʼ��RTC
		RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//����WAKE UP�ж�,1�����ж�һ��
		RTC_WakeUpCmd( ENABLE);//����WAKE UP ��ʱ��
	#endif
	#if defined WATCHDOG_USE
		IWDG_Init();
	#endif
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);//����CRCʱ��
	/*
	 װ�õĳ�ʼ��״̬
	*/
	memset(&DevHead,0,sizeof(__DEV_HEAD)); 		//�ṹ��������0
	memset(&DevSys,0,sizeof(__RUNSYS)); 		//�ṹ��������0
	DevHead.tb       = STA_ERR;
	DevHead.synflag  = STA_ERR;
	DevHead.bds      = STA_ERR;
	DevHead.gps      = STA_ERR;
	DevHead.b1       = STA_ERR;
	DevHead.b2 		   = STA_ERR;
	DevHead.monitor1 = STA_ERR;
	//������������Դ����
	//DevHead.monitor2 = STA_ERR;	
	DevHead.ptp1     = STA_ERR;
	DevHead.ptp2     = STA_ERR;
  FLASH_Read(FLASH_SAVE_ADDR,readbuff,3);//������Ϣ��readflash��
	if( readbuff[0] != 0x55 )//����ֵ
	 {
		 readbuff[0] = 0x55;
		 readbuff[1] = 1;
		 readbuff[2] = 0;
		 DevSys.LCD_Protect_ON =  readbuff[1];
		 DevSys.USER_PASS   =  readbuff[2];
		 FLASH_Write(FLASH_SAVE_ADDR,readbuff,3);//д�û�
		 for(i=0;i<1000;i++){}//�ȴ���д����		 
	 }
	else
	 {
		 DevSys.LCD_Protect_ON =  readbuff[1];
		 DevSys.USER_PASS   =  readbuff[2];
		 myprintf("readbuff[3]={h%x,%d,,%d}",readbuff[0],readbuff[1],readbuff[2]);
	 }
}


