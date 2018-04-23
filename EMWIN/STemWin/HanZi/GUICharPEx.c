/*
*********************************************************************************************************
*	                                  
*	ģ������ : ������ʾ�ӿ�
*	�ļ����� : GUICharPEx.c
*	��    �� : V1.0
*	˵    �� : ���ֺ�����ʾ��ʽ�ʺ�ʹ�õȿ����壬�ǵȿ��΢���źڵ�������ʾЧ���ϲ
*              ֧�ֵ���
*                1. ֧��12����16����24�����Լ�32�����֡�
*                2. 6X12�����ASCII��8X16�����ASCII��12X24�����ASCII��16X32�����ASCII��
*              �ֿ�洢��ʽ��
*                1. �ֿ�洢��SPI FLASH���档
*
*	�޸ļ�¼ :
*		�汾��    ����           ����         ˵��
*		V1.0    2015-04-24     Eric2013       �װ�
*
*      Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "Mainmenu.h"
#include "includes.h"


/* �������ݻ���, ������ڵ��ڵ�����ģ��Ҫ�Ĵ洢�ռ�*/ 
#define BYTES_PER_FONT      512 
static U8 GUI_FontDataBuf[BYTES_PER_FONT];

/*
*********************************************************************************************************
*	�� �� ��: GUI_GetDataFromMemory
*	����˵��: ��ȡ��������
*	��    ��: pProp  GUI_FONT_PROP���ͽṹ
*             c      �ַ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void GUI_GetDataFromMemory(const GUI_FONT_PROP GUI_UNI_PTR *pProp, U16P c) 
{ 
    U16 BytesPerFont; 
    U32 oft = 0, BaseAdd; 
	U8 code1,code2;
	
	char *font = (char *)pProp->paCharInfo->pData; 

	/* ÿ����ģ�������ֽ��� */
    BytesPerFont = GUI_pContext->pAFont->YSize * pProp->paCharInfo->BytesPerLine; 
    if (BytesPerFont > BYTES_PER_FONT)
	{
		BytesPerFont = BYTES_PER_FONT;
	}
	
	/* Ӣ���ַ���ַƫ�Ƽ��� */ 
    if (c < 0x80)                                                                
    { 
		if(strncmp("A12", font, 3) == 0)     /* 6*12 ASCII�ַ� */
		{
			BaseAdd = 0x1DBE00;
		}
		else if(strncmp("A16", font, 3) == 0) /* 8*16 ASCII�ַ� */
		{
			BaseAdd = 0x1DD780;
		}
		else if(strncmp("A24", font, 3) == 0) /* 12*24 ASCII�ַ� */
		{
			BaseAdd = 0x1DFF00;
		}
		else if(strncmp("A32", font, 3) == 0) /* 24*48 ASCII�ַ� */
		{
			BaseAdd = 0x1E5A50;
		}
		
		oft = (c-0x20) * BytesPerFont + BaseAdd; 
    } 
	/* ���ֺ�ȫ���ַ���ƫ�Ƶ�ַ���� */
    else                                                                           
    { 
		if(strncmp("H12", font, 3) == 0)      /* 12*12 �ַ� */
		{
			BaseAdd = 0x0;
		}
		else if(strncmp("H16", font, 3) == 0)  /* 16*16 �ַ� */
		{
			BaseAdd = 0x2C9D0;
		}
		else if(strncmp("H24", font, 3) == 0)  /* 24*24 �ַ� */
		{
			BaseAdd = 0x68190;
		}
		else if(strncmp("H32", font, 3) == 0)  /* 32*32 �ַ� */
		{
			BaseAdd = 0XEDF00;
		}
		
		/* ���ݺ�������ļ��㹫ʽ������ʼ��ַ */
		code2 = c >> 8;
		code1 = c & 0xFF;
		if (code1 >=0xA1 && code1 <= 0xA9 && code2 >=0xA1)
		{
			oft = ((code1 - 0xA1) * 94 + (code2 - 0xA1)) * BytesPerFont + BaseAdd;
		}
		else if (code1 >=0xB0 && code1 <= 0xF7 && code2 >=0xA1)
		{
			oft = ((code1 - 0xB0) * 94 + (code2 - 0xA1) + 846) * BytesPerFont + BaseAdd;
		}

    }
	W25QXX_Read(GUI_FontDataBuf,oft,BytesPerFont);
	/* ��ȡ�������� */
	//sf_ReadBuffer(GUI_FontDataBuf, oft, BytesPerFont);
} 

/*
*********************************************************************************************************
*	�� �� ��: GUIUSER_DispChar
*	����˵��: ��ʾ�ַ�
*	��    ��: c ��ʾ���ַ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GUIUSER_DispChar(U16P c)  
{ 
    int BytesPerLine; 
    GUI_DRAWMODE DrawMode = GUI_pContext->TextMode; 
    const GUI_FONT_PROP GUI_UNI_PTR *pProp = GUI_pContext->pAFont->p.pProp; 
	
    /* ȷ����ʾ���ַ��Ƿ�����Ч��Χ�� */  
    for (; pProp; pProp = pProp->pNext)                                          
    { 
        if ((c >= pProp->First) && (c <= pProp->Last))break; 
    } 
	
	/* �ַ���Ч��������ʾ */
    if (pProp) 
    { 
        GUI_DRAWMODE OldDrawMode;
        const GUI_CHARINFO GUI_UNI_PTR * pCharInfo = pProp->paCharInfo;
        GUI_GetDataFromMemory(pProp, c);
        BytesPerLine = pCharInfo->BytesPerLine;                
        OldDrawMode  = LCD_SetDrawMode(DrawMode);
        LCD_DrawBitmap(GUI_pContext->DispPosX, GUI_pContext->DispPosY,
                       pCharInfo->XSize, GUI_pContext->pAFont->YSize,
                       GUI_pContext->pAFont->XMag, GUI_pContext->pAFont->YMag,
                       1,    
                       BytesPerLine,
                       &GUI_FontDataBuf[0],
                       &LCD_BKCOLORINDEX
                       );
		
        /* ��䱳�� */
        if (GUI_pContext->pAFont->YDist > GUI_pContext->pAFont->YSize) 
        {
            int YMag = GUI_pContext->pAFont->YMag;
            int YDist = GUI_pContext->pAFont->YDist * YMag;
            int YSize = GUI_pContext->pAFont->YSize * YMag;
            if (DrawMode != LCD_DRAWMODE_TRANS) 
            {
                LCD_COLOR OldColor = GUI_GetColor();
                GUI_SetColor(GUI_GetBkColor());
                LCD_FillRect(GUI_pContext->DispPosX, GUI_pContext->DispPosY + YSize, 
                             GUI_pContext->DispPosX + pCharInfo->XSize, 
                             GUI_pContext->DispPosY + YDist);
                GUI_SetColor(OldColor);
            }
        }
		
        LCD_SetDrawMode(OldDrawMode);
//      if (!GUI_MoveRTL)
        GUI_pContext->DispPosX += pCharInfo->XDist * GUI_pContext->pAFont->XMag;
    }
} 

/*
*********************************************************************************************************
*	�� �� ��: GUIUSER_X_GetCharDistX
*	����˵��: ��ȡ�ַ���X����
*	��    ��: c  �ַ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int GUIUSER_GetCharDistX(U16P c, int * pSizeX) 
{
    const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUI_pContext->pAFont->p.pProp;  
    for (; pProp; pProp = pProp->pNext)                                         
    {
        if ((c >= pProp->First) && (c <= pProp->Last))break;
    }
    return (pProp) ? (pProp->paCharInfo)->XSize * GUI_pContext->pAFont->XMag : 0;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
