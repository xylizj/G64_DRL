/*
*********************************************************************************************************
* File       : hardware.c
*Description : s12g64的硬件设置，包括时钟和RTI实时中断的配置
*            : 适用芯片mc9s12g64
* Note(s)    : 最后修改日期 2014.12.1
*            : 适用芯片mc9s12g64
*********************************************************************************************************
*/

#include"hardeware.h"





#define OSC_FRQ_MHZ 8 /*[MHz]*/ //DRL 东风电动的板子晶振是8M
#define BUS_FRQ_MHZ 16 /*[MHz]*/ 
void CLOCK_Init(void)
{     
    /* CPMUPROT: PROT=0 */
    CPMUPROT = 0x26U;                    /* Disable protection of clock configuration registers */
    /* CPMUCLKS: PSTP=0 */
    CPMUCLKS &= (unsigned char)~(unsigned char)0x40U;                     
    /* CPMUCLKS: PLLSEL=1 */
    CPMUCLKS |= (unsigned char)0x80U;    /* Enable the PLL to allow write to divider registers */
    /* CPMUCLKS: RTIOSCSEL=1 */
    CPMUCLKS |= (unsigned char)0x02U;
    /* Set Freq = 1 MHz */
    CPMUREFDIV_REFDIV = OSC_FRQ_MHZ - 1; /* Set the divider register(Write only when PROT is 0,and PLLSEL is 1) */
    CPMUREFDIV_REFFRQ = 0;    
    /* Set Fvco is 2 * BUS_FRQ_MHZ */
    CPMUSYNR_SYNDIV = BUS_FRQ_MHZ - 1;   /* Set the multiplier register */
    CPMUSYNR_VCOFRQ = 0; /*0 For 32 MHz ~ 48 MHz*/
    
    /* Fpll = Fvco / 2 when PLL locked */
    CPMUPOSTDIV = 0x00U;
    
    CPMUOSC = 0x80U;                     /* Enable the oscillator */
    /* CPMUPLL: FM1=0,FM0=0 */
    CPMUPLL = 0x00U;                     /* Set the PLL frequency modulation(0 for FM off)*/
    while(0u == CPMUFLG_LOCK && 0u == CPMUFLG_UPOSC)
    {
        /* Wait until the oscillator is qualified by the PLL */
    }
    /* CPMUPROT: PROT=0 */
    CPMUPROT = 0x00U;                    /* Enable protection of clock configuration registers */
    /* CPMUCOP: RSBCK=0,WRTMASK=0 */
    CPMUINT &= (unsigned char)~(unsigned char)0x12U;
    /* CPMULVCTL: LVIE=0 */
    CPMULVCTL &= (unsigned char)~(unsigned char)0x02U;
    /* IRQCR: IRQEN=0 */
    IRQCR &= (unsigned char)~(unsigned char)0x40U;
}
/*
*********************************************************************************************************
*                                           芯片初始化
* Description: DEV_Init；用RTI实时中断设置时间节拍
*
* Arguments  : none
*
* Note(s)    : 
*********************************************************************************************************
*/
//RTI Ref clk 1000000HZ
void DEV_Init(void)
{                        
     CLOCK_Init();
     //CPMURTI = 0xD0;        //0b11010000,1000000/(50*1000)=20Hz,设定50ms的定时中断
     //CPMURTI = 0xB0;        //0b10110000,1000000/(10*1000)=100Hz,设定10ms的定时中断
     CPMURTI = 0x80;        //0b10000000,1000000/(1*1000)=1000Hz,设定1ms的定时中断
     CPMUINT_RTIE = 1;      //打开RTI的中断
}




uint32 SysTime;

#pragma CODE_SEG __NEAR_SEG NON_BANKED

void interrupt //VectorNumber_Vrti
OSTickISR(void)
{
    //asm cli;
    CPMUFLG_RTIF = 1;
    SysTime+=1;
}
#pragma CODE_SEG DEFAULT

















