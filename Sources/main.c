#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "hardeware.h"   //底层硬件驱动系统总头文件
#include "can.h"
#include "diag.h"
#include "dcm_cbk.h"

#define TIME_ACTION_AFTER_POSRESP 500//ms
#define TIME_ECU_PRESENT          1000

extern uint32 SysTime;

void Task_ActionAfterPosResp(void);
void Task_EcuPresent(void);

/*
*********************************************************************************************************
*                                          main函数
*********************************************************************************************************
*/
void main(void) 
{
  DisableInterrupts; 
  DEV_Init();
  Dem_Init();
  can_init();
  Diag_InitTP();
  EnableInterrupts;
  
  while(1)
  {
     can_rx_msg();
     Diag_State();
     //Dem_MainFunction();//can_period_handle(FALSE);//RX
     //Task_ActionAfterPosResp();
     Task_EcuPresent();
  }
}



void Task_ActionAfterPosResp(void)
{
  static uint32 TimeCnt_ActionAfterPosResp;
  
  
  if(SysTime >= (TIME_ACTION_AFTER_POSRESP+TimeCnt_ActionAfterPosResp) ){
    TimeCnt_ActionAfterPosResp = SysTime;

    if(EcuHardResetConditionOk){
      //Do EcuHardRest Action
      EcuHardResetConditionOk = 0;
      Appl_EcuHardReset();
    }
    if(EcuSoftResetConditionOk){
      //Do EcuHardRest Action
      EcuSoftResetConditionOk = 0;
      Appl_EcuSotfReset();
    }
  } 
}

extern uint8_t bs_sn;

void Task_EcuPresent(void)
{
  static uint32 TimeCnt_EcuPresent;  
  
  if(SysTime >= (TIME_ECU_PRESENT+TimeCnt_EcuPresent) ){
    TimeCnt_EcuPresent = SysTime;
    can_tx_msg(RCU_P_MSG_SEND);
  } 
}




 
