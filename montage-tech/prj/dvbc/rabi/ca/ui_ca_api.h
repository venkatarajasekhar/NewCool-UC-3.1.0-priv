/****************************************************************************

 ****************************************************************************/
#ifndef __UI_CA_API_H__
#define __UI_CA_API_H__

enum ca_msg
{
  MSG_CA_CARD_IN = MSG_EXTERN_BEGIN + 500,
  MSG_CA_CARD_OUT,
  MSG_CA_INIT_OK,
  MSG_CA_INIT_FAILED,
  MSG_CA_OPE_INFO,
  MSG_CA_ENT_INFO,
  MSG_CA_IPV_INFO,
  MSG_CA_BUR_INFO,
  MSG_CA_CARD_INFO,
  MSG_CA_PAIRE_INFO,
  MSG_CA_PAIRE_SET,
  MSG_CA_MON_CHILD_INFO,
  MSG_CA_MAIL_HEADER_INFO,
  MSG_CA_MAIL_BODY_INFO,
  MSG_CA_ANNOUNCE_HEADER_INFO,
  MSG_CA_ANNOUNCE_BODY_INFO,
  MSG_CA_PIN_SET_INFO,
  MSG_CA_CLEAR_DISPLAY,
  MSG_CA_EVT_NOTIFY,
  MSG_CA_EVT_LEVEL,
  MSG_CA_RATING_INFO,
  MSG_CA_RATING_SET,
  MSG_CA_IPP_REC_INFO,
  MSG_CA_IPP_BUY_OK,
  MSG_CA_PIN_VERIFY_INFO,
  MSG_CA_MON_CHILD_FEED,
  MSG_CA_FINGER_INFO,
  MSG_CA_HIDE_FINGER,
  MSG_CA_IPP_BUY_INFO,
  MSG_CA_IPP_HIDE_BUY_INFO,
  MSG_CA_FORCE_CHANNEL_INFO,
  MSG_CA_UNFORCE_CHANNEL_INFO,
  MSG_CA_MAIL_DEL_RESULT,
  MSG_CA_ANNOUNCE_DEL_RESULT,
  MSG_CA_SHOW_OSD,
  MSG_CA_HIDE_OSD,
  MSG_CA_HIDE_FORCE_OSD,
  MSG_CA_HIDE_OSD_UP,
  MSG_CA_HIDE_OSD_DOWN, 
  MSG_CA_UNLOCK_PARENTAL,
  MSG_CA_NOTIFY_EXPIRY_STATE,
  MSG_CA_WORK_TIME_INFO,
  MSG_CA_WORK_TIME_SET,
  MSG_CA_MAIL_CHANGE_STATUS,
  MSG_TIMER_CARD_OVERDUE,
  MSG_CA_CARD_UPDATE_PROGRESS,
  MSG_CA_ACLIST_INFO,
  MSG_CA_PLATFORM_ID,
  MSG_CA_READ_FEED_DATA,
  MSG_CA_SHOW_SENIOR_PREVIEW,
  MSG_CA_HIDE_SENIOR_PREVIEW,
  MSG_CA_STOP_CUR_PG,
  MSG_CA_CARD_NEED_UPDATE,
  MSG_CA_CARD_UPDATE_BEGIN,
  MSG_CA_CARD_UPDATE_ERR,
  MSG_CA_CARD_UPDATE_END,
  MSG_PIC_FLASH,
  MSG_SOFTWARE_UPDATE,
  MSG_CA_ECM_FINGER_INFO,
  MSG_CA_AUTHEN_EXPIRE_DAY,
  MSG_CA_CONDITION_SEARCH,
  MSG_CA_CONTINUE_WATCH_CHECK, 
  MSG_CA_OPEN_IPPV_PPT,
  MSG_CA_HIDE_IPPV_PPT,
  MSG_CA_IPP_PROG_NOTIFY,
  MSG_CA_IPPV_BUY_INFO,
  MSG_CA_IPPV_INFO,
  MSG_CA_IPP_BUY_NOTIFY,
  MSG_CA_BURSE_CHARGE_INFO,
  MSG_CA_ERROR_PIN_CODE,
  MSG_CA_SHOW_BURSE_CHARGE,
  MSG_CA_SHOW_ERROR_PIN_CODE,
  MSG_CA_CANCEL_PPV_ICON,
  MSG_CA_CANCEL_PRE_AUTH,
  MSG_CA_REQUIRE_CARD_INFO,
  MSG_CA_CONTINUE_WATCH_LIMIT,
  MSG_CA_STB_NOTIFICATION,
  MSG_CA_JUDGE_MOTHER_CARD,
  MSG_CA_SEND_CARD_STATUS_TO_FEED,
  MSG_CA_DEBUG,
  MSG_HOTKEY_GREEN,
  MSG_CA_IPP_EXIT,
  MSG_CA_CLOSE_FP,
  MSG_CA_SHOW_FP,
  MSG_OSD_ENABLE_UIO_KEY,  
  MSG_CA_REQUEST_CARD_INFO,
  MSG_CA_FINGER_BEAT,
  MSG_CA_FINGER_BEAT2,
  MSG_CA_FINGER_SPARKING_BEAT,
  MSG_CA_OTA_UPDATE_ABV,
  MSG_CA_IPP_RESULT_REFRESH,
  MSG_CA_FORCE_MSG,
  MSG_CA_HIDE_FORCE_MSG,
  MSG_CA_PVR_FINGER_PRINT,
  MSG_CA_PVR_ECM_FINGER_PRINT,
  MSG_CA_PVR_HIDE_ECM_FINGER_PRINT,
  MSG_CA_PVR_OSD,
  MSG_CA_UPGRADE_FREQ,
  MSG_CA_VOD_AUTH,
  MSG_ADS_MESSAGE,
  MSG_CA_FACTORY_UPDATE,
};


void ui_init_ca(void);

void ui_release_ca(void);

void ui_ca_set_sid(cas_sid_t *p_ca_sid);

void ui_ca_do_cmd(u32 cmd_id, u32 para1, u32 para2);

char * ui_ca_get_cardtype(void);

void ui_ca_set_language(language_set_t lang_set);

int snprintf(char *string, size_t count, const char *format, ...);

#endif

