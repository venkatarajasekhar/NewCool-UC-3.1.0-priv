/****************************************************************************
* Notice:      Copyright (c) Montage Techonoloy Corporation (2009).
*              All rights reserved.
* Owner:       Kevin Li <kai.li@montage-tech.com>
*              Mark Wu <mark.wu@montage-tech.com>
****************************************************************************/
#ifndef __UI_CONDITIONAL_ACCEPT_H__
#define __UI_CONDITIONAL_ACCEPT_H__
#define CONDITIONAL_ACCEPT_ITEM_X         COMM_ITEM_OX_IN_ROOT
#define CONDITIONAL_ACCEPT_ITEM_Y         COMM_ITEM_OY_IN_ROOT
#define CONDITIONAL_ACCEPT_ITEM_W         COMM_ITEM_MAX_WIDTH
#define CONDITIONAL_ACCEPT_ITEM_H         COMM_ITEM_H
#define CONDITIONAL_ACCEPT_ITEM_V_GAP          16


#define CONDITIONAL_ACCEPT_ITEM_CNT 3
typedef struct
{
  cas_card_info_t *p_card_info ;
  burses_info_t *p_ipp_buy_info;
}card_and_ipp_info;


RET_CODE open_conditional_accept(u32 para1, u32 para2);

#endif


