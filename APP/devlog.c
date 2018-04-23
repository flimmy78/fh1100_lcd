#include "mainmenu.h"
#include "sys.h"


#define WIAT_OUT   30

extern u8 LOG_POINTRES[100][40];
extern u8 LOG_POINT[100][40];



extern GUI_CONST_STORAGE GUI_BITMAP bmpic1;

extern void PAINTWIN_SETAPP(WM_MESSAGE * pMsg);

static uint8_t logbuff[3200] ;
	
void CAN_SEND_APP13(uint8_t req) 			
{
	uint32_t send_id=0x03101900;			//CANID
	uint8_t  can_err;
	send_id|=(req);
	can_err=CAN1_Send_REQ(send_id);
	CAN_SHOW_ERR(can_err);
}
	
static uint8_t REC_Handler_APP13(uint8_t reqindex) 			
{
	
	uint16_t mask=0x1019; 			 //2.->>CAN_BUF��������
	uint16_t FRAME_REV;
	int i ,t=0;
	CAN_SEND_APP13(reqindex+1);
	myprintf("��־����:%d\n",reqindex);
	while(t<WIAT_OUT)
	{
		if ((CAN1_RX1_STA&0xffff0000)>0)  							//CAN1_R1״̬��־
			{
				if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//�յ��ı����Ƕ�Ӧ�ۣ���Ӧ���ڣ���Ӧ���͵ı���
					{	
						FRAME_REV=(CAN1_RX1_STA&0xffff)>>4;		//�յ���־����=(�ֽ���/16)
						for(i=0;i<FRAME_REV;i++)
						{
							memcpy(&logbuff[(reqindex+(FRAME_REV-1-i))*10],&CAN1_RX1_BUF[LOG_OFFSET+i*16],10);
						}
						//memcpy(void *dest, const void *src, size_t n);
						//myprintf("CAN_REC �յ���־:%d��\n",FRAME_REV);
						
						CAN1_RX1_STA=0;
						myprintf("t:%d\n",t);
						return FRAME_REV;
					}	
				CAN1_RX1_STA=0;
			}
		GUI_Delay(1);
		t++;
  }
	myprintf("��־����:%d����ʱ\n",reqindex);
	return 0;
}





 void _cbCallback13(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
	HEADER_Handle hHeader;
	int i;
	char text3buf[30]={0};
  static uint16_t  logitems;
  static uint16_t reqindex,remain,fold,reced,temp,value;
	static uint16_t wrong_timeout,timecnt = 0;
	static char tbuf1[4] ;//Ҫ�����ַ�����������
  static char tbuf2[20] ;
	static char tbuf3[3] ;
	static char tbuf4[40] ;
	static uint16_t TBUFF_LENTH = 40;
 
  
  
	
  hDlg = pMsg->hWin;

   
		  
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
      break;
  case WM_INIT_DIALOG:
			PAINTWIN_SETAPP(pMsg);//���ñ�����
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03);
			TEXT_SetTextColor(hItem, GUI_WHITE);
			//TEXT_SetFont(hItem, &GUI_FontHZ32);
			//TEXT_SetTextAlign(hItem,   GUI_TA_VCENTER);
			TEXT_SetFont(hItem, &GUI_FontHZ16);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_APP0);  
			hHeader = LISTVIEW_GetHeader(hItem);
			HEADER_SetFont(hHeader, &GUI_FontHZ16);
			logitems = CAN1_RX0_BUF[T0_OFFSET+6];
			if(DevSys.quality>5)
				logitems=0;
			if(logitems>200){
				logitems=200;
			}		
			reced = 0;
			wrong_timeout = 0;
			timecnt = 0x55;
			myprintf("��־����:%d\n",logitems);
			//LISTVIEW_AddColumn(hItem, 1, "", GUI_TA_CENTER|TEXT_CF_VCENTER);//������˳�ʼ�����⣬�ӵ�һ����������
			LISTVIEW_AddColumn(hItem, 70, "���", GUI_TA_CENTER|TEXT_CF_VCENTER);
			LISTVIEW_AddColumn(hItem, 170, "ʱ��", GUI_TA_CENTER|TEXT_CF_VCENTER);
			LISTVIEW_AddColumn(hItem, 60, "����", GUI_TA_CENTER|TEXT_CF_VCENTER);
			LISTVIEW_AddColumn(hItem, 500,  "        ��־����" , TEXT_CF_LEFT|TEXT_CF_VCENTER);
			LISTVIEW_SetRowHeight(hItem, 55);
			LISTVIEW_SetGridVis(hItem,1);
			SCROLLBAR_CreateAttached(hItem, SCROLLBAR_CF_VERTICAL);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_APP0);		
			PROGBAR_SetMinMax(hItem,0,200);		
			PROGBAR_SetFont(hItem, &GUI_Font8x16);	
			PROGBAR_SetBarColor(hItem, 0, GUI_GREEN);
			// PROGBAR_SetBarColor(hItem, 1, GUI_RED);
			//reqindex = 0;
      break;
		case WM_PRE_PAINT:	
			hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_APP0);  
			WM_SetFocus (hItem);
		  //WM_RestartTimer(pMsg->Data.v, 10);	//������ʱ����ע�⣡
			break;
		 /************************************************************
				������Ϣ���洦��
				*/	
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
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp)){	
				 timecnt = 0;GUI_EndDialog(hDlg, 0);Menuapp();//���ص����˵�����
			}
			break;	
		/************************************************************
			���ù��ܵ�ʵ��
			*/	
		case WM_TIMER:
		   /*  ===================================
					 =========0��־�����============================*/
		
		    if(timecnt!=0x55)//��ʱ���������ڳ�ʼ��
				{
					myprintf("��ʱ��û�г�ʼ����\n");
					WM_RestartTimer(pMsg->Data.v, 50);	//������ʱ����ע�⣡
					break; 
				}
				if(logitems<1){
						hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03);
            TEXT_SetTextAlign(hItem,TEXT_CF_RIGHT);						
						TEXT_SetText(hItem,"������־" );
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_APP0  ));
						myprintf("��ʱû����־\n");
				}
				else{
					  WM_HWIN PRODlg;
				/*  ===================================
					 =========��һ�β�ѯ���Ͳ�ѯ��ʧ�����============================*/
					  myprintf("��־��ѯ(%d/%d)\n",reced,logitems);
					  //TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03),"��ȡ��־��..." );
						if((reced<logitems)&&(wrong_timeout<1))
						{
							/*  
					      ==��һ�β�ѯ���ж�ʧ��������ʱ�����ٴβ�ѯ==*/
						
							
							fold   =logitems/8; //ÿ��8��������
							remain =logitems%8; //ÿ��8��������
							myprintf("logitems = %d,fold = %d,remain = %d\n",logitems,fold,remain);
								//������־
							reced    = 0;//�յ�������
							reqindex = 0;//��ѯ��ָ��
							CAN1_RX1_STA=0;
							PRODlg = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_APP0  );
							for (i = 0; i <fold; i++) 
								{
									reced+=REC_Handler_APP13(reqindex);
									reqindex+=8;
									PROGBAR_SetValue(PRODlg, reced);
									myprintf("��־�յ�:%d\n",reced);
									if(reced==0)//�쳣��������ѯ����
										i=fold;
								}	
							if(remain>0)//�ٲ�ѯ���µ���
								{
									reced+=REC_Handler_APP13(reqindex);
									PROGBAR_SetValue(PRODlg, reced);
									myprintf("reced = %d\n",reced);
								}
							myprintf("��־������ɣ����յ�:%d/%d��\n",reced,logitems);
								if(reced<logitems)
								{
							    wrong_timeout++;
									sprintf((char*)text3buf,"��ȡʧ��:%d/%d,��%d������",reced,logitems,wrong_timeout);
									TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03),text3buf );
									WM_RestartTimer(pMsg->Data.v, 10);	//������ʱ����ע�⣡
								}
								else
								{
									wrong_timeout = 0;
									WM_RestartTimer(pMsg->Data.v, 10);	//������ʱ����ע�⣡
								}
						 }
						else
						{
							hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_APP0);  
							timecnt = 0;
								/*  
					      ==������������ʼ������־==*/
							memset(text3buf,0,30);
							if(wrong_timeout<1)
								{
									sprintf((char*)text3buf,"��־��ȡ�ɹ�,��%d����",logitems);
									TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03),text3buf );
									WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_APP0  ));
								}
							else
								{
									TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03),"��־��ȡʧ��:" );
								}
							for (i = 0; i <reced; i++) 
									{
										
										LISTVIEW_AddRow(hItem, 0);				
									}		
								//������־
							for (i = 0; i <reced; i++) 
								{  
								 
									int k;
									k = reced-1-i;
								 sprintf((char*)tbuf1,"%03d",logbuff[k*10]);
								 sprintf((char*)tbuf2,"%04d-%02d-%02d\n%02d:%02d:%02d",(logbuff[k*10+1]<<8)+logbuff[k*10+2],logbuff[k*10+3],logbuff[k*10+4],
								 logbuff[k*10+5],logbuff[k*10+6],logbuff[k*10+7]);
								 sprintf((char*)tbuf3,"%d",logbuff[k*10+8]);
								 temp  = logbuff[k*10+8];
								 value = logbuff[k*10+9]; 
						      switch(temp){
										case 7:
										case 8:	
											memcpy((char*)tbuf4,(char *)&(LOG_POINTRES[temp][0]),TBUFF_LENTH);//7,8����(GPS)����״̬:
												if(value == 1)
													strcat((char*)tbuf4,"�쳣");
												else if(value == 3)
													strcat((char*)tbuf4,"��·");
												else if(value == 4)
													strcat((char*)tbuf4,"��·");
												else
													strcat((char*)tbuf4,"�ָ�");
											break;
										case 38:
											memcpy((char*)tbuf4,(char *)&(LOG_POINTRES[temp][0]),TBUFF_LENTH);//38,ʱ��Դѡ����:
												switch(value){
													case 1:
														strcat((char*)tbuf4,"GPS");
														break;
													case 2: case 5:
														strcat((char*)tbuf4,"IRIG-B1");
														break;
													case 3: case 6:
														strcat((char*)tbuf4,"IRIG-B2");
														break;
													case 4:
														strcat((char*)tbuf4,"����ʱ");
														break;
													case 7:
														strcat((char*)tbuf4,"PTP1");
														break;
													case 8:
														strcat((char*)tbuf4,"PTP2");
														break;
													default:
														strcat((char*)tbuf4,"����");
														break;
												}
												break;
										case 39: case 40:
										case 43: case 44:
										case 45: case 46:	
											memcpy((char*)tbuf4,(char *)&(LOG_POINTRES[temp][0]),TBUFF_LENTH);//39-46,Դ���ȼ�:		
											switch(value){
													case 1:
														strcat((char*)tbuf4,"1");
														break;
													case 2: 
														strcat((char*)tbuf4,"2");
														break;
													case 3: 
														strcat((char*)tbuf4,"3");
														break;
													case 4:
														strcat((char*)tbuf4,"4");
														break;
													case 5:
														strcat((char*)tbuf4,"5");
														break;
													case 6:
														strcat((char*)tbuf4,"6");
														break;
													default:
														strcat((char*)tbuf4,"Ĭ��");
														break;
											}
											break;
										default://����Ҫ���⴦���
											if(value == 0)
												memcpy((char*)tbuf4,(char *)&(LOG_POINTRES[temp][0]),TBUFF_LENTH);
											else if(value == 1)
												memcpy((char*)tbuf4,(char *)&(LOG_POINT[temp][0]),TBUFF_LENTH);
											break;							 
								 }
								 
								 
								 
								 LISTVIEW_SetItemText(hItem,0,i,tbuf1);
								 LISTVIEW_SetItemText(hItem,1,i,tbuf2);
								 LISTVIEW_SetItemText(hItem,2,i,tbuf3);
								 LISTVIEW_SetItemText(hItem,3,i,tbuf4);
								}		//end LISTVIEW��ʼ��
							LISTVIEW_SetSel(hItem,0);
						}			//end ��־�����������			
		    }	//end ��־��¼��ʼ�����			
			break;		
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}




/*************************** End of file ****************************/
