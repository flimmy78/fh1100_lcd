
#include "mainmenu.h"
#include "sys.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmleft;
extern GUI_CONST_STORAGE GUI_BITMAP bmright;
extern uint8_t	s_ucSelIconIndex2;





static void Showsta(WM_MESSAGE * pMsg)
{
	int i=0;
	char tbuf[20]={0};
	
//	DEV_STA STA_PW1		=		(DEV_STA)((CAN1_RX0_BUF[T0_OFFSET]&0x80)>>7);			//��Դ1״̬	
//	DEV_STA STA_PW2		=		(DEV_STA)((CAN1_RX0_BUF[T0_OFFSET]&0x40)>>6);			//��Դ2״̬	
// 	uint16_t FQ_INT1   =   (CAN1_RX0_BUF[DWFQ_STA_OFFSET]<<8)|CAN1_RX0_BUF[DWFQ_STA_OFFSET+1];		
//	uint16_t FQ_DEC1   =   (CAN1_RX0_BUF[DWFQ_STA_OFFSET+2]<<8)|CAN1_RX0_BUF[DWFQ_STA_OFFSET+3];	
//	uint16_t FQ_INT2   =   (CAN1_RX0_BUF[DWFQ_STA_OFFSET+16]<<8)|CAN1_RX0_BUF[DWFQ_STA_OFFSET+17];						
//	uint16_t FQ_DEC2   =   (CAN1_RX0_BUF[DWFQ_STA_OFFSET+18]<<8)|CAN1_RX0_BUF[DWFQ_STA_OFFSET+19];	
//	if(STA_PW1==STA_OK)
//		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"����" );
//	else
//		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"�쳣" );
//	if(STA_PW2==STA_OK)
//			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"����" );
//	else
//		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"�쳣" );
//	sprintf( tbuf , "%02d.%04dHz\n",FQ_INT1,FQ_DEC1); 
//	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf );//Ƶ��
//	memset(tbuf,0,20); 
//	sprintf( tbuf , "%02d.%04dHz\n",FQ_INT2,FQ_DEC2); 
//	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf );//Ƶ��
	
	DEV_STA STA_PW1		=		(DEV_STA)((CAN1_RX0_BUF[T0_OFFSET]&0x80)>>7);			//��Դ1״̬	
	DEV_STA STA_PW2		=		(DEV_STA)((CAN1_RX0_BUF[T0_OFFSET]&0x40)>>6);			//��Դ2״̬	
 	double FQ_INT1   =   (CAN1_RX0_BUF[DWFQ_STA_OFFSET+2]<<16)|(CAN1_RX0_BUF[DWFQ_STA_OFFSET+1]<<8)|CAN1_RX0_BUF[DWFQ_STA_OFFSET];		
	double FQ_INT2   =   (CAN1_RX0_BUF[DWFQ_STA_OFFSET+18]<<16)|(CAN1_RX0_BUF[DWFQ_STA_OFFSET+17]<<8)|CAN1_RX0_BUF[DWFQ_STA_OFFSET+16];

  double fq1,fq2;
	if(STA_PW1==STA_OK)
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"����" );
	else
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"�쳣" );
	if(STA_PW2==STA_OK)
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"����" );
	else
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"�쳣" );
//	FQ_INT1=5012;
//	FQ_INT2=5013;
	fq1=FQ_INT1/10000;
	fq2=FQ_INT2/10000;
	sprintf( tbuf , "%2.4fHz\n",fq1); 
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf );//Ƶ��
	memset(tbuf,0,20); 
	sprintf( tbuf , "%2.4fHz\n",fq2); 
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf );//Ƶ��
	
}



void _cbCallback20(WM_MESSAGE * pMsg) 
{

  WM_HWIN hItem;
	int i =0;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_STAAPP(4);/*���������ʹ���*/
		  GUI_DrawLine(LXPOS0-20,YPOS0-5,LXPOS0-20,YPOS0+LYGAPS*4);
		 // GUI_DrawBitmap(&bmleft,30,480-46);
		  GUI_DrawBitmap(&bmright,680-48,480-46);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);
      for(i=0;i<4;i++)
			 {
			     hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03 + i );
				   TEXT_SetFont(hItem, &GUI_FontHZ32);
				   hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10 + i );
				   TEXT_SetFont(hItem, &GUI_FontHZ32);

			 }
			Showsta(pMsg); 
      break;
//			case MSG_SetLEFT:		
//			if(WM_IsWindow(hWinSetapp))
//			{	
//				if(s_ucSelIconIndex2>0)s_ucSelIconIndex2--;GUI_EndDialog(pMsg->hWin, 0);APP_Setup(s_ucSelIconIndex2+20);//���뵽����APP����
//			}	
//	 		break;
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



