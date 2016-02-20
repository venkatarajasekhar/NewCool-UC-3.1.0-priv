/****************************************************************************

****************************************************************************/

#include "ui_common.h"

enum comm_dlg_idc
{
  IDC_BTN_YES = 1,
  IDC_BTN_NO,
  IDC_CONTENT
};

static dlg_ret_t g_dlg_ret;

u16 comm_dlg_keymap(u16 key);

RET_CODE comm_dlg_proc(control_t *p_ctrl, u16 msg, u32 para1, u32 para2);

#define DO_DLG_FUNC(func) \
  if (func != NULL) (*func)(); 


static void ui_comm_dlg_set_priovity(control_t * p_ctrl, u32 context)
{
  OS_PRINTF("####[%s], set priovity = [%d]####\n", __FUNCTION__,context);
  ctrl_set_context(p_ctrl, context);
}


static u32 ui_comm_dlg_get_last_priovity(control_t * p_ctrl)
{
  return ctrl_get_context(p_ctrl);
}


dlg_ret_t ui_comm_dlg_open(comm_dlg_data_t *p_data, dlg_priority_t level)
{
  control_t *p_root = NULL;
  control_t *p_cont;
  control_t *p_btn, *p_txt;
  u16 i, cnt, x, y;
  u8 type, mode;
  u16 btn[2] = { IDS_YES, IDS_NO };
  dlg_ret_t  ret;
  dlg_ret_t  last_ret = DLG_RET_NULL;
  dlg_priority_t last_level;
  
  p_root = fw_find_root_by_id(ROOT_ID_POPUP);
  if(p_root != NULL) // already opened
  {
    last_level = ui_comm_dlg_get_last_priovity(p_root);
    OS_PRINTF("####[%s], line[%d], get last_priovity = [%d],new dlg level = [%d]####\n", __FUNCTION__, __LINE__, last_level, level);
    if(last_level < level)
    {
      ret = DLG_RET_NULL;
      return ret;
    }
    UI_PRINTF("UI: already open a dialog, force close it! \n");
    ui_comm_dlg_close();
    last_ret = g_dlg_ret;
  }

  p_cont = p_btn = p_txt = NULL;
  x = y = cnt = 0;

  type = p_data->style & 0x0F;
  mode = p_data->style & 0xF0;

  // create root at first
  p_cont = ctrl_create_ctrl(CTRL_CONT, ROOT_ID_POPUP,
                            p_data->x, p_data->y, p_data->w, p_data->h,
                            NULL, 0);
  ctrl_set_attr(p_cont, (u8)(type == \
                             DLG_FOR_SHOW ? OBJ_ATTR_INACTIVE : OBJ_ATTR_ACTIVE));
  ctrl_set_rstyle(p_cont,
                  RSI_BOX2,
                  RSI_BOX2,
                  RSI_BOX2);

  ctrl_set_keymap(p_cont, comm_dlg_keymap);
  ctrl_set_proc(p_cont, comm_dlg_proc);
  ui_comm_dlg_set_priovity(p_cont, level);
  
  p_txt = ctrl_create_ctrl(CTRL_TEXT, IDC_CONTENT,
                           DLG_CONTENT_GAP, DLG_CONTENT_GAP,
                           (u16)(p_data->w - 2 * DLG_CONTENT_GAP),
                           (u16)(p_data->h - 2 * DLG_CONTENT_GAP - DLG_BTN_H),
                           p_cont, 0);

  text_set_font_style(p_txt, FSI_DLG_CONTENT, FSI_DLG_CONTENT, FSI_DLG_CONTENT);
  if (mode == DLG_STR_MODE_STATIC)
  {
    text_set_content_type(p_txt, TEXT_STRTYPE_STRID);
    text_set_content_by_strid(p_txt, (u16)p_data->content);
  }
  else
  {
    text_set_content_type(p_txt, TEXT_STRTYPE_UNICODE);
    text_set_content_by_unistr(p_txt, (u16*)p_data->content);
  }

  switch (type)
  {
    case DLG_FOR_ASK:
      x = p_data->w / 3 - DLG_BTN_W / 2;
      y = p_data->h - DLG_BTN_H - DLG_CONTENT_GAP;
      cnt = 2;
      break;
    case DLG_FOR_CONFIRM:
      x = (p_data->w - DLG_BTN_W) / 2;
      y = p_data->h - DLG_BTN_H - DLG_CONTENT_GAP;
      cnt = 1;
      break;
    case DLG_FOR_SHOW:
      cnt = 0;
      break;
    default:
      MT_ASSERT(0);
  }

  for (i = 0; i < cnt; i++)
  {
    p_btn = ctrl_create_ctrl(CTRL_TEXT, (u8)(IDC_BTN_YES + i),
                             x, y, DLG_BTN_W, DLG_BTN_H, p_cont, 0);
    ctrl_set_rstyle(p_btn,
                    RSI_PBACK, RSI_OTT_IM_BUTTON_SL, RSI_PBACK);
    text_set_font_style(p_btn,
                        FSI_DLG_BTN_SH, FSI_DLG_BTN_HL, FSI_DLG_BTN_SH);
    text_set_content_type(p_btn, TEXT_STRTYPE_STRID);

    if(p_data->specify_bnt == TRUE)
    {
      text_set_content_by_strid(p_btn, p_data->specify_bnt_cont[i]);
    }
    else
    {
      text_set_content_by_strid(p_btn,
                                (u16)(cnt == 1 ? IDS_OK : btn[i]));
    }

    ctrl_set_related_id(p_btn,
                        (u8)((i - 1 + cnt) % cnt + 1), /* left */
                        0,                             /* up */
                        (u8)((i + 1) % cnt + 1),       /* right */
                        0);                            /* down */

    x += p_data->w / 3;
  }

#ifdef SPT_DUMP_DATA
  spt_dump_menu_data(p_cont);
#endif

  // attach the root onto mainwin
  OS_PRINTF("p_cont[0x%x], root[%d]\n", p_cont, p_data->parent_root);
  fw_attach_root(p_cont, p_data->parent_root);

  // paint
  if (cnt > 0)
  {
    ctrl_default_proc(p_btn, MSG_GETFOCUS, 0, 0);
  }
  ctrl_paint_ctrl(p_cont, FALSE);

  if(p_data->dlg_tmout != 0)
  {
    ret = fw_tmr_create(ROOT_ID_POPUP, MSG_CANCEL, p_data->dlg_tmout, FALSE);

    MT_ASSERT(ret == SUCCESS);
  }

  // start loop to get msg
  if (type == DLG_FOR_SHOW)
  {
    ret = DLG_RET_NULL;
  }
  else
  {
    fw_default_mainwin_loop(ctrl_get_ctrl_id(p_cont));
    ret = g_dlg_ret;
    g_dlg_ret = last_ret;
  }

  return ret;
}


dlg_ret_t ui_comm_dlg_open_ex(comm_dlg_data_t *p_data)
{
  if(p_data->dlg_tmout)
  {
    return ui_comm_dlg_open(p_data, THIRD_LEVEL);
  }
  else
  {
    return ui_comm_dlg_open(p_data, SEC_LEVEL);
  }
}

void ui_comm_dlg_close(void)
{
  fw_tmr_destroy(ROOT_ID_POPUP, MSG_CANCEL);
  manage_close_menu(ROOT_ID_POPUP, 0, 0);
}


void ui_comm_dlg_close_ex(void)
{
  control_t *p_root = NULL;

  p_root = fw_find_root_by_id(ROOT_ID_POPUP);
  if(NULL != p_root)
  {
    if(ui_comm_dlg_get_last_priovity(p_root) > FIRST_LEVEL)
    {
        ui_comm_dlg_close();
    }
  }
}

static void fill_dlg_data(comm_dlg_data_t *p_data, u8 style, u16 strid,
                          rect_t *p_dlg_rc, u32 tm_out)
{
  p_data->parent_root = 0;
  p_data->style = style;
  p_data->content = strid;
  p_data->dlg_tmout = tm_out;

  if (p_dlg_rc != NULL)
  {
    p_data->x = p_dlg_rc->left;
    p_data->y = p_dlg_rc->top;
    p_data->w = RECTWP(p_dlg_rc);
    p_data->h = RECTHP(p_dlg_rc);
  }
  else
  {
    p_data->x = COMM_DLG_X;
    p_data->y = COMM_DLG_Y;
    p_data->w = COMM_DLG_W;
    p_data->h = COMM_DLG_H;
  }
}


void ui_comm_cfmdlg_open(rect_t *p_dlg_rc, u16 strid, do_func_t do_cmf, u32 tm_out)
{
  comm_dlg_data_t dlg_data;

  fill_dlg_data(&dlg_data,
                DLG_FOR_CONFIRM | DLG_STR_MODE_STATIC,
                strid, p_dlg_rc, tm_out);

  ui_comm_dlg_open(&dlg_data, SEC_LEVEL);

  DO_DLG_FUNC(do_cmf);
}


void ui_comm_cfmdlg_open_ex(rect_t *p_dlg_rc, u16 strid, do_func_t do_cmf, u32 tm_out)
{
  if(tm_out)
  {
    ui_comm_cfmdlg_open(p_dlg_rc, strid, do_cmf, tm_out);
  }
  else
  {
    ui_comm_cfmdlg_open(p_dlg_rc, strid, do_cmf, 0);
  }
}

static RET_CODE on_dlg_select(control_t *p_ctrl, u16 msg, u32 para1, u32 para2)
{
  ui_comm_dlg_close();
  return SUCCESS;
}


static RET_CODE on_dlg_cancel(control_t *p_ctrl, u16 msg, u32 para1, u32 para2)
{
  control_t *p_btn;
  
  p_btn = ctrl_get_child_by_id(p_ctrl, IDC_BTN_NO);

  if(p_btn != NULL)
  {
    ctrl_set_active_ctrl(p_ctrl, p_btn);
  }
  
  ui_comm_dlg_close();
  return SUCCESS;
}

static RET_CODE on_dlg_destroy(control_t *p_ctrl, u16 msg, u32 para1, u32 para2)
{
  control_t *p_btn;
  
  p_btn = ctrl_get_active_ctrl(p_ctrl);
  if (p_btn != NULL)
  {
    g_dlg_ret = ctrl_get_ctrl_id(p_btn) == IDC_BTN_YES ? \
      DLG_RET_YES : DLG_RET_NO;
  }
  else
  {
    g_dlg_ret = DLG_RET_NULL;
  }

  fw_tmr_destroy(ROOT_ID_POPUP, MSG_CANCEL);

  return ERR_NOFEATURE;
}



void ui_comm_cfmdlg_open_unistr(rect_t *p_dlg_rc, u16* p_unistr, do_func_t do_cmf, u32 tm_out, dlg_priority_t level)
{
  comm_dlg_data_t dlg_data;

  fill_dlg_data(&dlg_data,
                DLG_FOR_CONFIRM | DLG_STR_MODE_EXTSTR,
                (u32)p_unistr, p_dlg_rc, tm_out);

  ui_comm_dlg_open(&dlg_data, level);

  DO_DLG_FUNC(do_cmf);
}

void ui_comm_ask_for_dodlg_open_ex(rect_t *p_dlg_rc, u16 strid,
                                  do_func_t yes_do, do_func_t no_do, do_func_t close_do, u32 tm_out, dlg_priority_t level)
{
  comm_dlg_data_t dlg_data;
  dlg_ret_t ret;

  fill_dlg_data(&dlg_data,
                DLG_FOR_ASK | DLG_STR_MODE_STATIC,
                (u32)strid, p_dlg_rc, tm_out);

  ret = ui_comm_dlg_open(&dlg_data, level);

  if (ret == DLG_RET_YES)
  {
    DO_DLG_FUNC(yes_do);
  }
  else if (ret == DLG_RET_NO)
  {
    DO_DLG_FUNC(no_do);
  }
  else
  {
    DO_DLG_FUNC(close_do);
  }
}

void ui_comm_ask_for_dodlg_open(rect_t *p_dlg_rc, u16 strid,
                                  do_func_t do_func, do_func_t undo_func, u32 tm_out, dlg_priority_t level)
{
  ui_comm_ask_for_dodlg_open_ex(p_dlg_rc, strid, do_func, undo_func, undo_func, tm_out, level);
}


BEGIN_KEYMAP(comm_dlg_keymap, NULL)
  ON_EVENT(V_KEY_OK, MSG_SELECT)
  ON_EVENT(V_KEY_CANCEL, MSG_CANCEL)
  ON_EVENT(V_KEY_LEFT, MSG_FOCUS_LEFT)
  ON_EVENT(V_KEY_RIGHT, MSG_FOCUS_RIGHT)
  ON_EVENT(V_KEY_MENU, MSG_CANCEL)
END_KEYMAP(comm_dlg_keymap, NULL)


BEGIN_MSGPROC(comm_dlg_proc, cont_class_proc)
  ON_COMMAND(MSG_SELECT, on_dlg_select)
  ON_COMMAND(MSG_CANCEL, on_dlg_cancel)
  ON_COMMAND(MSG_DESTROY, on_dlg_destroy)
END_MSGPROC(comm_dlg_proc, cont_class_proc)


