//////////////////////////////////////////////////////////////////////////////////	 
//��������:2016/5/25
//�汾��V1.0
//ZX						  
////////////////////////////////////////////////////////////////////////////////// 
#ifndef __TFTLCD_H
#define __TFTLCD_H
#include "sys.h"
#include "fmc.h"
//LCD��Ҫ������
typedef struct  _lcd_dev
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u8  dir;				//���������������ƣ�0��������1��������	
	u16	wramcmd;		//��ʼдgramָ��
}lcd_dev; 

extern  struct _lcd_dev lcddev;	

extern u16 POINT_COLOR;	//������ɫ
extern u16 BACK_COLOR;  //����ɫ 
extern u16 TextColor;

#define WHITE      0xFFFF		//��
#define BLACK      0x0000	  //��	
#define BLUE       0x001F  	
#define BRED       0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED        0xF800
#define MAGENTA    0xF81F
#define GREEN      0x07E0
#define CYAN       0x7FFF
#define YELLOW     0xFFE0
#define BROWN 		 0XBC40 //��ɫ
#define BRRED 		 0XFC07 //�غ�ɫ
#define GRAY  		 0X8430 //��ɫ
//GUI��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY      0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)



void MYLCD_DEV_INIT(void);


void LCD_DrawPoint(u16 x,u16 y);
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);
u32 LCD_Pow(u8 m,u8 n);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,char *p);
       

void UI_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, uint16_t *_pColor);




#endif
