#ifndef __GPSTIME_H
#define __GPSTIME_H 

#include <sys.h>	  

extern struct __DEV 			DevHead;
extern struct _RUNSYS     DevSys;
#define ADMIN_PASS   1100

typedef enum								//״̬�����쳣ö��
{ 
	STA_OK  		= 0x00, 	//״̬����	
	STA_ERR 		= 0x01		//״̬�쳣	
}DEV_STA;



typedef struct __DEV
{
	DEV_STA	 tb;				//ͬ����־
	DEV_STA	 synflag;			//��ʼ����־
	DEV_STA  bds;				//BDS��־
	DEV_STA  gps;				//GPS��־
	DEV_STA  b1;				//B1��־
	DEV_STA  b2;				//B2��־
	DEV_STA  monitor1;			//monitor1��־	
	DEV_STA  monitor2;			//monitor2��־
	DEV_STA  ptp1;				//PTP1��־	
	DEV_STA  ptp2;				//PTP1��־	
	
	uint8_t  devfalt;			//װ�ù���
	uint8_t  devalar;			//װ�ø澯
	//uint8_t	 cpufalt;		//CPU���İ��쳣����ɾ�������⿽����ֵʱ��ֵ��ȷ��
	
	uint8_t  is_master;	 		//������
	uint8_t  selsouce;			//��ǰ��׼Դ	
	uint8_t  bds_sta_synch;		//����Դͬ��״̬
	uint8_t  bds_num_capture;	//����Դ���ǲ�������  
	uint8_t  gps_sta_synch;		//GPSԴͬ��״̬
	uint8_t  gps_num_capture;	//GPSԴ���ǲ�������
	uint8_t  b1_sta_synch;		//IRIG-B1Դͬ��״̬
	uint8_t  b1_qual;			//IRIG-B1Դ����λ
	uint8_t  b2_sta_synch;		//IRIG-B2Դͬ��״̬
	uint8_t  b2_qual;			//IRIG-B2Դ����λ
	uint8_t  ptp1_sta_synch;	//IRIG-B1Դͬ��״̬
	uint8_t  ptp2_sta_synch;	//IRIG-B1Դͬ��״̬
	uint8_t  self_qual;			//����ʱ����λ
	uint16_t year;				//����ʱ��->��
	uint8_t	 month;				//����ʱ��->��
	uint8_t	 day;				//����ʱ��->��
	uint8_t	 h; 				//����ʱ��->ʱ
	uint8_t	 m;					//����ʱ��->��
	uint8_t	 s;					//����ʱ��->��
}__DEV_HEAD;


typedef struct _RUNSYS
{
	uint8_t   PPS_Ready_Out;		//PPS��ʼ����ı�־�����Լ��ж�
	uint8_t	  PPS_Tick;					//�����������ر�־(����RTCʱ��ÿ�뻽��)
	uint8_t	  PPS_Tick_300;	
	uint16_t  quality;					//���ܱ���ʱ������
	DEV_STA   idle_out_flag;		//���ܱ���
	uint16_t  idle_out;					//����û�в�����ʱ��(���ܱ������)
	uint16_t  wrongpass_cnt;		//�������ļ���
	uint16_t  wrongpass_waiting;//��������ĵȴ�ʱ��
	uint16_t  can0_err;         //CAN0�������,����һ��ֵ������ϵͳ
	uint8_t   can1_err;         //CAN1����״̬,������ʾCAN1����
	uint8_t   first_recv_Can0;  //��ɳ�ʼ������Open
	uint8_t   USER_Right;       //�û���½���Ȩ��
	uint32_t  USER_PASS;				//�û���½����
	uint8_t   Login_cnt;        //�û���½������־��һ�����������Դ�1��
	uint8_t   LCD_Protect_ON;		//��Ļ��������
	uint8_t   CAN1_Rec1st;		  //���ò����յ��ظ�
	uint8_t   CAN1_Reqcnt;		  //���ò����������
	
}__RUNSYS;


void Dev_Period_Handler(void);
void DEV_PPS_RECV(void);

uint8_t CAN_Head_Handler(void);
void DEV_Time_Handler(uint8_t tick);

void Clear_CAN1(void);

#endif



















