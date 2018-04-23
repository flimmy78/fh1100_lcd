
#include "mainmenu.h"
#include "sys.h"

static void REC_Handler_APP04(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	uint16_t mask=0xA040; 			 //2.->>CAN_BUF��������
	uint8_t data;
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//�յ��ı����Ƕ�Ӧ�ۣ���Ӧ���ڣ���Ӧ���͵ı���
	{
		myprintf("CAN_REC :������/n");
		data  = CAN1_RX1_BUF[MONIT_OFFSET];
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		DROPDOWN_SetSel(hItem,data) ;
		DevSys.CAN1_Rec1st=1;//CAN1�������
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP04(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	uint32_t send_id = 0x03A04000;			//CANID
	uint8_t  msg[8]  = {0,}; 
	uint8_t  can_err;
	if(rtr)
	{
		myprintf("CAN_REQ :������/n");
		if(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY+1)
			DevSys.CAN1_Reqcnt++;//CAN1����++
		can_err=CAN1_Send_REQ(send_id);
	}
	else
	{
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		msg[0]= DROPDOWN_GetSel(hItem);
		can_err=CAN1_Send_Msg(msg,send_id,8);
		myprintf("CAN_SEND:������/n");	
	}
	CAN_SHOW_ERR(can_err);
}




void _cbCallback04(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
  hDlg = pMsg->hWin;
	
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(1);/*���������ʹ���*/
				/*��������*/
		  GUI_DrawLine(XPOS0+30,YPOS0-5,XPOS0+30,YPOS0+YGAPS*1);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//���ñ�����
		  //����������
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_AddString(hItem, "�ر�");  
			DROPDOWN_AddString(hItem, "����");
		  
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			WM_SetFocus (hItem);
		  Clear_CAN1();//��ʼ������CAN1״̬
		  CAN_SEND_APP04(pMsg,1); //��������
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
				CAN_SEND_APP04(pMsg,0);
			}
			break;
		case WM_TIMER:
			REC_Handler_APP04(pMsg);
			//��Գ�ʼ�������û���յ��ָ�����γ���(3��)
			if((DevSys.CAN1_Rec1st==0)&&(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY))
				{
						CAN_SEND_APP04(pMsg,1);
						#ifdef SHOW_DEBUG		//������Ϣ
						if(DevSys.CAN1_Reqcnt==NUM_CAN1_RETRY-1)
							TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP01), GUI_ORANGE);
						#endif
				}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;	
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}



