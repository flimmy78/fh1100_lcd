#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT


//����ʼ���ȼ�
#define  APP_CFG_TASK_START_PRIO                              2u
#define  APP_CFG_TASK_MsgCAN0_PRIO                             3u
#define  APP_CFG_TASK_PPSTICK_PRIO                            4u
#define  APP_CFG_TASK_COM_PRIO                                5u
#define  APP_CFG_TASK_USER_IF_PRIO                            6u
#define  APP_CFG_TASK_GUI_PRIO                        (OS_CFG_PRIO_MAX - 4u)


#define  APP_CFG_TASK_USER_IF_STK_SIZE                    512u

//�����ջ��С
#define  APP_CFG_TASK_START_STK_SIZE                      512u
#define  APP_CFG_TASK_MsgCAN0_STK_SIZE                     512u
#define  APP_CFG_TASK_COM_STK_SIZE                        512u
#define  APP_CFG_TASK_PPSTICK_STK_SIZE                    512u
#define  APP_CFG_TASK_USER_IF_STK_SIZE                    512u
#define  APP_CFG_TASK_GUI_STK_SIZE                        2048u

//�����ջ�޶�
#define  APP_CFG_TASK_START_STK_SIZE_PCT_FULL             90u
#define  APP_CFG_TASK_START_STK_SIZE_LIMIT       (APP_CFG_TASK_START_STK_SIZE     * (100u - APP_CFG_TASK_START_STK_SIZE_PCT_FULL))    / 100u

#define  APP_CFG_TASK_BLINKY_STK_SIZE_LIMIT      (APP_CFG_TASK_BLINKY_STK_SIZE    * (100u - APP_CFG_TASK_START_STK_SIZE_PCT_FULL))    / 100u


//׷�ٺ͵���ѡ��
#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                0
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO               1
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                2
#endif

#define  APP_CFG_TRACE_LEVEL             TRACE_LEVEL_OFF
#define  APP_CFG_TRACE                   printf

#define  BSP_CFG_TRACE_LEVEL             TRACE_LEVEL_OFF
#define  BSP_CFG_TRACE                   printf

#define  APP_TRACE_INFO(x)               ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_CFG_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_CFG_TRACE x) : (void)0)

#define  BSP_TRACE_INFO(x)               ((BSP_CFG_TRACE_LEVEL  >= TRACE_LEVEL_INFO) ? (void)(BSP_CFG_TRACE x) : (void)0)
#define  BSP_TRACE_DBG(x)                ((BSP_CFG_TRACE_LEVEL  >= TRACE_LEVEL_DBG)  ? (void)(BSP_CFG_TRACE x) : (void)0)

#endif
