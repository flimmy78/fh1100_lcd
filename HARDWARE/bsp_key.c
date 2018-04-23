#include "includes.h"
#include	"bsp_key.h"

#define KEY_COUNT    8	   					/* ��������, 8�������� + 2����ϼ� */
/*
	�����˲�ʱ��KEY_FILTER_TIME*��λ�¼���
*/
#define KEY_FILTER_TIME   3				//����KEY_FILTER_TIME+2��                                                                                                                   
#define KEY_LONG_TIME     150			//�����¼���ֵ(��λ�¼�)
#define KEY_FIFO_SIZE			4				//����FIFO���
#define KEY_RepeatSpeed   1				//��������(KEY_RepeatSpeed+1).=0������



#define RCC_ALL_KEY 	(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC)	/* �����ڶ�Ӧ��RCCʱ�� */
#ifdef 	Hadware_version2						
	//-----------Ӳ���汾V1.2
	#define GPIO_PORT_K1    GPIOC
	#define GPIO_PIN_K1	    GPIO_Pin_1

	#define GPIO_PORT_K2    GPIOC
	#define GPIO_PIN_K2	    GPIO_Pin_2

	#define GPIO_PORT_K3    GPIOC
	#define GPIO_PIN_K3	    GPIO_Pin_3

	#define GPIO_PORT_K4    GPIOA
	#define GPIO_PIN_K4	    GPIO_Pin_0

	#define GPIO_PORT_K5    GPIOA
	#define GPIO_PIN_K5	    GPIO_Pin_1

	#define GPIO_PORT_K6    GPIOA
	#define GPIO_PIN_K6	    GPIO_Pin_2

	#define GPIO_PORT_K7    GPIOA
	#define GPIO_PIN_K7	    GPIO_Pin_3

	#define GPIO_PORT_K8    GPIOA
	#define GPIO_PIN_K8	    GPIO_Pin_4

#else //-------Ӳ���汾V1.1
	#define GPIO_PORT_K1    GPIOA
	#define GPIO_PIN_K1	    GPIO_Pin_0

	#define GPIO_PORT_K2    GPIOA
	#define GPIO_PIN_K2	    GPIO_Pin_1

	#define GPIO_PORT_K3    GPIOA
	#define GPIO_PIN_K3	    GPIO_Pin_2

	#define GPIO_PORT_K4    GPIOA
	#define GPIO_PIN_K4	    GPIO_Pin_3

	#define GPIO_PORT_K5    GPIOA
	#define GPIO_PIN_K5	    GPIO_Pin_4

	#define GPIO_PORT_K6    GPIOA
	#define GPIO_PIN_K6	    GPIO_Pin_5

	#define GPIO_PORT_K7    GPIOA
	#define GPIO_PIN_K7	    GPIO_Pin_6

	#define GPIO_PORT_K8    GPIOA
	#define GPIO_PIN_K8	    GPIO_Pin_7

#endif



/* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
typedef enum
{
	KID_K1 = 0,
	KID_K2,
	KID_K3,
	KID_K4,
	KID_K5,
	KID_K6,
	KID_K7,
	KID_K8
}KEY_ID_E;


/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */
	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatSpeed;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
}KEY_T;

/*
	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�
*/



typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}KEY_FIFO_T;	






static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;		/* ����FIFO����,�ṹ�� */

static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);

static uint8_t IsKeyDown1(void) {if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) == 0) return 1;else return 0;}
static uint8_t IsKeyDown2(void) {if ((GPIO_PORT_K2->IDR & GPIO_PIN_K2) == 0) return 1;else return 0;}
static uint8_t IsKeyDown3(void) {if ((GPIO_PORT_K3->IDR & GPIO_PIN_K3) == 0) return 1;else return 0;}
static uint8_t IsKeyDown4(void) {if ((GPIO_PORT_K4->IDR & GPIO_PIN_K4) == 0) return 1;else return 0;}
static uint8_t IsKeyDown5(void) {if ((GPIO_PORT_K5->IDR & GPIO_PIN_K5) == 0) return 1;else return 0;}
static uint8_t IsKeyDown6(void) {if ((GPIO_PORT_K6->IDR & GPIO_PIN_K6) == 0) return 1;else return 0;}
static uint8_t IsKeyDown7(void) {if ((GPIO_PORT_K7->IDR & GPIO_PIN_K7) == 0) return 1;else return 0;}
static uint8_t IsKeyDown8(void) {if ((GPIO_PORT_K8->IDR & GPIO_PIN_K8) == 0) return 1;else return 0;}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKey
*	����˵��: ��ʼ������. �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();		/* ��ʼ���������� */
	bsp_InitKeyHard();		/* ��ʼ������Ӳ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    ��:  _KeyCode : ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write  >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}



/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    ��:  ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t KEY_Scan(void)
{
	uint8_t ret;
	if (s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		if(ret%3!=2)
		{
			return ret; 	//�������£�
		}
		else
			return KEY_NONE;	//����̸��
	}
	
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey2
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ�������Ķ�ָ�롣
*	��    ��:  ��
*	�� �� ֵ: ��������
*********************************************************************************************************

uint8_t bsp_GetKey2(void)
{
	uint8_t ret;

	if (s_tKey.Read2 == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read2];

		if (++s_tKey.Read2 >= KEY_FIFO_SIZE)
		{
			s_tKey.Read2 = 0;
		}
		return ret;
	}
}
*/

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKeyState
*	����˵��: ��ȡ������״̬
*	��    ��:  _ucKeyID : ����ID����0��ʼ
*	�� �� ֵ: 1 ��ʾ���£� 0 ��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetKeyParam
*	����˵��: ���ð�������
*	��    �Σ�_ucKeyID : ����ID����0��ʼ
*			_LongTime : �����¼�ʱ��               
*			 _RepeatSpeed : �����ٶ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;			/* �����������ٶȣ�0��ʾ��֧������ */
	s_tBtn[_ucKeyID].RepeatCount = 0;						/* ���������� */
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyHard
*	����˵��: ���ð�����Ӧ��GPIO
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��1������GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_ALL_KEY, ENABLE);

	/* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �������������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K1;
	GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K2;
	GPIO_Init(GPIO_PORT_K2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K3;
	GPIO_Init(GPIO_PORT_K3, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K4;
	GPIO_Init(GPIO_PORT_K4, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K5;
	GPIO_Init(GPIO_PORT_K5, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K6;
	GPIO_Init(GPIO_PORT_K6, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K7;
	GPIO_Init(GPIO_PORT_K7, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K8;
	GPIO_Init(GPIO_PORT_K8, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyVar
*	����˵��: ��ʼ����������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;
	/* �԰���FIFO��дָ������ */
	s_tKey.Read  = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	/* ��ÿ�������ṹ���Ա������һ��ȱʡֵ */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
		s_tBtn[i].Count = 0;	                	
		s_tBtn[i].State = 0;							     
		s_tBtn[i].RepeatSpeed = KEY_RepeatSpeed; /* �����������ٶȣ�0��ʾ��֧������ */
		s_tBtn[i].RepeatCount = 0;						  /* ���������� */
	}
	/* �����Ҫ��������ĳ�������Ĳ����������ڴ˵������¸�ֵ */
	s_tBtn[KID_K5].LongTime = 0;
	s_tBtn[KID_K5].RepeatSpeed = 0;
	s_tBtn[KID_K6].LongTime = 0;
	s_tBtn[KID_K6].RepeatSpeed = 0;	
	
	/* �жϰ������µĺ��� */
	s_tBtn[0].IsKeyDownFunc = IsKeyDown1;
	s_tBtn[1].IsKeyDownFunc = IsKeyDown2;
	s_tBtn[2].IsKeyDownFunc = IsKeyDown3;
	s_tBtn[3].IsKeyDownFunc = IsKeyDown4;
	s_tBtn[4].IsKeyDownFunc = IsKeyDown5;
	s_tBtn[5].IsKeyDownFunc = IsKeyDown6;
	s_tBtn[6].IsKeyDownFunc = IsKeyDown7;
	s_tBtn[7].IsKeyDownFunc = IsKeyDown8;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectKey
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    ��:  �����ṹ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn;
	pBtn = &s_tBtn[i];
	if (pBtn->IsKeyDownFunc())	//��������
	{
		if (pBtn->Count < KEY_FILTER_TIME)	//С��5�а��£�ֵ=5
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)	//5<x<10�а��£�x++
		{
			pBtn->Count++;
		}
		else							//=10���жϰ���
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1;//��ǰ״̬=����
				bsp_PutKey((uint8_t)(3 * i + 1));//FIFO<<��ť����>>
			}
			if (pBtn->LongTime > 0)				 //֧�ֳ���
			{
				if (pBtn->LongCount < pBtn->LongTime)       // δ�ﵽ����ʱ����ֵ
				{
				   pBtn->LongCount++;
				   /*
				   //ע�͵���ӦΪ������ֻʹ��<<����>>��
					if (++pBtn->LongCount == pBtn->LongTime)//������++
					{
					//bsp_PutKey((uint8_t)(3 * i + 3));     //FIFO<<��ť����>>
					}
				   */
				}
				else//����ʱ����ֵ
				{
					if (pBtn->RepeatSpeed > 0)//��������
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							bsp_PutKey((uint8_t)(3 * i + 1));//FIFO<<��ť����>>
						}
					}
				}
			}
		}
	}
	else	//û�а���
	{
		/*ԭ�㷨2017.06.08
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;//�����--��������ֱ��=0��
		}
		*/
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count --;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count=0;
		}
		//�������㷨2017.06.08
		else//pBtn->CountΪ0
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;
				//bsp_PutKey((uint8_t)(3 * i + 2));//ע�͵���ӦΪ������ֻʹ��<<����>>��
			}
		}
		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyScan
*	����˵��: ɨ�����а���������������systick�ж������Եĵ���
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_KeyScan(void)
{
	uint8_t i;

	for (i = 0; i < KEY_COUNT; i++)
	{
		bsp_DetectKey(i);
	}
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
