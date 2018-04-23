
#include "mainmenu.h"
#include "sys.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmleft;
extern GUI_CONST_STORAGE GUI_BITMAP bmright;
extern uint8_t	s_ucSelIconIndex2;
extern uint8_t ALA_POINT[30][40];

static  uint8_t line ;
static  uint8_t  ALAMSG[40]={0};

static void Readline(void)
{	
	u8  nosounce    = 0;
	//u8 	devfault	= (CAN1_RX0_BUF[T0_OFFSET+1]);
	//u8 	devala		= (CAN1_RX0_BUF[T0_OFFSET+2]);
	u8 	devfault	= DevHead.devfalt;
	u8  devala		= DevHead.devalar;
	u8	bds_time_valid,gps_time_valid;
	u8  bdc1_sta_signal,bdc1_time_valid;
	u8  bdc2_sta_signal,bdc2_time_valid;
//	u8  ptp1_sta_signal,ptp2_sta_signal;
	u8 STA_PW1		=	(DEV_STA)((CAN1_RX0_BUF[T0_OFFSET]&0x80)>>7);					//��Դ1״̬	
	u8 STA_PW2		=	(DEV_STA)((CAN1_RX0_BUF[T0_OFFSET]&0x40)>>6);					//��Դ2״̬	
	bds_time_valid	=	(DEV_STA)((CAN1_RX0_BUF[T1_OFFSET+4]&0x04)>>2);					//����Դʱ������״̬ 
	gps_time_valid	=	(DEV_STA)((CAN1_RX0_BUF[T2_OFFSET+4]&0x04)>>2);					//GPSԴʱ������״̬
	bdc1_sta_signal	=	(DEV_STA)(CAN1_RX0_BUF[T3_OFFSET+2]&0x01);						//BDC1Դ�ź�״̬
	bdc1_time_valid	=	(DEV_STA)((CAN1_RX0_BUF[T3_OFFSET+2]&0x04)>>2);					//BDC1Դʱ������״̬ 
	bdc2_sta_signal	=	(DEV_STA)(CAN1_RX0_BUF[T3_OFFSET+5]&0x01);						//bdc2Դ�ź�״̬
	bdc2_time_valid	=	(DEV_STA)((CAN1_RX0_BUF[T3_OFFSET+5]&0x04)>>2);					//bdc2Դʱ������״̬ 
 	line =0;
	if(devfault>0)	//���ϵƿ���
	{		
		if(devfault&0x80)	{ALAMSG[line]=1;line++;}	
		if(devfault&0x40)	{ALAMSG[line]=2;line++;}  
		if(devfault&0x20)	{ALAMSG[line]=3;line++;}		
		if(devfault&0x10)	{ALAMSG[line]=4;line++;}											
		if(devfault&0x08)	{ALAMSG[line]=5;line++;}															
		if(devfault&0x04)	{ALAMSG[line]=6;line++;}													
		if((devfault&0x02)&&(nosounce==0))	{nosounce = 1;ALAMSG[line]=7;line++;}								
		if((devfault&0x01)&&(nosounce==0))	{nosounce = 1;ALAMSG[line]=8;line++;}		
	}
		
	if(devala>0)
	{
		
		if(devala&0x80)																	//�澯B7				
		{
			if(STA_PW1)	{ALAMSG[line]=10;line++;} 	//�澯��Դʧ��: ��Դ1
			if(STA_PW2)	{ALAMSG[line]=11;line++;}	//��Դʧ��: ��Դ2
		}	
		if(devala&0x40)	{ALAMSG[line]=12;line++;}	//�澯B6 	��������ʧ��
		if(devala&0x20)	{ALAMSG[line]=13;line++;}	//�澯B5 	GPS ����ʧ��
		if(devala&0x10)	{ALAMSG[line]=14;line++;}	//�澯B4  IRIG-B�������������ڱ���							
		if(devala&0x08)
		{
			if(1){
				{ALAMSG[line]=15;line++;}	//15 �澯B3.(1+2+3+4)		ʱ���������쳣
			}
			else{
				if(bds_time_valid)	{ALAMSG[line]=16;line++;}		//ʱ���������쳣: ����
				if(gps_time_valid)	{ALAMSG[line]=17;line++;}		//ʱ���������쳣:GPS
				if(bdc1_time_valid)	{ALAMSG[line]=18;line++;}		//ʱ���������쳣:IRIG-B1
				if(bdc2_time_valid)	{ALAMSG[line]=19;line++;}		//ʱ���������쳣:IRIG-B2
				//if(ptp1_time_valid)	{ALAMSG[line]=30;line++;}		//ʱ���������쳣:PTP1
				//if(ptp2_time_valid)	{ALAMSG[line]=31;line++;}		//ʱ���������쳣:PTP2
			}
			
		}
		if(devala&0x04)	
		{
			if(1){
				{ALAMSG[line]=20;line++;}	//20 �澯B2.(1+2+3+4)		����һ·ʱ��Դ������
			}
			else{
				if(devala&0x40) 	{ALAMSG[line]=21;line++;}		//ʱ��Դ������:����
				if(devala&0x20) 	{ALAMSG[line]=22;line++;}		//ʱ��Դ������:GPS
				if(bdc1_sta_signal) {ALAMSG[line]=23;line++;}		//ʱ��Դ������:IRIG-B1
				if(bdc2_sta_signal) {ALAMSG[line]=24;line++;}		//ʱ��Դ������:IRIG-B2
				//if(ptp1_sta_signal)	{ALAMSG[line]=28;line++;}		//ʱ��Դ������:PTP1
				//if(ptp2_sta_signal)	{ALAMSG[line]=29;line++;}		//ʱ��Դ������:PTP2
			}
		}
		if((devala&0x02)&&(nosounce==0))	{nosounce = 1;ALAMSG[line]=26;line++;}						//26 �澯B1 ���ж���ʱԴ��������(�״�ͬ��,<30����) 
		if((devala&0x01)&&(nosounce==0))	{nosounce = 1;ALAMSG[line]=27;line++;}						//27 �澯B0 ���ж���ʱԴ��������(����ͬ��,<24Сʱ) 
	}		

}
static void Showalarm(WM_MESSAGE * pMsg)
{
	 WM_HWIN hItem;
	 int i =0,temp;
	 char tbuf1[3];
	 char tbuf2[40];
	 if(line == 0)
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03);
				TEXT_SetTextColor(hItem, GUI_WHITE);
				TEXT_SetFont(hItem, &GUI_FontHZ16);
				TEXT_SetText(hItem,"�޸澯��Ϣ" );
			}
		  else
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_APP0); 
				for (i = 0; i <line; i++) 
					{
						
						LISTVIEW_AddRow(hItem, 0);				
					}		
		
				for(i=0;i<line;i++)
					{
						temp=ALAMSG[i];									
						sprintf( (char*)(tbuf1 ),"%02d",i+1);
						memcpy((char*)tbuf2,(char *)&(ALA_POINT[temp][0]),40);
						LISTVIEW_SetItemText(hItem,0,i,tbuf1);
						LISTVIEW_SetItemText(hItem,1,i,tbuf2);	
						
					}
			}					
			hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_APP0);  
			WM_SetFocus (hItem);			
	
}



void _cbCallback22(WM_MESSAGE * pMsg) 
{

	HEADER_Handle hHeader;
  WM_HWIN hItem;
	//int i =0,temp;
  //char tbuf1[3];
	//char tbuf2[40];
  switch (pMsg->MsgId) {
    case WM_PAINT:
			/*��������*/
			GUI_SetColor(GUI_COL_TITLE1);
			GUI_FillRect(0, 0, 800, 36 );
			GUI_SetPenSize(1);
			GUI_SetColor(GUI_WHITE);
			GUI_DrawLine(170,8,170,34);
			GUI_DrawLine(0,37,800,37);
			GUI_SetColor(GUI_BLACK);
			GUI_DrawLine(0,38,800,38);
		  GUI_DrawBitmap(&bmleft,30,480-46);
		  GUI_DrawBitmap(&bmright,680-48,480-46);
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04);
			TEXT_SetFont(hItem, &GUI_FontHZ16);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);
		
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_APP0);  
			hHeader = LISTVIEW_GetHeader(hItem);
			HEADER_SetFont(hHeader, &GUI_FontHZ16);
		  LISTVIEW_AddColumn(hItem, 70, "���", GUI_TA_CENTER|TEXT_CF_VCENTER);
		  LISTVIEW_AddColumn(hItem, 728,  "        �澯״̬" , TEXT_CF_LEFT|TEXT_CF_VCENTER);
		  LISTVIEW_SetRowHeight(hItem, 55);
			LISTVIEW_SetGridVis(hItem,1);
		  SCROLLBAR_CreateAttached(hItem, SCROLLBAR_CF_VERTICAL);
		
		  Readline();//��ȡ�澯��Ϣ
		  Showalarm(pMsg);
      break;
		case MSG_SetUP:
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
		  break;
		case MSG_SetDOWM:
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
		  break;
	  case MSG_SetInc:	
			GUI_SendKeyMsg(GUI_KEY_PGUP, 1);
	 		break;
	  case MSG_SetDec:	
      GUI_SendKeyMsg(GUI_KEY_PGDOWN, 1);			
			break;	 
		case MSG_SetLEFT:		
			if(WM_IsWindow(hWinSetapp))
			{	
				if(s_ucSelIconIndex2>0)s_ucSelIconIndex2--;GUI_EndDialog(pMsg->hWin, 0);APP_Setup(s_ucSelIconIndex2+20);//���뵽����APP����
			}	
	 		break;
	  case MSG_SetRIGHT:			
			if(WM_IsWindow(hWinSetapp))
			{	
				if(s_ucSelIconIndex2<Devsta_num)s_ucSelIconIndex2++;GUI_EndDialog(pMsg->hWin, 0);APP_Setup(s_ucSelIconIndex2+20);//���뵽����APP����
			}	
			break;
		case MSG_SetENTER:
			Readline();//��ȡ�澯��Ϣ
		  Showalarm(pMsg);
		  break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp))
			{	
				 GUI_EndDialog(pMsg->hWin, 0);MenuSta();//���ص�״̬�˵�����
			}
			break;	
    default:
      WM_DefaultProc(pMsg);
  }
	
}



