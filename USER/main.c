#include "includes.h"
#include "Mainmenu.h"
#include "sys.h"
#include "WM.h"
#include "DIALOG.h"

/*
*********************************************************************************************************
*			���ȼ�����
*********************************************************************************************************
*/
/*
APP_CFG_TASK_START_PRIO            2u		ɨ�谴��
APP_CFG_TASK_MsgCAN0_PRIO          3u		��CAN0����		
APP_CFG_TASK_PPSTICK_PRIO          4u		ÿ�����״̬(ÿ���CAN0������CANû�յ���ʹ��Ӳ��RTC�뻽��)
APP_CFG_TASK_COM_PRIO              5u		����
APP_CFG_TASK_USER_IF_PRIO          6u		��Ļ��������������
APP_CFG_TASK_GUI_PRIO              (OS_CFG_PRIO_MAX - 4u)	������ʾ��������ȼ�
*/

/*
*********************************************************************************************************
*            ���̿��ƿ�
*********************************************************************************************************
*/                                                         
static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB   AppTaskMsgCAN0TCB;
static  CPU_STK  AppTaskMsgCAN0Stk[APP_CFG_TASK_MsgCAN0_STK_SIZE];

static  OS_TCB   AppTaskCOMTCB;
static  CPU_STK  AppTaskCOMStk[APP_CFG_TASK_COM_STK_SIZE];

static  OS_TCB   AppTaskPPSTICKTCB;
static  CPU_STK  AppTaskPPSTICKStk[APP_CFG_TASK_PPSTICK_STK_SIZE];

static  OS_TCB   AppTaskUserIFTCB;
static  CPU_STK  AppTaskUserIFStk[APP_CFG_TASK_USER_IF_STK_SIZE];

static  OS_TCB   AppTaskGUITCB;
static  CPU_STK  AppTaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE];

/*
*********************************************************************************************************
*            ���̺���
*********************************************************************************************************
*/
static  void  AppTaskStart          (void     *p_arg);
static  void  AppTaskMsgCAN0         (void     *p_arg);
static  void  AppTaskPPSTICK         (void     *p_arg);
static  void  AppTaskCOM			(void 	  *p_arg);
static  void  AppTaskCreate         (void);
static  void  AppTaskGUI(void *p_arg);
static  void  AppObjCreate          (void);
void    App_Printf (CPU_CHAR *format, ...);

/*
*******************************************************************************************************
*             �ź�������
*******************************************************************************************************
*/
static  OS_SEM       AppPrintfSemp;	/* ����printf���� */
static	OS_SEM       SEM_SYNCH;	    /* ����CANͬ�� */
static	OS_SEM       SEM_PPSTICK;	  /* ������������Ӧ */
char BDTIMEBUF[30]= {0};


int main(void)
{
	OS_ERR  err;  
	bsp_init(); 
	sprintf(BDTIMEBUF,"%c%c%c%c -%c%c-%c%c "__TIME__"",BUILD_Y0,BUILD_Y1,BUILD_Y2,BUILD_Y3,
			BUILD_M0,BUILD_M1,BUILD_D0,BUILD_D1);
	OSInit(&err);  
	/* ����һ����������Ҳ���������񣩡���������ᴴ�����е�Ӧ�ó������� */
	OSTaskCreate((OS_TCB       *)&AppTaskStartTCB,  /* ������ƿ��ַ */           
                 (CPU_CHAR     *)"App Task Start",  /* ������ */
                 (OS_TASK_PTR   )AppTaskStart,      /* ������������ַ */
                 (void         *)0,                 /* ���ݸ�����Ĳ��� */
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO, /* �������ȼ� */
                 (CPU_STK      *)&AppTaskStartStk[0],     /* ��ջ����ַ */
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE / 10, /* ��ջ������������ʾ��10%��Ϊ����� */
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,      /* ��ջ�ռ��С */
                 (OS_MSG_QTY    )0,  /* ������֧�ֽ��ܵ������Ϣ�� */
                 (OS_TICK       )0,  /* ����ʱ��Ƭ */
                 (void         *)0,  /* ��ջ�ռ��С */  
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
				 (OS_ERR       *)&err);	
				 /*  �������£�
					OS_OPT_TASK_STK_CHK      ʹ�ܼ������ջ��ͳ������ջ���õĺ�δ�õ�
					OS_OPT_TASK_STK_CLR      �ڴ�������ʱ����������ջ
					OS_OPT_TASK_SAVE_FP      ���CPU�и���Ĵ��������������л�ʱ���渡��Ĵ���������
				 */   
    OSStart(&err);                                               
    (void)&err;
    return (0);
}


static  void  AppTaskStart (void *p_arg)
{
	OS_ERR      err;
    (void)p_arg;  
#if OS_CFG_STAT_TASK_EN > 0u
     OSStatTaskCPUUsageInit(&err);   
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif	
	/* ����Ӧ�ó�������� */
	AppTaskCreate();
	/* ��������ͨ�� */
	AppObjCreate();
	while(1)
	{
		bsp_KeyScan();		//���ֶ԰�����ɨ��
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ5ms	
	}     
}


static void AppTaskMsgCAN0(void *p_arg)
{
	OS_ERR      err;
	(void)p_arg;
	while(1)
	{	
		OSSemPend(&SEM_SYNCH,0,OS_OPT_PEND_BLOCKING,0,&err); //�ȴ�����CPU��CAN�źţ���RTC���뻽�ѹ�ͬȥ(����)������ʱ��3
		if(err == OS_ERR_NONE)
		{	
			CAN_Head_Handler();	
		}			
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ50ms	
	}   
}


static void AppTaskPPSTICK(void *p_arg)
{
	OS_ERR      err;
	(void)p_arg;	              /* ������������� */
	while (1) 
	{   
		OSSemPend(&SEM_PPSTICK,0,OS_OPT_PEND_BLOCKING,0,&err); //����PPSTICK�ź������ɶ�ʱ��3����
		DEV_PPS_RECV();        			//ÿ�����װ�õĽ������ʾ(������͵�)
		#if defined PPSTICK__RTC
			DEV_Time_Handler(1);		//��������λ���С��2����ʹ�ñ��ĵ�ʱ�䣬�����RTC��ʱ������ʾ
		#endif
		DevSys.PPS_Tick =1;				//��������װ�ý����״̬���µ�һ���ź�
		OSTimeDlyHMSM(0,0,0,350,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ350ms	
		DevSys.PPS_Tick_300 = 1;		//��������װ�ý����״̬���µ���һ���ź�,��������˸Ч��
	}
}



static void AppTaskCOM(void *p_arg)
{
	
	OS_ERR  err;  	
	(void)p_arg;
	while(1)
	{	
		OSTimeDlyHMSM(0,10,0,0,OS_OPT_TIME_HMSM_STRICT,&err); //��������������ȡ������Ϣ��
	} 						  	 	       											   
}


static void AppTaskUserIF(void *p_arg)
{
	
	static uint8_t laststate = 0;
	OS_ERR      err;
	(void)p_arg;	           /* ������������� */
	while (1) 
	{ 
		//ι��
   	#if defined WATCHDOG_USE
			IWDG_Feed();
		#endif	
		if(DevSys.LCD_Protect_ON == 0)//������Ļ����
			{
				if(DevSys.idle_out_flag!=laststate)  //�б�λ����
					{
						if(DevSys.idle_out_flag == 0)
						 LCD_BackLightOn();
						else
						 LCD_BackLightOff();
					}
				laststate = DevSys.idle_out_flag;
			}
		//��Ļ����
		if(DevSys.idle_out>=IDLE_TIMEOUT)			//����
			{
				DevSys.idle_out_flag = STA_ERR;
			}
		else										//�ۼ�
			{
				DevSys.idle_out_flag = STA_OK;
				DevSys.idle_out++;
			}
		//��ͨѸ����������
		if(DevHead.devfalt&0x08){
			if(DevSys.can0_err<CPU_REBOOT_TIME)
				DevSys.can0_err++;
			else if(DevSys.can0_err==CPU_REBOOT_TIME)
				NVIC_SystemReset();
		}	
		else
			DevSys.can0_err=0;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
	}
}


static void AppTaskGUI(void *p_arg)
{
	MainTask();
}


static  void  AppTaskCreate (void)
{
	OS_ERR      err;
	
	/**************����MsgCAN0����*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskMsgCAN0TCB,             
                 (CPU_CHAR     *)"App Task MspCAN0",
                 (OS_TASK_PTR   )AppTaskMsgCAN0, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_MsgCAN0_PRIO,
                 (CPU_STK      *)&AppTaskMsgCAN0Stk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_MsgCAN0_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_MsgCAN0_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
				 
	/**************����PPS_TICK����*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskPPSTICKTCB,             
                 (CPU_CHAR     *)"App Task PPSTICK",
                 (OS_TASK_PTR   )AppTaskPPSTICK, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_PPSTICK_PRIO,
                 (CPU_STK      *)&AppTaskPPSTICKStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_PPSTICK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_PPSTICK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )( OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);

	/**************����COM����*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskCOMTCB,            
                 (CPU_CHAR     *)"App Task COM",
                 (OS_TASK_PTR   )AppTaskCOM, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_COM_PRIO,
                 (CPU_STK      *)&AppTaskCOMStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);	
/**************����USER IF����*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskUserIFTCB,             
                 (CPU_CHAR     *)"App Task UserIF",
                 (OS_TASK_PTR   )AppTaskUserIF, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_USER_IF_PRIO,
                 (CPU_STK      *)&AppTaskUserIFStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_USER_IF_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_USER_IF_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_SAVE_FP | OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
								 
	/**************����GUI����*********************/			 
	OSTaskCreate((OS_TCB       *)&AppTaskGUITCB,              
                 (CPU_CHAR     *)"App Task GUI",
                 (OS_TASK_PTR   )AppTaskGUI, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_GUI_PRIO,
                 (CPU_STK      *)&AppTaskGUIStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),//STEMWIN��Ҫ�洢����
                 (OS_ERR       *)&err);


 
								 
}

/*
*********************************************************************************************************
*	�� �� ��: AppObjCreate
*	����˵��: ��������ͨѶ
*	��    ��: p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static  void  AppObjCreate (void)
{
	OS_ERR      err;
	
	/* 
	   �����ź�����ֵΪ1��ʱ�����ʵ�ֻ��⹦�ܣ�Ҳ����ֻ��һ����Դ����ʹ�� 
	   �������ǽ�����1�Ĵ�ӡ������Ϊ��������Դ����ֹ���ڴ�ӡ��ʱ������������ռ
	   ��ɴ��ڴ�ӡ���ҡ�
	*/
	OSSemCreate((OS_SEM    *)&AppPrintfSemp,
				(CPU_CHAR  *)"AppPrintfSemp",
				(OS_SEM_CTR )0,//��ʼֵ
				(OS_ERR    *)&err);
	
	/* ��������ֵΪ0������ʵ������ͬ������ */
	OSSemCreate((OS_SEM    *)&SEM_SYNCH,
				(CPU_CHAR  *)"SEM_SYNCH",
				(OS_SEM_CTR )0,
				(OS_ERR    *)&err);
	
	/* ��������ֵΪ0������ʵ������PPS���� */
	OSSemCreate((OS_SEM    *)&SEM_PPSTICK,
				(CPU_CHAR  *)"SEM_PPSTICK",
				(OS_SEM_CTR )0,
				(OS_ERR    *)&err);
}

/*
*********************************************************************************************************
*	�� �� ��: App_Printf
*	����˵��: �̰߳�ȫ��printf��ʽ		  			  
*	��    ��: ͬprintf�Ĳ�����
*             ��C�У����޷��г����ݺ���������ʵ�ε����ͺ���Ŀʱ,������ʡ�Ժ�ָ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void  App_Printf(CPU_CHAR *format, ...)
{
    CPU_CHAR  buf_str[80 + 1];
    va_list   v_args;
    OS_ERR    os_err;


    va_start(v_args, format);
   (void)vsnprintf((char       *)&buf_str[0],
                   (size_t      ) sizeof(buf_str),
                   (char const *) format,
                                  v_args);
    va_end(v_args);

	/* ������� */
    OSSemPend((OS_SEM  *)&AppPrintfSemp,
              (OS_TICK  )0u,
              (OS_OPT   )OS_OPT_PEND_BLOCKING,
              (CPU_TS  *)0,
              (OS_ERR  *)&os_err);

    printf("%s", buf_str);

   (void)OSSemPost((OS_SEM  *)&AppPrintfSemp,
                   (OS_OPT   )OS_OPT_POST_1,
                   (OS_ERR  *)&os_err);

}


//RTC WAKE UP�жϷ�����
void RTC_WKUP_IRQHandler(void)
{ 
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntEnter();    
#endif	
if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET)//WK_UP�ж�?
	{ 
		RTC_ClearFlag(RTC_FLAG_WUTF);	//����жϱ�־
		TIM_Cmd(TIM3, DISABLE);
		TIM_Cmd(TIM3, ENABLE);
	}   
	EXTI_ClearITPendingBit(EXTI_Line22);//����ж���22���жϱ�־ 			
#if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//�˳��жϣ������жϼ�����
#endif		
}
 


 /**
  * @brief  CAN1_RX0��Ϣ�Һ��жϷ�����������POWERPCÿ�붯̬��Ϣ
  * @param  ��
  * @retval ��
  */		 
void CAN1_RX0_IRQHandler(void)
{
	OS_ERR    os_err;
	CanRxMsg RxMessage;
	u32 i,bufoff;
	RxMessage.ExtId=0;
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntEnter();    
#endif
	
	if(RxMessage.ExtId==0x01100130)		//�յ����İ�ͷ�����¿�ʼ����
			CAN1_RX0_STA=0;
	if((CAN1_RX0_STA&0x10000)>>16)//�������	
		CAN1->RF0R |= CAN_RF0R_RFOM0; //��FIFO_0�Һ���Ϣ
	else
	{
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		bufoff=(((RxMessage.ExtId&0xff00)>>8)+(RxMessage.ExtId&0xf))<<3;			//����BUF��ŵĵ�ַƫ����
		if(RxMessage.ExtId==0x01102311)					//�յ����һ֡���ý�����ɱ�־λ
			{
				CAN1_RX0_STA|=0x10000;	
 /*
*********************************************************************************************************
			�����ź���
*********************************************************************************************************
*/			
				OSSemPost(&SEM_SYNCH,OS_OPT_POST_1,&os_err);//�����ź���			
			}
		
		for(i=0;i<RxMessage.DLC;i++)												//����֡�̶�8���ֽ�
		{
			CAN1_RX0_BUF[i+bufoff]=RxMessage.Data[i];
			CAN1_RX0_STA++;
			if((CAN1_RX0_STA&0xffff)>(CAN1_RX0_LEN-1))
				CAN1_RX0_STA=0;//�������ݴ���,���¿�ʼ����	 		
		}
	}
#if DEBUG_CANRECIVE_
	//ShowxNum(0,0,bufoff,6,0);
#endif
 #if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//�˳��жϣ������жϼ�����
#endif		
}

 /**
  * @brief  CAN1_RX1��Ϣ�Һ��жϷ���������������״̬��Ϣ
  * @param  ��
  * @retval ��
  */
void CAN1_RX1_IRQHandler(void)
{
	CanRxMsg RxMessage;
	u32 i, FRAME_LEN,FRAME_NUMS,bufoff,FRAME_REV; 
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntEnter();    
	#endif
	if((CAN1_RX1_STA&0xffff0000)!=0)		//������ɱ�־	2016.06.21�޸Ľ����������ΪCAN1_RX1_STA&0xffff0000
		CAN1->RF1R |= CAN_RF1R_RFOM1; 		//��FIFO_1�Һ���Ϣ
	else
	{
		CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
		bufoff=(((RxMessage.ExtId&0xff00)>>8)+(RxMessage.ExtId&0xf))<<3;			//����BUF��ŵĵ�ַƫ����
		FRAME_NUMS=RxMessage.ExtId&0x0f;     	//֡���
		FRAME_LEN=(RxMessage.ExtId&0xf0)>>4; 	//���İ�֡����
		FRAME_REV=(CAN1_RX1_STA&0xffff)>>3;		//�յ�֡��=(�ֽ���/8)
		if(FRAME_NUMS==FRAME_LEN)	
		{
			if((FRAME_REV==FRAME_LEN)||(FRAME_LEN==0))		//2016.08.15�޸ģ�1.���İ�֡����=֡���=�յ�֡��2.���İ�֡����=֡���=0�������	
				CAN1_RX1_STA|=((RxMessage.ExtId&0x00ffff00)<<8);	//�ñ�־λ(��־�������λ�����ںţ��������͹�16BIT)
		}	
		else if(FRAME_NUMS==0)							//����յ����İ�ͷ�����¿�ʼ����
			CAN1_RX1_STA=0;
		for(i=0;i<RxMessage.DLC;i++)				
		{
			CAN1_RX1_BUF[i+(bufoff)]=RxMessage.Data[i];
			CAN1_RX1_STA++;
			if((CAN1_RX1_STA&0xffff)>(CAN1_RX1_LEN-1))
				CAN1_RX1_STA=0;//�������ݴ���,���¿�ʼ����	
		}	
	}		
#if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//�˳��жϣ������жϼ�����
#endif	
	//LED10=!LED10;
}

//��ʱ��3�жϷ�����,��PPS���Ѻ�CAN_PPS��������
void TIM3_IRQHandler(void)
{
	OS_ERR    os_err;
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntEnter();    
#endif
	if(TIM_GetITStatus(TIM3,TIM_IT_CC1)==SET) //����ж�	
	{
		
		 /*
*********************************************************************************************************
			�����ź���
*********************************************************************************************************
*/			
		OSSemPost(&SEM_PPSTICK,OS_OPT_POST_1,&os_err);//�����ź���		
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);  //����жϱ�־λ
	
#if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//�˳��жϣ������жϼ�����
#endif		
}







