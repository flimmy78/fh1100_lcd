
#include "mainmenu.h"
#include "sys.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmleft;
extern GUI_CONST_STORAGE GUI_BITMAP bmright;
extern uint8_t	s_ucSelIconIndex2;
extern u8 PTP_STA_CONST[10][10];	
extern u8 PTP_MODE_CONST[9][12];		
#define BUF_S_LEN 36


static void Showsta(WM_MESSAGE * pMsg)
{
	int i =0;
	char tbuf[BUF_S_LEN]={0};
	DEV_STA	 					ptp1_sta_synch,ptp1_sta_signal,ptp1_time_valid,ptp1_dif_sign,ptp1_LS_come,ptp1_LS_sign;
	//DEV_STA ptp1_dif_cf;
	u32 ptp1_crt_field;
	u8			ptp1_twostep;
	u8			ptp1_workmode,ptp1_state;
	u8 			ptp1_bestmac[12];
	u32 		ptp1_dif_s,ptp1_dif_ns,ptp1_link_dly;
	u16 		offset2,offset3,offset4;
	ptp1_sta_signal				=		(DEV_STA) (CAN1_RX0_BUF[P2_STA_OFFSET+2]&0x01);						//PTP2�ź�״̬	
	ptp1_sta_synch				=		(DEV_STA)((CAN1_RX0_BUF[P2_STA_OFFSET+2]&0x02)>>1);				//ͬ��״̬	
	ptp1_time_valid				=		(DEV_STA)((CAN1_RX0_BUF[P2_STA_OFFSET+2]&0x04)>>2);			  //ʱ������״̬ 
//	ptp1_time_valid				=		(DEV_STA)((CAN1_RX0_BUF[P2_STA_OFFSET+5]&0x10)>>3);				//ptp1Դʱ������״̬
	ptp1_twostep					=		CAN1_RX0_BUF[P2_STA_OFFSET+3];											//������˫��
	ptp1_workmode					=		CAN1_RX0_BUF[P2_STA_OFFSET+4];						          //����ģʽ
	ptp1_state				    =		CAN1_RX0_BUF[P2_STA_OFFSET+5];						          //PTP״̬
	
	ptp1_LS_come					=		(DEV_STA)((CAN1_RX0_BUF[P2_STA_OFFSET+1]&0x80)>>7);		//����Ԥ��
	ptp1_LS_sign					=		(DEV_STA)((CAN1_RX0_BUF[P2_STA_OFFSET+1]&0x40)>>6);		//�����־
	
	offset2=P2_STA_OFFSET+8;	
	ptp1_dif_sign					=		(DEV_STA)((CAN1_RX0_BUF[offset2]&0x80)>>7);	//ptp1ԴPPS��Բ����
  ptp1_dif_s						=		((CAN1_RX0_BUF[offset2]&0x7f)<<24)+(CAN1_RX0_BUF[offset2+1]<<16)+(CAN1_RX0_BUF[offset2+2]<<8)
														+(CAN1_RX0_BUF[offset2+3]);									//ptp1ԴPPS��Բ�ֵ��λ
	ptp1_dif_ns						=		((CAN1_RX0_BUF[offset2+4]<<24)+(CAN1_RX0_BUF[offset2+5]<<16)+(CAN1_RX0_BUF[offset2+6]<<8)
														+(CAN1_RX0_BUF[offset2+7]))*OCX_PERIOD;									//ptp1ԴPPS��Բ�ֵ����λ
	ptp1_dif_ns =ptp1_dif_ns&0x7fffffff;													
	if(ptp1_dif_ns>999999999) ptp1_dif_ns=999999999;												//���ֵ��999 999 999
	
	offset3=P2_STA_OFFSET+16;

	ptp1_bestmac[0]		=		CAN1_RX0_BUF[offset3];
	ptp1_bestmac[1]		=		CAN1_RX0_BUF[offset3+1];
	ptp1_bestmac[2]		=		CAN1_RX0_BUF[offset3+2];
	ptp1_bestmac[3]		=		CAN1_RX0_BUF[offset3+3];
	ptp1_bestmac[4]		=		CAN1_RX0_BUF[offset3+4];
	ptp1_bestmac[5]		=		CAN1_RX0_BUF[offset3+5];
	
	
	
	offset4=P2_STA_OFFSET+24;
	
  ptp1_link_dly					=		(CAN1_RX0_BUF[offset4]<<24)+(CAN1_RX0_BUF[offset4+1]<<16)+(CAN1_RX0_BUF[offset4+2]<<8)
														+(CAN1_RX0_BUF[offset4+3]);									//ptp1Դ������
	if(ptp1_link_dly>999999999) ptp1_link_dly	=	999999999;												//���ֵ��999 999 999		
//ptp1_dif_cf						=		(DEV_STA)((CAN1_RX0_BUF[offset4+4]&0x80)>>7);	//ptp1Դ���������	
//	ptp1_crt_field				=		((CAN1_RX0_BUF[offset4+4]&0x7f)<<24)+(CAN1_RX0_BUF[offset4+5]<<16)+(CAN1_RX0_BUF[offset4+6]<<8)
//														+(CAN1_RX0_BUF[offset4+7]);									//ptp1Դ��·��ʱ
	if(ptp1_crt_field>999999999) ptp1_crt_field		=	999999999;												//���ֵ��999 999 999
	
	
	  /*-----------------------0ͬ��״̬-----------------------------------------------------------------------------*/
		if(ptp1_sta_synch==STA_OK)
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"ͬ��");
		else 
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"ʧ��");
		
		/*
		-------------------------1����ģʽ-----------------------------------------------------------------------------*/
		ptp1_workmode = ptp1_workmode%9;
		memcpy((char*)tbuf,(char *)&(PTP_MODE_CONST[ptp1_workmode][0]),12);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf);
		
				/*
		-------------------------2PTP״̬-----------------------------------------------------------------------------*/
		memset(tbuf,0,BUF_S_LEN);
		ptp1_state = ptp1_state%10;
		memcpy((char*)tbuf,(char *)&(PTP_STA_CONST[ptp1_state][0]),10);
		if(ptp1_state==9)
			{
				if(ptp1_twostep)
					sprintf( (char*)(tbuf+10),"(������)");
				else
					sprintf( (char*)(tbuf+10),"(һ����)");
			}
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf);
		
			/*
		-------------------------3��·�ӳ�-----------------------------------------------------------------------------*/
		memset(tbuf,0,BUF_S_LEN);
		sprintf( (char*)(tbuf),"%03d����",ptp1_link_dly);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf);
		
		/*
		-------------------------4ͨ����ֵ-----------------------------------------------------------------------------*/
		memset(tbuf,0,BUF_S_LEN);
		if(ptp1_dif_sign==STA_OK)		
			sprintf( (char*)(tbuf),"+%d��% 03d����\n",ptp1_dif_s,ptp1_dif_ns);
		else
			sprintf( (char*)(tbuf),"-%d��% 03d����\n",ptp1_dif_s,ptp1_dif_ns);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf);
		/*
		-------------------------5�ź�״̬-----------------------------------------------------------------------------*/
		if(ptp1_sta_signal==STA_OK)	
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"����");
		else 
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"�쳣");
		
			/*
		-------------------------6����״̬-----------------------------------------------------------------------------*/
		if(ptp1_LS_come)
		{
			if(ptp1_LS_sign==0)
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"������");
			else
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"������");
		}
		else
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"������");
			/*
		-------------------------7ʱ��������״̬-----------------------------------------------------------------------------*/
		if(ptp1_time_valid==STA_OK)	
		 TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"����");
		else
		 TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"�쳣");
			/*
		-------------------------8�����ʱ��-----------------------------------------------------------------------------*/
		memset(tbuf,0,BUF_S_LEN);
		sprintf( (char*)(tbuf),"%02X-%02X-%02X-%02X-%02X-%02X",ptp1_bestmac[0],ptp1_bestmac[1],ptp1_bestmac[2],ptp1_bestmac[3],ptp1_bestmac[4],ptp1_bestmac[5]);	
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf);
		
}




void _cbCallback28(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_MSTAAPP(8);/*���������ʹ���*/
		  GUI_DrawLine(XPOS1-20,YPOS0-5-YGAPS,XPOS1-20,YPOS0+YGAPS*8);
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



