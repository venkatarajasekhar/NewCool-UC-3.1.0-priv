#ifndef __UI_FACTORY_H__
#define __UI_FACTORY_H__

//#define FACTORY_TEST_WITH_ALONE_SMARTCARD
#define FACTORY_TEST_WITH_DIFF_SMARTCARD
//#define FACTORY_TEST_WITH_SEARCH
//#define FACTORY_TEST_WITH_CONNECT_WIFI
//#define FACTORY_TEST_DEBUG
//#define FACTORY_TEST_FRONTSPANEL_ALL

#define  UI_FACTORY_CONT_X		(COMM_BG_X)
#define  UI_FACTORY_CONT_Y		(COMM_BG_Y)
#define  UI_FACTORY_CONT_W		(COMM_BG_W)
#define  UI_FACTORY_CONT_H		(COMM_BG_H)

#define UI_FACTORY_LIST_FIELD			(3)
#define UI_FACTORY_MODULE_INTERGER		(40)

//info
#define  UI_FACTORY_INFO_CONT_X		(120)
#define  UI_FACTORY_INFO_CONT_Y		(80)
#define  UI_FACTORY_INFO_CONT_W		(450)
#define  UI_FACTORY_INFO_CONT_H		(UI_FACTORY_MODULE_INTERGER*UI_FACTORY_INFO_LIST_COUNT)


#define  UI_FACTORY_INFO_LIST_X		(0)
#define  UI_FACTORY_INFO_LIST_Y		(0)
#define  UI_FACTORY_INFO_LIST_W		(UI_FACTORY_INFO_CONT_W)
#define  UI_FACTORY_INFO_LIST_H		(UI_FACTORY_INFO_CONT_H)
#if defined FACTORY_TEST_WITH_ALONE_SMARTCARD || defined FACTORY_TEST_WITH_DIFF_SMARTCARD
#define  UI_FACTORY_INFO_LIST_COUNT		(7)
#else
#define  UI_FACTORY_INFO_LIST_COUNT		(5)
#endif

//search
#define  UI_FACTORY_SEARCH_CONT_X		(UI_FACTORY_INFO_CONT_X)
#define  UI_FACTORY_SEARCH_CONT_Y		(UI_FACTORY_INFO_CONT_Y + UI_FACTORY_INFO_CONT_H)
#define  UI_FACTORY_SEARCH_CONT_W		(UI_FACTORY_INFO_CONT_W)
#define  UI_FACTORY_SEARCH_CONT_H		(UI_FACTORY_MODULE_INTERGER*UI_FACTORY_SEARCH_LIST_COUNT)


#define  UI_FACTORY_SEARCH_LIST_X			(0)
#define  UI_FACTORY_SEARCH_LIST_Y			(0)
#define  UI_FACTORY_SEARCH_LIST_W			(UI_FACTORY_SEARCH_CONT_W)
#define  UI_FACTORY_SEARCH_LIST_H			(UI_FACTORY_SEARCH_CONT_H)
#ifdef FACTORY_TEST_WITH_SEARCH
#define  UI_FACTORY_SEARCH_LIST_COUNT		(4)
#else
#define  UI_FACTORY_SEARCH_LIST_COUNT		(2)
#endif

//frontpanel
#define  UI_FACTORY_FRONTPANEL_CONT_X		(UI_FACTORY_INFO_CONT_X)
#define  UI_FACTORY_FRONTPANEL_CONT_Y		(UI_FACTORY_SEARCH_CONT_Y + UI_FACTORY_SEARCH_CONT_H)
#define  UI_FACTORY_FRONTPANEL_CONT_W		(UI_FACTORY_INFO_CONT_W)
#define  UI_FACTORY_FRONTPANEL_CONT_H		(UI_FACTORY_MODULE_INTERGER*3)

#define  UI_FACTORY_FRONTPANEL_TITLE_X		(10)
#define  UI_FACTORY_FRONTPANEL_TITLE_Y		(0)
#define  UI_FACTORY_FRONTPANEL_TITLE_W		(UI_FACTORY_FRONTPANEL_CONT_W - UI_FACTORY_FRONTPANEL_TITLE_X)
#define  UI_FACTORY_FRONTPANEL_TITLE_H		(UI_FACTORY_MODULE_INTERGER)

#define  UI_FACTORY_FRONTPANEL_MBOX_X		(0)
#define  UI_FACTORY_FRONTPANEL_MBOX_Y		(UI_FACTORY_FRONTPANEL_TITLE_Y + UI_FACTORY_FRONTPANEL_TITLE_H)
#define  UI_FACTORY_FRONTPANEL_MBOX_W		(UI_FACTORY_FRONTPANEL_CONT_W)
#define  UI_FACTORY_FRONTPANEL_MBOX_H		(UI_FACTORY_FRONTPANEL_CONT_H - UI_FACTORY_FRONTPANEL_MBOX_Y)

#ifdef FACTORY_TEST_FRONTSPANEL_ALL
#define  UI_FACTORY_FRONTPANEL_MBOX_COL		(7)
#else
#define  UI_FACTORY_FRONTPANEL_MBOX_COL		(6)
#endif
#define  UI_FACTORY_FRONTPANEL_MBOX_ROW		(2)
#define  UI_FACTORY_FRONTPANEL_MBOX_TOTAL	(UI_FACTORY_FRONTPANEL_MBOX_COL*UI_FACTORY_FRONTPANEL_MBOX_ROW)


//net
#define  UI_FACTORY_NET_CONT_X		(UI_FACTORY_INFO_CONT_X + UI_FACTORY_INFO_CONT_W + UI_FACTORY_MODULE_INTERGER)
#define  UI_FACTORY_NET_CONT_Y		(UI_FACTORY_INFO_CONT_Y)
#define  UI_FACTORY_NET_CONT_W		(UI_FACTORY_INFO_CONT_W)
#define  UI_FACTORY_NET_CONT_H		(UI_FACTORY_MODULE_INTERGER*UI_FACTORY_NET_LIST_COUNT)


#define  UI_FACTORY_NET_LIST_X			(0)
#define  UI_FACTORY_NET_LIST_Y			(0)
#define  UI_FACTORY_NET_LIST_W			(UI_FACTORY_NET_CONT_W)
#define  UI_FACTORY_NET_LIST_H			(UI_FACTORY_NET_CONT_H)
#define  UI_FACTORY_NET_LIST_COUNT		(7)


//button
#define  UI_FACTORY_BUTTON_CONT_X		(UI_FACTORY_NET_CONT_X)
#define  UI_FACTORY_BUTTON_CONT_Y		(UI_FACTORY_NET_CONT_Y + UI_FACTORY_NET_CONT_H + 2*UI_FACTORY_MODULE_INTERGER)
#define  UI_FACTORY_BUTTON_CONT_W		(UI_FACTORY_NET_CONT_W)
#define  UI_FACTORY_BUTTON_CONT_H		(UI_FACTORY_MODULE_INTERGER)


#define  UI_FACTORY_BUTTON_TEXT_X			(0)
#define  UI_FACTORY_BUTTON_TEXT_Y			(0)
#define  UI_FACTORY_BUTTON_TEXT_W			(UI_FACTORY_BUTTON_CONT_W)
#define  UI_FACTORY_BUTTON_TEXT_H			(UI_FACTORY_BUTTON_CONT_H)

RET_CODE open_factory_test(u32 para1, u32 para2);

#endif

