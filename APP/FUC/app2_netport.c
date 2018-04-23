
#include "mainmenu.h"
#include "sys.h"






/*****************************************************
APP02-��������
*****************************************************/
static void REC_Handler_APP02(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	uint16_t  mask=0x1004; 			 //2.->>CAN_BUF��������
	uint16_t i;
	
	uint8_t  slot,port;
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);//���
	slot= DROPDOWN_GetSel(hItem);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);//�˿�
	port= DROPDOWN_GetSel(hItem);
	/*ȷ��CAN_ID��*/
	mask=((slot+1)<<12)|port<<8|0x09;
	
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//�յ��ı����Ƕ�Ӧ�ۣ���Ӧ���ڣ���Ӧ���͵ı���
	{
		//myprintf("CAN_REQ ��������:%d.%d\n",slot,port);
		for(i=0;i<12;i++)
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i);
				EDIT_SetValue(hItem,CAN1_RX1_BUF[S5_OFFSET+i]) ;
			}	
		DevSys.CAN1_Rec1st=1;//CAN1�������	
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP02(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	int      i;
	uint32_t send_id ;			//CANID
	uint8_t  msg[8]  = {0,}; 
	uint8_t  can_err;
	uint8_t  slot,port;
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);//���
	slot= DROPDOWN_GetSel(hItem);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);//�˿�
	port= DROPDOWN_GetSel(hItem);
	/*ȷ��CAN_ID��*/
	if((slot==0)||(slot==10))//cpu��ͼ�ذ�
		send_id=0x03<<24|((slot+1)<<20)|port<<16|0x0940;
	else//net6��
		send_id=0x06<<24|((slot+1)<<20)|port<<16|0x0940;
	if(rtr)
		{
			//myprintf("CAN_REQ ��������:%d.%d\n",slot,port);
			for(i=0;i<12;i++)
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i);//��0
				EDIT_SetValue(hItem,0) ;
			}	
			if(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY+1)
				DevSys.CAN1_Reqcnt++;//CAN1����++
			can_err=CAN1_Send_REQ(send_id);
			
		}
	else
		{
			uint8_t readdata[16]={0};
			for(i=0;i<12;i++)
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i);
				readdata[i]= EDIT_GetValue(hItem) ;
			}	
			CAN1_Send_Msg(&readdata[0],send_id,8);
			CAN1_Send_Msg(&readdata[8],send_id+1,8);
			if((slot==0)||(slot==10))//cpu��ͼ�ذ�
				{
					CAN1_Send_Msg(&CAN1_RX1_BUF[S5_OFFSET+16],send_id+2,8);
					CAN1_Send_Msg(&CAN1_RX1_BUF[S5_OFFSET+24],send_id+3,8);
					can_err=CAN1_Send_Msg(&CAN1_RX1_BUF[S5_OFFSET+32],send_id+4,8);
				}
			else//net6��
				{
					CAN1_Send_Msg(msg,send_id+2,8);
					CAN1_Send_Msg(msg,send_id+3,8);
					msg[7] = (send_id&0xff0000)>>16;
					can_err=CAN1_Send_Msg(msg,send_id+4,8);
				}
			//myprintf("CAN_SEND ��������:%d.%d\n",slot,port);
		}
	CAN_SHOW_ERR(can_err);
}


 void _cbCallback02(WM_MESSAGE * pMsg) 
{
	/*IP������ʹ���˾�̬������ʵ�ֹ��λ�õı仯*/
	static WM_HWIN xitems[5];      //�ؼ����ֳ�6�У���¼ÿ�е�һ���ؼ��ľ����
	static uint8_t xsel  , ysel;   //����λ��
	//static uint8_t ITEMS = 16;     //�༭�����Ŀ
	static uint8_t ITEMS = 12;     //�༭�����Ŀ
  WM_HWIN hDlg;
  WM_HWIN hItem;
  int     i;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			/*���������ʹ���*/
		  PAINTTITL_SETAPP(5);
		  GUI_DrawLine(XPOS0-10,YPOS0-5,XPOS0-10,YPOS0+YGAPS*5);
			/*��д�����ַ��.��*/ 
		  GUI_SetTextMode(GUI_TM_TRANS);
		  GUI_SetColor(GUI_BLACK);
		  for (i = 2; i < 5; i++) 
		   {
		    GUI_DispCharAt('.',XPOS0+XGAPS*1-8,YPOS0+YGAPS*i+14);
		    GUI_DispCharAt('.',XPOS0+XGAPS*2-8,YPOS0+YGAPS*i+14);
			  GUI_DispCharAt('.',XPOS0+XGAPS*3-8,YPOS0+YGAPS*i+14);
		   }
       break;
    case WM_INIT_DIALOG:
			//���ñ�����
       PAINTWIN_SETAPP(pMsg);
		  //����������
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_AddString(hItem, "CPU��");  
			DROPDOWN_AddString(hItem, "��չ��3");
			DROPDOWN_AddString(hItem, "��չ��4");
			DROPDOWN_AddString(hItem, "��չ��5");
			DROPDOWN_AddString(hItem, "��չ��6");
			DROPDOWN_AddString(hItem, "��չ��7");
			DROPDOWN_AddString(hItem, "��չ��8");
			DROPDOWN_AddString(hItem, "��չ��9");
			DROPDOWN_AddString(hItem, "��չ��10");
		  DROPDOWN_AddString(hItem, "��չ��11");
		  DROPDOWN_AddString(hItem, "����");
		  //DROPDOWN_AddString(hItem, "��չ��13");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);  
			DROPDOWN_AddString(hItem, "����1");
			DROPDOWN_AddString(hItem, "����2");
			DROPDOWN_AddString(hItem, "����3");
			DROPDOWN_AddString(hItem, "����4");
			DROPDOWN_AddString(hItem, "����5");
			DROPDOWN_AddString(hItem, "����6");
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04);  
			TEXT_SetFont(hItem, &GUI_FontHZ16);
       //����EDIT��
      for (i = 0; i < ITEMS; i++) 
	    {
        hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + i);
        EDIT_SetDecMode(hItem, 0, 0, 255, 0, 0);
      }
      //�ؼ����ֳ�6�У���¼ÿ�е�һ���ؼ��ľ��
			xitems[0] = WM_GetDialogItem(hDlg, ID_DROPDOWN_APP0 );
			xitems[1] = WM_GetDialogItem(hDlg, ID_DROPDOWN_APP1 );
			xitems[2] = WM_GetDialogItem(hDlg, ID_EDIT_APP00 );
		  xitems[3] = WM_GetDialogItem(hDlg, ID_EDIT_APP04 );
			xitems[4] = WM_GetDialogItem(hDlg, ID_EDIT_APP08 );
			//xitems[5] = WM_GetDialogItem(hDlg, ID_EDIT_APP12 );
			
			//��ʼ������
			xsel = ysel =0;
			WM_SetFocus (xitems[0]);
			Clear_CAN1();//��ʼ������CAN1״̬
			CAN_SEND_APP02(pMsg,1);
      break;
		/************************************************************
			������Ϣ���⴦��
			*/	
		case MSG_SetUP:
			ysel = (ysel+4)%5;
		  WM_SetFocus (xitems[ysel]);
		  if(ysel>1)EDIT_SetCursorAtChar(xitems[ysel],0);
		  xsel = 0;
			break;
		case MSG_SetDOWM:
			ysel = (ysel+1)%5;
		  WM_SetFocus (xitems[ysel]);
		  if(ysel>1) EDIT_SetCursorAtChar(xitems[ysel],0);
		  xsel = 0;
			break;	
		case MSG_SetLEFT:
			if(ysel>1)
			{
				xsel = (xsel+11)%12;
				hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (ysel-2)*4 + xsel/3);
				WM_SetFocus (hItem);	
				EDIT_SetCursorAtChar(hItem,xsel%3);
			}
			break;
		case MSG_SetRIGHT:
			if(ysel>1)
			{
				xsel = (xsel+1)%12;
				hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (ysel-2)*4 + xsel/3);
				WM_SetFocus (hItem);	
				EDIT_SetCursorAtChar(hItem,xsel%3);
			}
			break;
 		/************************************************************
			������Ϣ���洦��
			*/	
	  case MSG_SetInc:		
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
		  if(ysel<2){
				Clear_CAN1();//��ʼ������CAN1״̬
				CAN_SEND_APP02(pMsg,1);
			}
	 		break;
	  case MSG_SetDec:			
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
		  if(ysel<2){
				Clear_CAN1();//��ʼ������CAN1״̬
				CAN_SEND_APP02(pMsg,1);
			}
			break;
		case MSG_SetENTER:
			Password_Verify();
		  break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp))
			{	
				 GUI_EndDialog(hDlg, 0);hWin_msgto=hWinMenuSet;MenuSet();//���ص����ò˵�����
			}
			break;	
		/************************************************************
			���ù��ܵ�ʵ��
			*/	
		case MSG_VerifyOK:
			{
			  CAN_SEND_APP02(pMsg,0);
			}
			break;
		case WM_TIMER:
			
			 REC_Handler_APP02(pMsg);
			//��Գ�ʼ�������û���յ��ָ�����γ���(3��)
			if((DevSys.CAN1_Rec1st==0)&&(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY))
				{
						CAN_SEND_APP02(pMsg,1);
						#ifdef SHOW_DEBUG		//������Ϣ
						if(DevSys.CAN1_Reqcnt==NUM_CAN1_RETRY-1)
							TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP01), GUI_ORANGE);
						#endif
				}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;		
		case MSG_FOUCE:
			xsel=ysel=0;
		  WM_SetFocus (xitems[0]);	
    default:
      WM_DefaultProc(pMsg);
  }
}





