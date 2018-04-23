//////////////////////////////////////////////////////////////////////////////////	 
//��������:2016/5/26
//�汾��V1.0
//ZX						  
//�޸ļ�¼��1��2016.06.21�޸Ľ����������ΪCAN1_RX1_STA&0xffff0000	
////////////////////////////////////////////////////////////////////////////////// 
#include "can.h"
#include "delay.h"
#include "includes.h"
//bit15��	������ɱ�־
//
//bit13~0��	���յ�����Ч�ֽ���Ŀ	
u8 CAN1_RX0_BUF[CAN1_RX0_LEN]={0}; 	//CAN����BUF,���CAN1_RX0_LEN���ֽ�.
u32 CAN1_RX0_STA=0;       			//����״̬���	
u8 CAN1_RX1_BUF[CAN1_RX1_LEN]={0}; 	//CAN����BUF,���CAN1_RX1_LEN���ֽ�.
u32 CAN1_RX1_STA=0;       			//����״̬���	
extern OS_SEM       SEM_SYNCH;	    /* ����ͬ�� */
 /**
  * @brief  CAN1��ʼ��
  * @param  ��
  * @retval ��ʼ���ɹ�->0
  */
u8 CAN1_Mode_Init(void)
{
  	GPIO_InitTypeDef 				GPIO_InitStructure; 
	  CAN_InitTypeDef       	CAN_InitStructure;
  	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;

   	NVIC_InitTypeDef  NVIC_InitStructure;

    //ʹ�����ʱ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PORTBʱ��	    
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
    //��ʼ��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); 
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); 

		CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ��
		CAN_InitStructure.CAN_ABOM=DISABLE;			   //MCR-ABOM  �ر��Զ����߹��� 
		CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  �ر��Զ�����ģʽ
		CAN_InitStructure.CAN_NART=ENABLE;			   //MCR-NART  ���������Զ��ش�	  
		CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  ����FIFO ������ ���ʱ�±��ĻḲ��ԭ�б���  
		CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  ����FIFO���ȼ� ȡ���ڱ��ı�ʾ�� 
		CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //��������ģʽ
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		  //BTR-SJW ����ͬ����Ծ��� 1��ʱ�䵥Ԫ
		CAN_InitStructure.CAN_BS1=CAN_BS1_4tq;		  //BTR-TS1 ʱ���1 ռ����4��ʱ�䵥Ԫ
		CAN_InitStructure.CAN_BS2=CAN_BS1_2tq;		  //BTR-TS1 ʱ���2 ռ����2��ʱ�䵥Ԫ	   
		CAN_InitStructure.CAN_Prescaler =6;		  		//BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� (CAN ʱ��Ƶ��Ϊ APB 1 = 42 MHz) 42/(1+4+2)/6=1 Mbps
		
  	CAN_Init(CAN1, &CAN_InitStructure); // ��ʼ��CAN1 
		//���ù�����0
 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	//����ģʽ
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	//32λ 																				
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x01<<11;      					//ֻɸѡ0x01�ı���
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;																				
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xF800;     				//32λMASK������Ϊ1��λ������ʾ��FilterId��ͬ��ɸѡλ�ɹ���
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;							//�����ĵ��ֽ�
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��		
		//���ù�����1
 	  CAN_FilterInitStructure.CAN_FilterNumber=1;	  //������1
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	//32λ 																				
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x02<<11;      					//ֻɸѡ0x02�ı���
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;																				
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xF800;      
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;//�����ĵ��ֽ�
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1;//������1������FIFO1
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������1
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
		//���ù�����2
		CAN_FilterInitStructure.CAN_FilterNumber=2;	  //������2
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	//32λ 																				
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x08<<11;     					//ֻɸѡ0x08�ı���
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;																				
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xF800;      
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;//�����ĵ��ֽ�
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1;//������1������FIFO1
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������2
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ
		//���ù�����3
		CAN_FilterInitStructure.CAN_FilterNumber=3;	  //������3
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	//32λ 																				
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x09<<11;      					//ֻɸѡ0x09�ı���
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;																				
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xF800;     
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;//�����ĵ��ֽ�
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1;//������1������FIFO1
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������3
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ
		
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
		
		CAN_ITConfig(CAN1,CAN_IT_FMP1,ENABLE);//FIFO1��Ϣ�Һ��ж�����.		    
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
		return 0;
}   



//void CAN1_RX1_IRQHandler(void)
//{
//	CanRxMsg RxMessage;
//	u32 i, FRAME_LEN,FRAME_NUMS,bufoff,FRAME_REV; 
//	if((CAN1_RX1_STA&0xffff0000)!=0)		//������ɱ�־	2016.06.21�޸Ľ����������ΪCAN1_RX1_STA&0xffff0000
//		CAN1->RF1R |= CAN_RF1R_RFOM1; 		//��FIFO_1�Һ���Ϣ
//	else
//	{
//		CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
//		bufoff=(((RxMessage.ExtId&0xff00)>>8)+(RxMessage.ExtId&0xf))<<3;			//����BUF��ŵĵ�ַƫ����
//		FRAME_NUMS=RxMessage.ExtId&0x0f;     	//֡���
//		FRAME_LEN=(RxMessage.ExtId&0xf0)>>4; 	//���İ�֡����
//		FRAME_REV=(CAN1_RX1_STA&0xffff)>>3;		//�յ�֡��=(�ֽ���/8)
//		if(FRAME_NUMS==FRAME_LEN)	
//		{
//			if((FRAME_REV==FRAME_LEN)&&(FRAME_LEN==0))		//2016.08.15�޸ģ�1.���İ�֡����=֡���=�յ�֡��2.���İ�֡����=֡���=0�������	
//				CAN1_RX1_STA|=((RxMessage.ExtId&0x00ffff00)<<8);	//�ñ�־λ(��־�������λ�����ںţ��������͹�16BIT)
//		}	
//		else if(FRAME_NUMS==0)							//����յ����İ�ͷ�����¿�ʼ����
//			CAN1_RX1_STA=0;
//		for(i=0;i<RxMessage.DLC;i++)				
//		{
//			CAN1_RX1_BUF[i+(bufoff)]=RxMessage.Data[i];
//			CAN1_RX1_STA++;
//			if((CAN1_RX1_STA&0xffff)>(CAN1_RX1_LEN-1))
//				CAN1_RX1_STA=0;//�������ݴ���,���¿�ʼ����	
//		}	
//		#if DEBUG_CANRECIVE_
//		ShowxNum(0,0,bufoff,6,0);
//		ShowxNum(0,0,RxMessage.ExtId,8,0);
//		#endif	
//	}		
//}




 /**
  * @brief  CAN��������״̬��ѯ
	* @param  CANx:CAN��x
	* @retval 0,�п�����; 1:�޿�����;
  */	
uint8_t CAN_GETMAILFLAG(CAN_TypeDef* CANx)
{
  uint8_t transmit_mailbox = 0;
  assert_param(IS_CAN_ALL_PERIPH(CANx));
  if ((CANx->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)
  {
    transmit_mailbox = 0;
  }
  else if ((CANx->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)
  {
    transmit_mailbox = 1;
  }
  else if ((CANx->TSR&CAN_TSR_TME2) == CAN_TSR_TME2)
  {
    transmit_mailbox = 2;
  }
  else
  {
    transmit_mailbox = CAN_TxStatus_NoMailBox;
  }
  if (transmit_mailbox != CAN_TxStatus_NoMailBox)
		return 0;
	else
		return 1;
}


 /**
  * @brief  CAN1�������ݺ���

  * @param  msg:����ָ��,���Ϊ8���ֽ�
	* @param  ext_id:���͵�CAN��չID
	* @param	len:���ݳ���(���Ϊ8)
	* @retval 0,�ɹ�; 1:����ʧ�ܣ�2:���߷�æ/����;
  */	
u8 CAN1_Send_Msg(u8* msg,u32 ext_id,u8 len)
{	
  u8 mbox;
  u32 i=0;
  CanTxMsg TxMessage;                   //����ṹ��TxMessage 
	TxMessage.ExtId=ext_id;	 							//������չ��ʾ����29λ��      	0 to 0x1FFFFFFF.  
  TxMessage.IDE=CAN_Id_Extended;		  	//ʹ����չ��ʽ
  TxMessage.RTR=CAN_RTR_Data;		  			//��Ϣ����Ϊ����֡
  TxMessage.DLC=len;							 			//���ݳ���(��Լ��Լ��Ϊ�̶�8)
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];	
	while(CAN_GETMAILFLAG(CAN1))
	{
		i++;	       
		delay_us(1);						//�ȴ�1us�ٳ���
		//if(i>=0XFFF)
		if(i>=0X3FFFF)
		{
			return 2;	//>65ms���г�ʱ
		}
	}
	mbox= CAN_Transmit(CAN1, &TxMessage); 
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF)) //�ȴ��������
	{
		i++;	                               
		if(i>=0XFFFF)return 1;
	}
	return 0;		
}
 /**
  * @brief  CAN1����Զ��֡
  * @param  ext_id:���͵�CAN��չID
	* @retval 0,�ɹ�; 1:����ʧ�ܣ�2:���߷�æ/����;
  */
u8 CAN1_Send_REQ(u32 ext_id)
{	
  u8 mbox;
  u32 i=0;
  CanTxMsg TxMessage;                   //����ṹ��TxMessage 
	TxMessage.ExtId=ext_id;	 							//������չ��ʾ����29λ��      	0 to 0x1FFFFFFF.  
	TxMessage.DLC=0;
  TxMessage.IDE=CAN_Id_Extended;		  	//ʹ����չ��ʽ
  TxMessage.RTR=CAN_RTR_Remote;		  		//��Ϣ����Ϊң��֡	
	while(CAN_GETMAILFLAG(CAN1))
	{
		i++;	       
		delay_us(1);						//�ȴ�1us�ٳ���
		//if(i>=0XFFF)
		if(i>=0XFFFF)
		{
			return 2;	//>65ms���г�ʱ
		}
		
	}
  mbox= CAN_Transmit(CAN1, &TxMessage); 
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF)) //�ȴ��������
	{
		i++;	                               
		if(i>=0XFFF)return 1;
	}
  return 0;		
}

void CAN_SHOW_ERR(u8 can_err)
{
	if(can_err==0)
	{
		DevSys.can1_err = 0;
	}
	else if(DevSys.can1_err<3)
	{
		DevSys.can1_err ++;
	}
}

