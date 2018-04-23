
#include "mainmenu.h"
#include "sys.h"

static void REC_Handler_APP10(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	uint16_t mask=0x100E; 			 //2.->>CAN_BUF��������
	uint8_t data;
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//�յ��ı����Ƕ�Ӧ�ۣ���Ӧ���ڣ���Ӧ���͵ı���
	{
		myprintf("CAN_REC :ʱ������/n");
		data  = CAN1_RX1_BUF[S6_OFFSET];
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		if(data!=0)
		 DROPDOWN_SetSel(hItem,0) ;
		else
		 DROPDOWN_SetSel(hItem,1) ;
		data  = CAN1_RX1_BUF[S6_OFFSET+1];
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);
		DROPDOWN_SetSel(hItem,data) ;
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP10(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	uint32_t send_id = 0x03100E00;			//CANID
	uint8_t  msg[8]  = {0,}; 
	uint8_t  can_err;
	if(rtr)
	{
		myprintf("CAN_REQ :ʱ������/n");
		can_err=CAN1_Send_REQ(send_id);
	}
	else
	{
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		msg[0]= DROPDOWN_GetSel(hItem);
		if(msg[0]==0)
			msg[0] = +8;
		else
			msg[0] = 0;
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);
		msg[1]= DROPDOWN_GetSel(hItem);
		can_err=CAN1_Send_Msg(msg,send_id,8);
		myprintf("CAN_SEND:ʱ������/n");	
	}
	CAN_SHOW_ERR(can_err);
}




void _cbCallback10(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
  hDlg = pMsg->hWin;
	
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(2);/*���������ʹ���*/
				/*��������*/
		  GUI_DrawLine(XPOS0+30,YPOS0-5,XPOS0+30,YPOS0+YGAPS*2);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//���ñ�����
		  //����������
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_AddString(hItem, "UTC+8(�й�)");  
			DROPDOWN_AddString(hItem, "UTC+0(����)");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);  
			DROPDOWN_AddString(hItem, "��ƫ��(Ĭ��)");  
			DROPDOWN_AddString(hItem, "ƫ��0.5Сʱ");
		  CAN_SEND_APP10(pMsg,1); //��������
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			WM_SetFocus (hItem);
		  
      break;
 		/************************************************************
			�������⴦��
			*/	
		case MSG_FOUCE:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			WM_SetFocus (hItem);
			break;
		/************************************************************
			������Ϣ���洦��
			*/	
		case MSG_SetUP:
			GUI_SendKeyMsg(GUI_KEY_BACKTAB, 1);
		  break;
		case MSG_SetDOWM:
			GUI_SendKeyMsg(GUI_KEY_TAB, 1);
		  break;
		case MSG_SetLEFT:
			GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
			break;
		case MSG_SetRIGHT:
			GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
			break;	
	  case MSG_SetInc:		
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
	 		break;
	  case MSG_SetDec:			
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
			break;
		case MSG_SetENTER:
			Password_Verify();
		  break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp))
			{	
				 GUI_EndDialog(hDlg, 0);MenuSet();//���ص����ò˵�����
			}
			break;	
		/************************************************************
			���ù��ܵ�ʵ��
			*/	
		case MSG_VerifyOK:
			{
				CAN_SEND_APP10(pMsg,0);
			}
			break;
		case WM_TIMER:
			{
				REC_Handler_APP10(pMsg);
			}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;	
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}



