//////////////////////////////////////////////////////////////////////////////////	 
//��������:2016/5/25
//�汾��V1.0
//ZX						  
////////////////////////////////////////////////////////////////////////////////// 
#include "tftlcd.h"
#include "delay.h" 
#include "sys.h" 
#include "fmc.h"
u16 POINT_COLOR	=0x0000;	//������ɫ
u16 BACK_COLOR	=0xFFE0;  //����ɫ 
u16 TextColor		;
struct _lcd_dev lcddev;	




void MYLCD_DEV_INIT(void)
{
  lcddev.width	=	800;
	lcddev.height	=	480;
	lcddev.dir		= 1;				//���������������ƣ�0��������1��������	
	lcddev.wramcmd=	0x02;	
}




void TFTLCD_Clear(u16 Color)
{
  u32 index = 0;
  
  LCD_SetCursor(0,0); 

  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

  for(index = 0; index < 384000; index++)
  {
    LCD_DataWrite(Color);
  }
 // while(1);  
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
*******************************************************************************/

		 




//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_DataWrite(POINT_COLOR);
}
//���ٻ���
//x,y:����
//color:��ɫ
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_DataWrite(color); 
}	 




//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
		LCD_SetCursor(sx,i);      				//���ù��λ�� 
		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
		for(j=0;j<xlen;j++)LCD_DataWrite(color);	//��ʾ��ɫ 	    
	}	 
}  
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)LCD_DataWrite(color[i*width+j]);//д������ 
	}		  
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    	   	 	  
}   
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,char *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}


///**
// * @brief  ������ģ�����ź����ģ��1�����ص���8������λ����ʾ
//										0x01��ʾ�ʼ���0x00��ʾ�հ���
// * @param  in_width ��ԭʼ�ַ����
// * @param  in_heig ��ԭʼ�ַ��߶�
// * @param  out_width �����ź���ַ����
// * @param  out_heig�����ź���ַ��߶�
// * @param  in_ptr ���ֿ�����ָ��	ע�⣺1pixel 1bit
// * @param  out_ptr �����ź���ַ����ָ�� ע��: 1pixel 8bit
// *		out_ptrʵ����û������������ĳ���ֱ�������ȫ��ָ��zoomBuff��
// * @param  en_cn ��0ΪӢ�ģ�1Ϊ����
// * @retval ��
// */
//void LCD_zoomChar(uint16_t in_width,	//ԭʼ�ַ����
//									uint16_t in_heig,		//ԭʼ�ַ��߶�
//									uint16_t out_width,	//���ź���ַ����
//									uint16_t out_heig,	//���ź���ַ��߶�
//									uint8_t *in_ptr,	//�ֿ�����ָ��	ע�⣺1pixel 1bit
//									uint8_t *out_ptr, //���ź���ַ����ָ�� ע��: 1pixel 8bit
//									uint8_t en_cn)		//0ΪӢ�ģ�1Ϊ����	
//{
//	uint8_t *pts,*ots;
//	//����Դ��ģ��Ŀ����ģ��С���趨����������ӣ�����16��Ϊ�˰Ѹ�������ת�ɶ�������
//	unsigned int xrIntFloat_16=(in_width<<16)/out_width+1; 
//  unsigned int yrIntFloat_16=(in_heig<<16)/out_heig+1;
//	
//	unsigned int srcy_16=0;
//	unsigned int y,x;
//	uint8_t *pSrcLine;
//	uint8_t tempBuff[1024] = {0};
//	u32			uChar;
//	u16			charBit = in_width / 8;
//	u16			Bitdiff = 32 - in_width;
//	
//	//�������Ƿ�Ϸ�
//	if(in_width >= 32) return;												//�ֿⲻ������32����
//	if(in_width * in_heig == 0) return;	
//	if(in_width * in_heig >= 1024 ) return; 					//����������� 32*32
//	
//	if(out_width * out_heig == 0) return;	
//	if(out_width * out_heig >= ZOOMMAXBUFF ) return; //����������� 128*128
//	pts = (uint8_t*)&tempBuff;
//	
//	//Ϊ�������㣬�ֿ��������1 pixel 1bit ӳ�䵽1pixel 8bit
//	//0x01��ʾ�ʼ���0x00��ʾ�հ���
//	if(en_cn == 0x00)//Ӣ��
//	{
//		//������16 * 24�ֿ���Ϊ���ԣ�������С���ֿ����и������д�������ӳ��Ϳ���
//		//Ӣ�ĺ������ֿ����±߽粻�ԣ����ڴ˴ε�������Ҫע��tempBuff��ֹ���
//			pts+=in_width*4;
//			for(y=0;y<in_heig;y++)	
//			{
//				uChar = *(u32 *)(in_ptr + y * charBit) >> Bitdiff;
//				for(x=0;x<in_width;x++)
//					{
//						*pts++ = (uChar >> x) & 0x01;
//					}
//			}		
//	}
//	else //����
//	{
//			for(y=0;y<in_heig;y++)	
//			{
//				/*Դ��ģ����*/
//				uChar = in_ptr [ y * 3 ];
//				uChar = ( uChar << 8 );
//				uChar |= in_ptr [ y * 3 + 1 ];
//				uChar = ( uChar << 8 );
//				uChar |= in_ptr [ y * 3 + 2];
//				/*ӳ��*/
//				for(x=0;x<in_width;x++)
//					{
//						if(((uChar << x) & 0x800000) == 0x800000)
//							*pts++ = 0x01;
//						else
//							*pts++ = 0x00;
//					}
//			}		
//	}

//	//zoom����
//	pts = (uint8_t*)&tempBuff;	//ӳ����Դ����ָ��
//	ots = (uint8_t*)&zoomBuff;	//������ݵ�ָ��
//	for (y=0;y<out_heig;y++)	/*�б���*/
//    {
//				unsigned int srcx_16=0;
//        pSrcLine=pts+in_width*(srcy_16>>16);				
//        for (x=0;x<out_width;x++) /*�������ر���*/
//        {
//            ots[x]=pSrcLine[srcx_16>>16]; //��Դ��ģ���ݸ��Ƶ�Ŀ��ָ����
//            srcx_16+=xrIntFloat_16;			//������ƫ��Դ���ص�
//        }
//        srcy_16+=yrIntFloat_16;				  //������ƫ��Դ���ص�
//        ots+=out_width;						
//    }
//	/*���������ź����ģ����ֱ�Ӵ洢��ȫ��ָ��zoomBuff����*/
//	out_ptr = (uint8_t*)&zoomBuff;	//out_ptrû����ȷ�������������ֱ�Ӹĳ���ȫ�ֱ���ָ�룡
//	
//	/*ʵ�������ʹ��out_ptr����Ҫ������һ�䣡����
//		ֻ����Ϊout_ptrû��ʹ�ã��ᵼ��warning��ǿ��֢*/
//	out_ptr++; 
//}			


///**
// * @brief  �������ź����ģ��ʾ�ַ�
// * @param  Xpos ���ַ���ʾλ��x
// * @param  Ypos ���ַ���ʾλ��y
// * @param  Font_width ���ַ����
// * @param  Font_Heig���ַ��߶�
// * @param  c ��Ҫ��ʾ����ģ����
// * @param  DrawModel ���Ƿ�ɫ��ʾ 
// * @retval ��
// */
//void LCD_DrawChar_Ex(uint16_t Xpos, //�ַ���ʾλ��x
//												uint16_t Ypos, //�ַ���ʾλ��y
//												uint16_t Font_width, //�ַ����
//												uint16_t Font_Heig,  //�ַ��߶� 
//												uint8_t *c,						//��ģ����
//												uint16_t DrawModel)		//�Ƿ�ɫ��ʾ
//{
//  uint32_t index = 0, counter = 0, xpos =0;
//  uint32_t  Xaddress = 0;
//  
//  xpos = Xpos*lcddev.width*3;
//  Xaddress += Ypos;
//  
//  for(index = 0; index < Font_Heig; index++)
//  {
//    
//    for(counter = 0; counter < Font_width; counter++)
//    {
//      if(*c++ == DrawModel)	//������ģ����ɫ���þ�����ʾ������ɫ
//      {
//        *(__IO uint16_t*)(CurrentFrameBuffer + (3*Xaddress) + xpos) = (0x00FFFF & BACK_COLOR);        //GB
//        *(__IO uint8_t*)(CurrentFrameBuffer + (3*Xaddress) + xpos+2) = (0xFF0000 & BACK_COLOR) >> 16; //R
//      }
//      else
//      {
//        *(__IO uint16_t*)(CurrentFrameBuffer + (3*Xaddress) + xpos) = (0x00FFFF & POINT_COLOR);        //GB
//        *(__IO uint8_t*)(CurrentFrameBuffer + (3*Xaddress) + xpos+2) = (0xFF0000 & POINT_COLOR) >> 16; //R
//      }
//      Xaddress++;
//    }
//      Xaddress += (LCD_PIXEL_WIDTH - Font_width);
//  }
//}




//void LCD_DisplayStringLineEx(uint16_t x, 		//�ַ���ʾλ��x
//														 uint16_t y, 				//�ַ���ʾλ��y
//														 uint16_t Font_width,	//Ҫ��ʾ�������ȣ�Ӣ���ַ��ڴ˻�����/2��ע��Ϊż��
//														 uint16_t Font_Heig,	//Ҫ��ʾ������߶ȣ�ע��Ϊż��
//														 uint8_t size,	
//														 uint8_t *ptr,					//��ʾ���ַ�����
//														 uint16_t DrawModel)  //�Ƿ�ɫ��ʾ
//{
//	uint16_t refcolumn = x; //x����
//	uint16_t Charwidth;
//	uint8_t *psr;
//	uint8_t Ascii;	//Ӣ��
//	uint16_t usCh;  //����
//	uint8_t ucBuffer [ 24*24/8 ];	
//	
//	//while ((refcolumn < lcddev.width) && ((*ptr != 0) & (((refcolumn + size) & 0xFFFF) >= size)))
//	while ((refcolumn < lcddev.width) && ((*ptr != 0) ))
//	{
//		if(*ptr > 0x80) //���������
//		{
//			Charwidth = Font_width;
//			usCh = * ( uint16_t * ) ptr;				
//			usCh = ( usCh << 8 ) + ( usCh >> 8 );
//			macGetGBKCode ( ucBuffer, usCh );	//ȡ��ģ����
//			//������ģ����
//			LCD_zoomChar(24,24,Charwidth,Font_Heig,(uint8_t *)&ucBuffer,psr,1); 
//			//��ʾ�����ַ�
//			LCD_DrawChar_Ex(y,refcolumn,Charwidth,Font_Heig,(uint8_t*)&zoomBuff,DrawModel);
//			refcolumn+=Charwidth;
//			ptr+=2;
//		}
//		else
//		{
//				Charwidth = Font_width / 2;
//				Ascii = *ptr - 32;
//				//������ģ����
//				LCD_zoomChar(16,24,Charwidth,Font_Heig,(uint8_t *)&LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height],psr,0);
//			  //��ʾ�����ַ�
//				LCD_DrawChar_Ex(y,refcolumn,Charwidth,Font_Heig,(uint8_t*)&zoomBuff,DrawModel);
//				refcolumn+=Charwidth;
//				ptr++;
//		}
//	}
//}

void UI_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, uint16_t *_pColor)
{
	uint16_t i;

	 LCD_SetCursor(_usX1, _usY1);
	
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	
	
	for (i = 0; i < _usWidth; i++)
	{
		LCD_DataWrite(*_pColor++);
	
		//RA8875_WriteData16(*_pColor++);
	}
	
}




