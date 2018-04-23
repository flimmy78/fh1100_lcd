
#include "mainmenu.h"
#include "sys.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmleft;
extern GUI_CONST_STORAGE GUI_BITMAP bmright;
extern uint8_t	s_ucSelIconIndex2;
#define BUF_S_LEN 36


static void Showsta(WM_MESSAGE * pMsg)
{
	int i =0;
	char tbuf[BUF_S_LEN]={0};
	DEV_STA	 bdc1_sta_synch,bdc1_sta_signal,bdc1_dif_sign,bdc1_time_valid,bdc1_LS_come,bdc1_LS_sign;
	u8 			bdc1_num_qua;
	u32 		bdc1_dif_s,bdc1_dif_ns;

	bdc1_sta_synch				=		(DEV_STA)((CAN1_RX0_BUF[T3_OFFSET+2]&0x02)>>1);					//BDC1Դͬ��״̬	
	bdc1_sta_signal				=		(DEV_STA)(CAN1_RX0_BUF[T3_OFFSET+2]&0x01);							//BDC1Դ�ź�״̬
	bdc1_time_valid				=		(DEV_STA)((CAN1_RX0_BUF[T3_OFFSET+2]&0x04)>>2);					//BDC1Դʱ������״̬ 
	bdc1_num_qua					=		(CAN1_RX0_BUF[T3_OFFSET+1]&0xf);														//BDC1Դʱ������λ
	bdc1_dif_sign					=		(DEV_STA)((CAN1_RX0_BUF[T3_OFFSET+8]&0x80)>>7);								//BDC1ԴPPS��Բ����
  bdc1_dif_s						=		((CAN1_RX0_BUF[T3_OFFSET+8]&0x7f)<<24)+(CAN1_RX0_BUF[T3_OFFSET+8+1]<<16)+(CAN1_RX0_BUF[T3_OFFSET+8+2]<<8)
														+(CAN1_RX0_BUF[T3_OFFSET+8+3]);																//BDC1ԴPPS��Բ�ֵ��λ
	bdc1_dif_ns						=		((CAN1_RX0_BUF[T3_OFFSET+8+4]<<24)+(CAN1_RX0_BUF[T3_OFFSET+8+5]<<16)+(CAN1_RX0_BUF[T3_OFFSET+8+6]<<8)
														+(CAN1_RX0_BUF[T3_OFFSET+8+7]))*OCX_PERIOD;																//BDC1ԴPPS��Բ�ֵ����λ
	bdc1_dif_ns =bdc1_dif_ns&0x7fffffff;
	if(bdc1_dif_ns>999999999) bdc1_dif_ns=999999999;																		//���ֵ��999 999 999
	
	
	bdc1_LS_come					=		(DEV_STA)((CAN1_RX0_BUF[T3_OFFSET+1]&0x80)>>7);		//����Ԥ��
	bdc1_LS_sign					=		(DEV_STA)((CAN1_RX0_BUF[T3_OFFSET+1]&0x40)>>6);		//�����־
	
	  
		if(bdc1_sta_synch==STA_OK)
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"ͬ��");
		else 
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"ʧ��");
		/*
		-------------------------1ʱ������λ-----------------------------------------------------------------------------*/
		sprintf( (char*)(tbuf),"%X\n",bdc1_num_qua);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf);
		
		/*
		-------------------------2ͨ����ֵ-----------------------------------------------------------------------------*/
		memset(tbuf,0,BUF_S_LEN);
		if(bdc1_dif_sign==STA_OK)		
			sprintf( (char*)(tbuf),"+%d��% 03d����\n",bdc1_dif_s,bdc1_dif_ns);
		else
			sprintf( (char*)(tbuf),"-%d��% 03d����\n",bdc1_dif_s,bdc1_dif_ns);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf);
				/*
		-------------------------3�ź�״̬-----------------------------------------------------------------------------*/
		if(bdc1_sta_signal==STA_OK)	
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"����");
		else 
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"�쳣");
			/*
		-------------------------4����״̬-----------------------------------------------------------------------------*/
		
		if(bdc1_LS_come)
		{
			if(bdc1_LS_sign==0)
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"������");
			else
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"������");
		}
		else
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"������");
//		memset(tbuf,0,BUF_S_LEN);
//		sprintf( (char*)(tbuf),"����Ԥ��: %02d\n�����־: %02d\n",bds_LS_come,bds_LS_sign);	
			/*
		-------------------------5ʱ��������״̬-----------------------------------------------------------------------------*/
		if(bdc1_time_valid==STA_OK)	
		 TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"����");
		else
		 TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"�쳣");
}










void _cbCallback25(WM_MESSAGE * pMsg) 
{
	 int i;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			//PAINTTITL_SETAPP(5);/*���������ʹ���*/
		 
		  GUI_SetColor(GUI_COL_TITLE1);
			GUI_FillRect(0, 0, 800, 36 );
			GUI_SetPenSize(1);
			GUI_SetColor(GUI_WHITE);
			GUI_DrawLine(200,8,200,34);
			GUI_DrawLine(0,37,800,37);
			GUI_SetColor(GUI_BLACK);
			GUI_DrawLine(0,38,800,38);
			/*�����Ŀ�*/
			GUI_SetColor(GUI_WHITE);
			GUI_FillRoundedRect(20,70,700,440,8);
			/*��װ�ε�����*/
			GUI_SetColor(GUI_COL_WIN1);
			GUI_SetPenSize(3);
			GUI_DrawLine(80,112,600,112);
			GUI_SetPenSize(1);
			for (i = 1; i < 7; i++) 
			 {
				GUI_DrawLine(80,112+YGAPS*i,600,112+YGAPS*i);
			 }
		
		
		  GUI_DrawLine(XPOS1-20,YPOS0-5,XPOS1-20,YPOS0+YGAPS*6);
		  GUI_DrawBitmap(&bmleft,30,480-46);
		  GUI_DrawBitmap(&bmright,680-48,480-46);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);
			Showsta(pMsg); 
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
			
			
			
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp))
			{	
				 GUI_EndDialog(pMsg->hWin, 0);MenuSta();//���ص�״̬�˵�����
			}
			break;	
		case MSG_PPSTICK:
			Showsta(pMsg);
		 break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}



