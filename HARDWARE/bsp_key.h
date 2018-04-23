/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_key.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "sys.h"

///* ����Ӧ�ó���Ĺ��������������� */
#define KEY1_U 		KEY_1_DOWN
#define KEY2_D 		KEY_2_DOWN
#define KEY3_L		KEY_3_DOWN
#define KEY4_R 		KEY_4_DOWN
#define KEY5_E 		KEY_5_DOWN
#define KEY6_Q 		KEY_6_DOWN
#define KEY7_INC 	KEY_7_DOWN
#define KEY8_DEC 	KEY_8_DOWN

typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_1_DOWN,				/* 1������ */
	KEY_1_UP,				  /* 1������ */
	KEY_1_LONG,				/* 1������ */

	KEY_2_DOWN,				/* 2������ */
	KEY_2_UP,				  /* 2������ */
	KEY_2_LONG,				/* 2������ */

	KEY_3_DOWN,				/* 3������ */
	KEY_3_UP,				  /* 3������ */
	KEY_3_LONG,				/* 3������ */

	KEY_4_DOWN,				/* 4������ */
	KEY_4_UP,				  /* 4������ */
	KEY_4_LONG,				/* 4������ */

	KEY_5_DOWN,				/* 5������ */
	KEY_5_UP,			  	/* 5������ */
	KEY_5_LONG,				/* 5������ */

	KEY_6_DOWN,				/* 6������ */
	KEY_6_UP,				  /* 6������ */
	KEY_6_LONG,				/* 6������ */

	KEY_7_DOWN,				/* 7������ */
	KEY_7_UP,				  /* 7������ */
	KEY_7_LONG,			 	/* 7������ */

	KEY_8_DOWN,				/* 8������ */
	KEY_8_UP,				  /* 8������ */
	KEY_8_LONG,				/* 8������ */

}KEY_ENUM;
/* ���ⲿ���õĺ������� */
void bsp_InitKey(void);
void bsp_KeyScan(void);
uint8_t KEY_Scan(void);
//void bsp_PutKey(uint8_t _KeyCode);
//uint8_t bsp_GetKey2(void);
//uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
//void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
//void bsp_ClearKey(void);



#endif

