
#include "mainmenu.h"
#include "sys.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmleft;
extern GUI_CONST_STORAGE GUI_BITMAP bmright;
extern uint8_t	s_ucSelIconIndex2;
extern void _cbCallback25(WM_MESSAGE * pMsg);
#define BUF_S_LEN 36

static void Showsta(WM_MESSAGE * pMsg)
{
	int i =0;
	char tbuf[BUF_S_LEN]={0};
	DEV_STA	 bdc2_sta_synch,bdc2_sta_signal,bdc2_dif_sign,bdc2_time_valid,bdc2_LS_come,bdc2_LS_sign;
	u8 			bdc2_num_qua;
	u32 		bdc2_dif_s,bdc2_dif_ns;			
	//offset2=T3_OFFSET+16;	
	bdc2_sta_synch				=		(DEV_STA)((CAN1_RX0_BUF[T3_OFFSET+5]&0x02)>>1);						//bdc2Դͬ��״̬	
	bdc2_sta_signal				=		(DEV_STA)(CAN1_RX0_BUF[T3_OFFSET+5]&0x01);								//bdc2Դ�ź�״̬
	bdc2_time_valid				=		(DEV_STA)((CAN1_RX0_BUF[T3_OFFSET+5]&0x04)>>2);						//bdc2Դʱ������״̬ 
	
	bdc2_num_qua					=		(CAN1_RX0_BUF[T3_OFFSET+4]&0xf);														//bdc2Դʱ������λ
	bdc2_dif_sign					=		(DEV_STA)((CAN1_RX0_BUF[T3_OFFSET+16]&0x80)>>7);							//bdc2ԴPPS��Բ����
  bdc2_dif_s						=		((CAN1_RX0_BUF[T3_OFFSET+16]&0x7f)<<24)+(CAN1_RX0_BUF[T3_OFFSET+16+1]<<16)+(CAN1_RX0_BUF[T3_OFFSET+16+2]<<8)
														+(CAN1_RX0_BUF[T3_OFFSET+16+3]);																//bdc2ԴPPS��Բ�ֵ��λ
	bdc2_dif_ns						=		((CAN1_RX0_BUF[T3_OFFSET+16+4]<<24)+(CAN1_RX0_BUF[T3_OFFSET+16+5]<<16)+(CAN1_RX0_BUF[T3_OFFSET+16+6]<<8)
														+(CAN1_RX0_BUF[T3_OFFSET+16+7]))*OCX_PERIOD;														//bdc2ԴPPS��Բ�ֵ����λ
	bdc2_dif_ns =bdc2_dif_ns&0x7fffffff;
	if(bdc2_dif_ns>999999999) bdc2_dif_ns=999999999;																		//���ֵ��999 999 999
	
	
	bdc2_LS_come					=		(DEV_STA)((CAN1_RX0_BUF[T3_OFFSET+4]&0x80)>>7);		//����Ԥ��
	bdc2_LS_sign					=		(DEV_STA)((CAN1_RX0_BUF[T3_OFFSET+4]&0x40)>>6);		//�����־

	
	  
		if(bdc2_sta_synch==STA_OK)
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"ͬ��");
		else 
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"ʧ��");
			/*
		-------------------------1ʱ������λ-----------------------------------------------------------------------------*/
		sprintf( (char*)(tbuf),"%X\n",bdc2_num_qua);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf);
		/*
		-------------------------2ͨ����ֵ-----------------------------------------------------------------------------*/
		memset(tbuf,0,BUF_S_LEN);
		if(bdc2_dif_sign==STA_OK)		
			sprintf( (char*)(tbuf),"+%d��% 03d����\n",bdc2_dif_s,bdc2_dif_ns);
		else
			sprintf( (char*)(tbuf),"-%d��% 03d����\n",bdc2_dif_s,bdc2_dif_ns);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf);
				/*
		-------------------------3�ź�״̬-----------------------------------------------------------------------------*/
		if(bdc2_sta_signal==STA_OK)	
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"����");
		else 
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"�쳣");
		
			/*
		-------------------------4����״̬-----------------------------------------------------------------------------*/
		
		if(bdc2_LS_come)
		{
			if(bdc2_LS_sign==0)
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
		if(bdc2_time_valid==STA_OK)	
		 TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"����");
		else
		 TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"�쳣");
}








void _cbCallback26(WM_MESSAGE * pMsg) 
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

