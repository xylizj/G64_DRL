#ifndef __CAN_H__
#define __CAN_H__


#include "derivative.h" /* derivative-specific definitions */
#include "typedefs.h"


#define CAN_MSG_EMPTY           0x00
#define CAN_MSG_NEW_MASK        0x01
#define CAN_MSG_OVER_MASK       0x02
#define CAN_MSG_NEW_OVER        0x03
#define CAN_MSG_TIMEOUT         0x04
#define CAN_MSG_UNTIMEOUT_MASK  0xFB

#define CAN_MSG_CHECK(msg_index)          (can_msgs_buf_array[msg_index].msg_st & CAN_MSG_NEW_OVER)
#define CAN_MSG_CELAR(msg_index)          (can_msgs_buf_array[msg_index].msg_st = 0)
#define CAN_MSG_EMPTY_CHECK(msg_index)    (can_msgs_buf_array[msg_index].msg_st & CAN_MSG_UNTIMEOUT_MASK) == CAN_MSG_EMPTY /*REQ IDs: TPMS_SW_BASE_0377*/
#define CAN_MSG_TIMEOUT_SET(msg_index)    (can_msgs_buf_array[msg_index].msg_st |= CAN_MSG_TIMEOUT)
#define CAN_MSG_TIMEOUT_CLEAR(msg_index)  (can_msgs_buf_array[msg_index].msg_st &= CAN_MSG_UNTIMEOUT_MASK)
#define CAN_MSG_TIMEOUT_CHECK(msg_index)  (can_msgs_buf_array[msg_index].msg_st & CAN_MSG_TIMEOUT)

/*Hardware interface control*/
#define CAN_REG(_reg) CAN##_reg /*For CAN Module extend*/

/*Disable MSCAN*/
#define CAN_MODULE_STOP CAN_REG(CTL1_CANE) = 0

typedef enum __CAN_MSG_T__
{
    MSG_DIAG_RX_PHY,
    MSG_DIAG_RX_FUN,
    MSG_DIAG_TX,
    RCU_P_MSG_SEND,
    CAN_MSGS_COUNT,
}can_msg_t;


typedef struct __CAN_MSG_CONFIG_T__
{
    uint32_t id;
    bool     is_ext_id;
    bool     is_tx;
    void (*process_fun)(void);
}can_msg_config_t;

typedef struct __CAN_MSG_BUF_CFG_T__
{
    uint8_t msg_st;
    uint8_t dlc;
    uint8_t *pbuffer;
    uint8_t period; /*某一个报文周期等于其配置的period值乘以can_period_handle()被调用的周期*/
                    //period如果为0，则不是周期性报文
    uint8_t tick;   
}can_msg_buf_cfg_t;

extern uint8_t ECU_P_Msg_send_buf[8];
extern uint32_t real_id;


extern can_msg_buf_cfg_t can_msgs_buf_array[];
extern io_err_t can_init(void);
extern io_err_t can_tx_msg(can_msg_t msg_index);
extern void can_tx_check(void);
extern void can_rx_msg(void);
extern void can_period_handle(BOOL tx_rx);






















#endif /*End of __CAN_H__*/

