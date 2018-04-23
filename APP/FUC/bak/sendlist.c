
#include "mainmenu.h"
#include "sys.h"


typedef struct 
{
	WM_CALLBACK * cb;
} APP_LIST_ITEM;

static void PAINTTITL_SETAPP(uint8_t line)
{
	int i;
	/*��������*/
	GUI_SetColor(GUI_COL_TITLE1);
	GUI_FillRect(0, 0, 800, 36 );
	GUI_SetPenSize(1);
	GUI_SetColor(GUI_WHITE);
	GUI_DrawLine(170,8,170,34);
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
	GUI_DrawLine(XPOS0-10,YPOS0-5,XPOS0-10,YPOS0+YGAPS*line);
	for (i = 1; i < line+1; i++) 
	 {
		GUI_DrawLine(80,112+YGAPS*i,600,112+YGAPS*i);
	 }
}

static void PAINTWIN_SETAPP(WM_MESSAGE * pMsg)
{

  WM_HWIN hItem = pMsg->hWin;
	WINDOW_SetBkColor(pMsg->hWin, GUI_COL_WIN1); //���ñ���ɫ
	//���ñ�����
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP00);
	TEXT_SetTextColor(hItem, GUI_WHITE);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_FontHZ24);
		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP01);
	TEXT_SetTextColor(hItem, GUI_GRAY_11);
	TEXT_SetTextAlign(hItem,   GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_FontHZ16);
		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP02);
	TEXT_SetTextColor(hItem, GUI_GRAY_11);
	TEXT_SetTextAlign(hItem,   GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_Font13B_1);
}


static void _cbCallback00(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(1);/*���������ʹ���*/
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//���ñ�����
		  //����������
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_SetFont(hItem, &GUI_FontHZ24); 
			DROPDOWN_AddString(hItem, "��ʱ��");  
			DROPDOWN_AddString(hItem, "��ʱ��");
      break;
 		/************************************************************
			�������⴦��
			*/	
		
		case MSG_SetUP:
		case MSG_SetDOWM:
		case MSG_SetLEFT:
		case MSG_SetRIGHT:
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
				 GUI_EndDialog(hDlg, 0);hWin_msgto=hWinMenuSet;MenuSet();//���ص����ò˵�����
			}
			break;	
		/************************************************************
			���ù��ܵ�ʵ��
			*/	
		case MSG_VerifyOK:
			{
				myprintf("set ok ");
			}
			break;
		case WM_TIMER:
			{
			
			}
			WM_RestartTimer(pMsg->Data.v, Timer2value);	
			break;	
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}


static void _cbCallback02(WM_MESSAGE * pMsg) 
{
	/*IP������ʹ���˾�̬������ʵ�ֹ��λ�õı仯*/
	static WM_HWIN xitems[6];      //�ؼ����ֳ�6�У���¼ÿ�е�һ���ؼ��ľ����
	static uint8_t xsel  , ysel;   //����λ��
	static uint8_t ITEMS = 16;     //�༭�����Ŀ
	const uint8_t _aDefaultSep[16] = {192,168, };  //�༭��ĳ�ʼֵ
  WM_HWIN hDlg;
  WM_HWIN hItem;
  int     i;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			/*���������ʹ���*/
		  PAINTTITL_SETAPP(6);
			/*��д�����ַ��.��*/ 
		  GUI_SetTextMode(GUI_TM_TRANS);
		  GUI_SetColor(GUI_BLACK);
		  for (i = 2; i < 6; i++) 
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
			DROPDOWN_SetFont(hItem, &GUI_FontHZ16); 
			DROPDOWN_AddString(hItem, "CPU��");  
			DROPDOWN_AddString(hItem, "��չ��1");
			DROPDOWN_AddString(hItem, "��չ��2");
			DROPDOWN_AddString(hItem, "��չ��3");
			DROPDOWN_AddString(hItem, "��չ��4");
			DROPDOWN_AddString(hItem, "��չ��5");
			DROPDOWN_AddString(hItem, "��չ��6");
			DROPDOWN_AddString(hItem, "��չ��7");
			DROPDOWN_AddString(hItem, "��չ��8");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);  
			DROPDOWN_SetFont(hItem, &GUI_FontHZ16); 
			DROPDOWN_AddString(hItem, "����1");
			DROPDOWN_AddString(hItem, "����2");
			DROPDOWN_AddString(hItem, "����3");
			DROPDOWN_AddString(hItem, "����4");
			DROPDOWN_AddString(hItem, "����5");
			DROPDOWN_AddString(hItem, "����6");
      
       //����EDIT��
      for (i = 0; i < ITEMS; i++) 
	    {
        hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + i);
        EDIT_SetDecMode(hItem, _aDefaultSep[i], 0, 255, 0, 0);
      }
      //�ؼ����ֳ�6�У���¼ÿ�е�һ���ؼ��ľ��
			xitems[0] = WM_GetDialogItem(hDlg, ID_DROPDOWN_APP0 );
			xitems[1] = WM_GetDialogItem(hDlg, ID_DROPDOWN_APP1 );
			xitems[2] = WM_GetDialogItem(hDlg, ID_EDIT_APP00 );
		  xitems[3] = WM_GetDialogItem(hDlg, ID_EDIT_APP04 );
			xitems[4] = WM_GetDialogItem(hDlg, ID_EDIT_APP08 );
			xitems[5] = WM_GetDialogItem(hDlg, ID_EDIT_APP12 );
      break;
		/************************************************************
			������Ϣ���⴦��
			*/	
		case MSG_SetUP:
			ysel = (ysel+5)%6;
		  WM_SetFocus (xitems[ysel]);
		  if(ysel>1)EDIT_SetCursorAtChar(xitems[ysel],0);
		  xsel = 0;
		  myprintf("MSG_SetUP");
			break;
		case MSG_SetDOWM:
			ysel = (ysel+1)%6;
		  WM_SetFocus (xitems[ysel]);
		  if(ysel>1) EDIT_SetCursorAtChar(xitems[ysel],0);
		  xsel = 0;
		 myprintf("MSG_SetDOWM");
			break;	
		case MSG_SetLEFT:
			if(ysel>1)
			{
				xsel = (xsel+11)%12;
				hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (ysel-2)*4 + xsel/3);
				WM_SetFocus (hItem);	
				EDIT_SetCursorAtChar(hItem,xsel%3);
				myprintf("MSG_SetLEFT");
			}
			break;
		case MSG_SetRIGHT:
			if(ysel>1)
			{
				xsel = (xsel+1)%12;
				hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (ysel-2)*4 + xsel/3);
				WM_SetFocus (hItem);	
				EDIT_SetCursorAtChar(hItem,xsel%3);
				myprintf("MSG_SetRIGHT");
			}
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
				 GUI_EndDialog(hDlg, 0);hWin_msgto=hWinMenuSet;MenuSet();//���ص����ò˵�����
			}
			break;	
		/************************************************************
			���ù��ܵ�ʵ��
			*/	
		case MSG_VerifyOK:
			{
				myprintf("set ok ");
			}
			break;
    case WM_NOTIFY_PARENT:
      break;
		case MSG_CAN1REV:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}




const APP_LIST_ITEM _APP_CB_LIST[] ={
	_cbCallback00,
	_cbCallback02,
	_cbCallback02,
  _cbCallback02,
};



/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/


