#ifndef __FMC_H
#define __FMC_H
#include "sys.h"

#define	LCD_LED 	PCout(12)  		//LCD����    		
#define LCD_RST 	PDout(13)

#define LCD_BASE        ((u32)(0x60000000 | 0x0000FFFF))//LCD��ַ�ṹ��
#define TFTLCD             ((LCD_TypeDef *) LCD_BASE)
typedef struct
{
	//vu8 LCD_REG;
	vu8 LCD_RAM;
	vu8 LCD_REG;
} LCD_TypeDef;


	 


void	FMC_Init(void);


void LCD_CmdWrite(vu8 regval);
void LCD_DataWrite(vu16 data);
u8 	 LCD_DataRead(void);
u8 	 LCD_StatusRead(void);
u16 LCD_ReadPoint(u16 Xpos,u16 Ypos);

void TFTLCD_Clear(u16 Color);
void LCD_WriteRAM_Prepare(void);
void LCD_SetCursor(u16 Xpos, u16 Ypos);



void TFTLCD_Init(void );



void LCD_BackLightOn(void);
void LCD_BackLightOff(void);
void LCD_BackLightPwm(u8 pwm);
void LCD_DisplayOn(void);	
void LCD_DisplayOff(void);





//void ShowString(u8 x,u8 y,u8 p);//ָ��λ��λ����ʾһ���ַ���
//void ShowString_H(u8 x,u8 y,char *p);//ָ��λ��λ����ʾһ�������ַ���
//void ShowString_B(u8 x,u8 y,char *p);//ָ��λ����ʾ�Ӵ��ַ���
//void ShowString_L(u8 x,u8 y,char *p);//ָ��λ����ʾ�Ӵַ����ַ���

//void ShowChar(u8 x,u8 y,char *p,u8 count);//ָ��λ��λ����ʾһ���ַ���
//void ShowChar_H(u8 x,u8 y,char *p,u8 count);//ָ��λ��λ����ʾһ�������ַ���
//void ShowChar_B(u8 x,u8 y,char *p,u8 count);//ָ��λ����ʾ�Ӵ��ַ���
//void ShowChar_HB(u8 x,u8 y,char *p,u8 count);//ָ��λ����ʾ�Ӵַ����ַ���
//void ShowChar_L(u8 x,u8 y,char *p,u8 count);//ָ��λ����ʾ�Ӵַ����ַ���

//void ShowxNum(u8 x,u8 y,u32 num,u8 len,u8 mode);
//void ShowxNum_H(u8 x,u8 y,u32 num,u8 len,u8 mode);
//void ShowxNum_B(u8 x,u8 y,u32 num,u8 len,u8 mode);
//void ShowxNum_L(u8 x,u8 y,u32 num,u8 len,u8 mode);
//	
//void ShowSymbol(u8 x,u8 y,u8 symbol);



#endif
