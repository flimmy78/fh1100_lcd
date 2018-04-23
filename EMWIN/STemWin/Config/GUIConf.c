#include "GUI.h"
#include "malloc.h"
#include "sys.h"

#define USE_EXRAM  0//ʹ���ⲿRAM
//����EMWIN�ڴ��С
#define GUI_NUMBYTES  MEM1_MAX_SIZE
#define GUI_BLOCKSIZE 0X80  //���С


//GUI_X_Config
//��ʼ����ʱ�����,��������emwin��ʹ�õ��ڴ�
void GUI_X_Config(void) 
{

	if(USE_EXRAM) //ʹ���ⲿRAM
	{
//		U32 *aMemory = mymalloc(SRAMEX,GUI_NUMBYTES); //���ⲿSRAM�з���GUI_NUMBYTES�ֽڵ��ڴ�
//		GUI_ALLOC_AssignMemory((void*)aMemory, GUI_NUMBYTES); //Ϊ�洢����ϵͳ����һ���洢��
//		GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE); //���ô洢���ƽ���ߴ�,����Խ��,���õĴ洢������Խ��
//		GUI_SetDefaultFont(GUI_FONT_6X8); //����Ĭ������
	}else  //ʹ���ڲ�RAM
	{
		
		U32 *aMemory = mymalloc(SRAMIN,GUI_NUMBYTES); //���ڲ�RAM�з���GUI_NUMBYTES�ֽڵ��ڴ�
		GUI_ALLOC_AssignMemory((U32 *)aMemory, GUI_NUMBYTES); //Ϊ�洢����ϵͳ����һ���洢��
		GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE); //���ô洢���ƽ���ߴ�,����Խ��,���õĴ洢������Խ��
		GUI_SetDefaultFont(GUI_FONT_6X8); //����Ĭ������
	}
}
