#include "mainmenu.h"
#include "sys.h"

#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_TEXT_0      (GUI_ID_USER + 0x01)
#define ID_TEXT_1      (GUI_ID_USER + 0x02)
#define ID_TEXT_2      (GUI_ID_USER + 0x03)
//#define WIN_COL     0xe9af31

WM_HWIN  hWin_msgto ;   //ָ�����ھ���õľ��ָ�룬��Ҫ���ڴ��ݰ�����Ϣ��ָ���۽�
WM_HWIN  hWinTop;       //��Ŷ��㴰�ڵľ��
WM_HWIN  hWinLogin;     //�û���¼
//uint8_t  LoginRight;    //�û�Ȩ��
WM_HWIN  hWinMainMenu;  //���˵�
WM_HWIN  hWinIconMenu;  //���˵���ͼ��
//һ���˵�
WM_HWIN  hWinDevsta;    //װ��״̬
WM_HWIN  hWinMenuSet;   //��������
WM_HWIN  hWinIconSet;   //�������õ�ͼ��
WM_HWIN  hWinMenuSta;   //״̬Ŀ¼
WM_HWIN  hWinIconSta;   //װ��״̬��ͼ��
WM_HWIN  hWinLog;       //��־��ѯ
WM_HWIN  hWinDevinfo;   //������Ϣ
WM_HWIN  hWinOthers;    //��������
//�����˵�
WM_HWIN  hWinSetapp;    //��������Ӧ�õľ��
WM_HWIN  hWinOthersapp; //��������Ӧ�õľ��
WM_HWIN  hWinPassv;     //����ȷ�Ͻ���ľ��
uint32_t USERPASS;      //�û�������

extern GUI_CONST_STORAGE GUI_BITMAP bmMAIN_1;
extern GUI_CONST_STORAGE GUI_BITMAP bmMAIN_2;
extern GUI_CONST_STORAGE GUI_BITMAP bmMAIN_3;
extern GUI_CONST_STORAGE GUI_BITMAP bmMAIN_4;
extern GUI_CONST_STORAGE GUI_BITMAP bmMAIN_5;
uint8_t	s_ucSelIconIndex = 0;   //ͼ���ָ��

static const BITMAP_ITEM _aBitmapItem[] = 
{
	{&bmMAIN_1,    "װ��״̬"},
	{&bmMAIN_2,    "��������"},
	{&bmMAIN_3,    "��־��ѯ"},
	{&bmMAIN_4,    "������Ϣ"},
	{&bmMAIN_5,    "��������"},
};


static const GUI_WIDGET_CREATE_INFO _aDialogCreate0[] = 
{
	{ WINDOW_CreateIndirect,      "",ID_WINDOW_0, 0, 0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "���˵�",            	ID_TEXT_0, 12,	2,  150,	32, GUI_TA_HCENTER|GUI_TA_VCENTER, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", ID_TEXT_1, 180,	9,  200,	16, GUI_TA_VCENTER, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",  	ID_TEXT_2, 180,	23, 200,	16, GUI_TA_VCENTER, 0x64, 0},
};


static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	WM_MESSAGE pMsgInfo;
	int NCode, Id;
	switch (pMsg->MsgId) 
		{
		case WM_PAINT:
			GUI_SetColor(GUI_BLUE);
		  GUI_FillRect(0, 39, 800, 480);
			//GUI_DrawGradientV(0, 39, 800, 480, GUI_LIGHTBLUE, GUI_BLUE);
		  GUI_SetPenSize(1);
		  GUI_SetColor(GUI_COL_TITLE2);
		  GUI_DrawLine(170,8,170,34);
		  GUI_DrawLine(0,37,800,37);
		  GUI_SetColor(GUI_COL_TITLE3);
		  GUI_DrawLine(0,38,800,38);
			break;
		case WM_INIT_DIALOG:
			WINDOW_SetBkColor(pMsg->hWin, GUI_COL_TITLE1);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetTextColor(hItem, GUI_WHITE);
			TEXT_SetFont(hItem, &GUI_FontHZ32);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
			TEXT_SetTextColor(hItem, GUI_GRAY_11);
			TEXT_SetFont(hItem, &GUI_FontHZ16);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
			TEXT_SetTextColor(hItem, GUI_GRAY_11);
			TEXT_SetFont(hItem, &GUI_Font13B_1);
			break;
		case MSG_SetUP:
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
			break;
		case MSG_SetDOWM:
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
			break;	
		case MSG_SetLEFT:
			GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
			break;
		case MSG_SetRIGHT:
			GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
			break;	
		case MSG_SetENTER:
			pMsgInfo.MsgId = WM_NOTIFY_PARENT;
			pMsgInfo.hWinSrc = hWinIconMenu;
			pMsgInfo.Data.v = WM_NOTIFICATION_RELEASED;
			WM_SendMessage(pMsg->hWin, &pMsgInfo);	
			break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinMainMenu))
			{	
				GUI_EndDialog(hWinMainMenu, 0); 
				DevSys.Login_cnt  = 0;
				Topapp();//���ص����㴰��
			}
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);     
			NCode = pMsg->Data.v;  
			switch (Id) 
			{
				case GUI_ID_ICONVIEW0:
					switch (NCode) 
					{
						case  WM_NOTIFICATION_RELEASED:
							s_ucSelIconIndex  = ICONVIEW_GetSel(pMsg->hWinSrc);
							switch( s_ucSelIconIndex)
							{
								case 0:
									if(WM_IsWindow(hWinMainMenu))
									{	
										GUI_EndDialog(hWinMainMenu, 0);MenuSta();	//����װ��״̬Ŀ¼
									}
								   break;	
								case 1:
									if(WM_IsWindow(hWinMainMenu))
									{	
										GUI_EndDialog(hWinMainMenu, 0);	MenuSet();//����������ô���
									}
									break;	
								case 2:	
									if(WM_IsWindow(hWinMainMenu))
									{	
										GUI_EndDialog(hWinMainMenu, 0);APP_Setup(13);//���뵽����APP����
									}	
								  break;	
								case 3:		
										if(WM_IsWindow(hWinMainMenu))
									{	
										GUI_EndDialog(hWinMainMenu, 0);APP_Setup(14);//���뵽����APP����
									}	
								  break;
								case 4:		
										if(WM_IsWindow(hWinMainMenu))
									{	
										GUI_EndDialog(hWinMainMenu, 0);APP_Setup(15);//���뵽����APP����
									}	
								  break;	
								default:
									break;
							}	
						 break;
					}
				break;//end GUI_ID_ICONVIEW0:
			}
			break;//end WM_NOTIFY_PARENT:
	 case WM_PRE_PAINT:		
		 WM_SetFocus(hWinIconMenu);
		 break;
		default:
			WM_DefaultProc(pMsg);
		  break;
	}
}



void Menuapp(void) 
{
	uint8_t i;
	hWinMainMenu = GUI_CreateDialogBox(_aDialogCreate0, GUI_COUNTOF(_aDialogCreate0), _cbDialog, WM_HBKWIN, 0, 0);
	hWin_msgto   = hWinMainMenu;
	hWinIconMenu = ICONVIEW_CreateEx(
	             84, 					   	
						   85, 						
							 632,    				
							 330, 					
	             hWinMainMenu, 				  
							 WM_CF_SHOW | WM_CF_HASTRANS,     
	             0,                
							 GUI_ID_ICONVIEW0, 	
							 128, 				    
							 128);	
	ICONVIEW_SetFont(hWinIconMenu, &GUI_FontHZ24);
	for (i = 0; i < GUI_COUNTOF(_aBitmapItem); i++) 
	{	
		ICONVIEW_AddBitmapItem(hWinIconMenu, _aBitmapItem[i].pBitmap, _aBitmapItem[i].pText);
	}
	ICONVIEW_SetBkColor(hWinIconMenu, ICONVIEW_CI_SEL, GUI_WHITE| 0x80000000);
	ICONVIEW_SetSpace(hWinIconMenu, GUI_COORD_Y, 40);
	ICONVIEW_SetSpace(hWinIconMenu, GUI_COORD_X, 40);
	ICONVIEW_SetIconAlign(hWinIconMenu, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
	ICONVIEW_SetSel(hWinIconMenu,s_ucSelIconIndex);
}





