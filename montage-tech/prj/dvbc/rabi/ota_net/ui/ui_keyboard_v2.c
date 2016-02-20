/****************************************************************************

 ****************************************************************************/

#include "ui_common.h"
#include "ui_keyboard_v2.h"

/* ------------------------- IME Manager ------------------------- */
#define IME_ATTRIB(caps_lock_ime, symbol_ime, prefer_flag) \
        (u32)((((u8)caps_lock_ime)<<24) | (((u8)symbol_ime)<<16) | ((u8)prefer_flag))

typedef enum
{
  INPUT_MODE_LITTLE_ENGLISH = 0,
  INPUT_MODE_CAPITAL_ENGLISH,
  INPUT_MODE_LITTLE_LATIN,
  INPUT_MODE_CAPITAL_LATIN,
  INPUT_MODE_ARABIC,
  INPUT_MODE_ARABIC_SYMBOL,

  INPUT_MODE_SYMBOL,
  INPUT_MODE_DEC_NUM,
  INPUT_MODE_LITTLE_HEX_NUM,
  INPUT_MODE_CAPITAL_HEX_NUM,
  INPUT_MODE_MAX_COUNT,
  INPUT_MODE_INVALID = 0xFF
} input_mode_t;

typedef enum
{
  IME_ATTR_NONE = 0,
  IME_ATTR_PREFERED = 0x1,
  IME_ATTR_SYMBOL = (0x1 << 1),
  
  IME_ATTR_MAX
} ime_attr_t;

typedef struct
{
  u8  mode_id;
  u16  input_type;
  u16 hint[4];
  u32  attrib;
} ime_mode_details_t;

typedef struct
{
  u8  pref_mode_id;
} allowed_ime_t;

static ime_mode_details_t ime_array[INPUT_MODE_MAX_COUNT] = 
{
  {
    INPUT_MODE_LITTLE_ENGLISH, 
    KB_INPUT_TYPE_SENTENCE | KB_INPUT_TYPE_ENGLISH,
    {'a', 'b', 'c', '\0'},
    IME_ATTRIB(INPUT_MODE_CAPITAL_ENGLISH, INPUT_MODE_SYMBOL, IME_ATTR_PREFERED)
  },
  {
    INPUT_MODE_CAPITAL_ENGLISH, 
    KB_INPUT_TYPE_SENTENCE | KB_INPUT_TYPE_ENGLISH,
    {'A', 'B', 'C', '\0'},
    IME_ATTRIB(INPUT_MODE_LITTLE_ENGLISH, INPUT_MODE_SYMBOL, IME_ATTR_NONE)
  },
  {
    INPUT_MODE_LITTLE_LATIN, 
    KB_INPUT_TYPE_SENTENCE,
    {0x00e4, 0x00e7, 0x00e9, '\0'},
    IME_ATTRIB(INPUT_MODE_CAPITAL_LATIN, INPUT_MODE_SYMBOL, IME_ATTR_PREFERED)
  },
  {
    INPUT_MODE_CAPITAL_LATIN, 
    KB_INPUT_TYPE_SENTENCE,
    {0x00e4, 0x00c4, 0x00c9, '\0'},
    IME_ATTRIB(INPUT_MODE_LITTLE_LATIN, INPUT_MODE_SYMBOL, IME_ATTR_NONE)
  },
  {
    INPUT_MODE_ARABIC, 
    KB_INPUT_TYPE_SENTENCE,
    {'a', 'r', '\0'},
    IME_ATTRIB(INPUT_MODE_INVALID, INPUT_MODE_ARABIC_SYMBOL, IME_ATTR_PREFERED)
  },
  {
    INPUT_MODE_ARABIC_SYMBOL, 
    KB_INPUT_TYPE_SENTENCE,
    {'\0'},
    IME_ATTRIB(INPUT_MODE_INVALID, INPUT_MODE_INVALID, IME_ATTR_SYMBOL)
  },
  {
    INPUT_MODE_SYMBOL, 
    KB_INPUT_TYPE_SENTENCE,
    {'\0'},
    IME_ATTRIB(INPUT_MODE_INVALID, INPUT_MODE_INVALID, IME_ATTR_SYMBOL)
  },
  {
    INPUT_MODE_DEC_NUM, 
    KB_INPUT_TYPE_DEC_NUMERIC,
    {'1', '2', '3', '\0'},
    IME_ATTRIB(INPUT_MODE_INVALID, INPUT_MODE_INVALID, IME_ATTR_PREFERED)
  },
  {
    INPUT_MODE_LITTLE_HEX_NUM, 
    KB_INPUT_TYPE_HEX_NUMERIC,
    {'h', 'e', 'x', '\0'},
    IME_ATTRIB(INPUT_MODE_CAPITAL_HEX_NUM, INPUT_MODE_INVALID, IME_ATTR_PREFERED)
  },
  {
    INPUT_MODE_CAPITAL_HEX_NUM, 
    KB_INPUT_TYPE_HEX_NUMERIC,
    {'H', 'E', 'X', '\0'},
    IME_ATTRIB(INPUT_MODE_LITTLE_HEX_NUM, INPUT_MODE_INVALID, IME_ATTR_NONE)
  }
};

/* ------------------------- Keyboard Config ------------------------- */
#define KB_MATRIX_KEYS_MAX_COUNT                    (42)
#define KB_CUSTOM_KEYS_MAX_COUNT                    (7)
#define KB_MBOX_KEYS_MAX_COUNT                      (KB_MATRIX_KEYS_MAX_COUNT+KB_CUSTOM_KEYS_MAX_COUNT)

typedef enum 
{
  KB_ENABLE_MASK_NONE = 0,
  KB_ENABLE_MASK_LANG_SWITCH = 0x1,
  KB_ENABLE_MASK_CAPSLOCK = (0x1 << 1),
  KB_ENABLE_MASK_SYMBPICKER = (0x1 << 2),

  KB_ENABLE_MASK_ALL = 0xFF
} kb_enable_mask_t;

typedef enum 
{
  KB_KEY_TYPE_NONE,
  KB_KEY_TYPE_CHAR,
  
  KB_KEY_TYPE_CAPSLOCK,
  KB_KEY_TYPE_SYMBPICKER,
  KB_KEY_TYPE_CURSER_LEFT,
  KB_KEY_TYPE_CURSER_RIGHT,
  KB_KEY_TYPE_DONE,
  KB_KEY_TYPE_DELETE,
  KB_KEY_TYPE_MAX_COUNT
} kb_key_type_t;

static u8 kb_key_type[KB_MBOX_KEYS_MAX_COUNT] = 
{
  KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR,
  KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR,
  KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR,
  KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR,
  KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CAPSLOCK, KB_KEY_TYPE_SYMBPICKER, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CURSER_LEFT, KB_KEY_TYPE_CURSER_RIGHT, KB_KEY_TYPE_DONE, KB_KEY_TYPE_DELETE
};

static u8 kb_num_key_type[KB_MBOX_KEYS_MAX_COUNT] = 
{
  KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR,
  KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE,
  KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE,
  KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE,
  KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_CAPSLOCK, KB_KEY_TYPE_SYMBPICKER, KB_KEY_TYPE_NONE, KB_KEY_TYPE_CURSER_LEFT, KB_KEY_TYPE_CURSER_RIGHT, KB_KEY_TYPE_DONE, KB_KEY_TYPE_DELETE
};

static u8 kb_hex_num_key_type[KB_MBOX_KEYS_MAX_COUNT] = 
{
  KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR,
  KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_CHAR, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE,
  KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE,
  KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE,
  KB_KEY_TYPE_NONE, KB_KEY_TYPE_NONE, KB_KEY_TYPE_CAPSLOCK, KB_KEY_TYPE_SYMBPICKER, KB_KEY_TYPE_NONE, KB_KEY_TYPE_CURSER_LEFT, KB_KEY_TYPE_CURSER_RIGHT, KB_KEY_TYPE_DONE, KB_KEY_TYPE_DELETE
};

u16 kb_custom_string[KB_CUSTOM_KEYS_MAX_COUNT] = {'\0', '\0', ' ', '\0', '\0', '\0', '\0'};

/* ------------------------- Local UI Control  ------------------------- */
enum local_msg
{
  MSG_LABEL_CHANGE = MSG_LOCAL_BEGIN + 1000,
  MSG_CAP,
  MSG_DEL_LETTER,
  MSG_LABEL_SAVE,
  MSG_LABEL_CANCEL,
};

enum cont_child_id
{
  IDC_INVALID = 0,
  IDC_EDIT = 1,
  IDC_KEYBOARD_CONT = 2,
  IDC_HELP = 3,
};

enum kb_cont_child_id
{
  IDC_LABEL_CONT = 1,
  IDC_MBOX = 2,
  IDC_MBOX_BUTTON = 3,
};

enum label_child_id
{
  /* input method */
  IDC_IPM_1 = 1,
};

/* ------------------------- IME Variable  ------------------------- */
static allowed_ime_t cur_allowed_input_mode[INPUT_MODE_MAX_COUNT];
static u8 cur_allowed_count;
static u8 cur_allowed_index;
static u8 cur_input_mode;
static u8 pre_input_mode;
static u8 ime_enable;

/* ------------------------- KB Variable  ------------------------- */
static u8 *cur_key_type = NULL;

/* ------------------------- Local Variable  ------------------------- */
static kb_param_t new_name;
static u16 cur_pos = 0;

static comm_help_data_t ipm_help_data = //help bar data
{
  1,
  1,
  {IDS_CHANGE},
  {IM_HELP_INFOR}
};

static u16 keyboard_input_method_keymap(u16 key);
static RET_CODE keyboard_input_method_proc(control_t *p_ctrl, u16 msg, u32 para1, u32 para2);

static RET_CODE on_del_letter(control_t *p_mbox, u16 msg, u32 para1, u32 para2);

static void abc_ime_update(control_t *p_mbox)
{
  u8 i = 0;
  u16 uni_str[2];
  u16 uni_char[KB_MATRIX_KEYS_MAX_COUNT] =
  {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', ':', ';', '?', ',', 
    '\\', '/'
  };

  uni_str[1] = '\0';

  for(i = 0; i < KB_MATRIX_KEYS_MAX_COUNT; i++)
  {
    uni_str[0] = uni_char[i];
    mbox_set_content_by_unistr(p_mbox, i, uni_str);
  }
}

static void ABC_ime_update(control_t *p_mbox)
{
  u8 i = 0;
  u16 uni_str[2];
  u16 uni_char[KB_MATRIX_KEYS_MAX_COUNT] =
  {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', ':', ';', '?', ',', 
    '\\', '/'
  };

  uni_str[1] = '\0';

  for(i = 0; i < KB_MATRIX_KEYS_MAX_COUNT; i++)
  {
    uni_str[0] = uni_char[i];
    mbox_set_content_by_unistr(p_mbox, i, uni_str);
  }
}

static void latin_ime_update(control_t *p_mbox)
{
  u16 uni_str[2];
  u16 uni_char[KB_MATRIX_KEYS_MAX_COUNT] =
  {
    0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8,
    0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2,
    0xF3, 0xF4, 0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD,
    0xFE, 0xFF, 0x015B, 0x015D, 0x015F, 0x0107, 0x0109, 0x0133, 0x0144, 0x0148,
    0x0153, 0x0177
  };
  u8 i;

  uni_str[1] = '\0';

  for(i = 0; i < KB_MATRIX_KEYS_MAX_COUNT; i++)
  {
    uni_str[0] = uni_char[i];
    mbox_set_content_by_unistr(p_mbox, i, uni_str);
  }
}

static void LATIN_ime_update(control_t *p_mbox)
{
  u16 uni_str[2];
  u16 uni_char[KB_MATRIX_KEYS_MAX_COUNT] =
  {
    0xDF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8,
    0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2,
    0xD3, 0xD4, 0xD5, 0xD6, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD,
    0xDE, 0x0178, 0x015A, 0x015C, 0x015E, 0x0106, 0x0108, 0x0132, 0x0143, 0x0147,
    0x0152, 0x0176
  };
  u8 i;

  uni_str[1] = '\0';

  for(i = 0; i < KB_MATRIX_KEYS_MAX_COUNT; i++)
  {
    uni_str[0] = uni_char[i];
    mbox_set_content_by_unistr(p_mbox, i, uni_str);
  }
}

static void arabic_ime_update(control_t *p_mbox)
{
  u8 i = 0;
  u16 uni_str[2];
  u16 uni_char[KB_MATRIX_KEYS_MAX_COUNT] =
  {
    0x0660, 0x0661, 0x0662, 0x0663, 0x0664, 0x0665, 0x0666, 0x0667, 0x0668, 0x0669,
    0x0636, 0x0635, 0x062B, 0x0642, 0x0641, 0x063A, 0x0639, 0x0647, 0x062E, 0x062D, 0x062C, //0x060C
    0x0634, 0x0633, 0x064A, 0x0628, 0x0644, 0x0627, 0x062A, 0x0646, 0x0645, 0x0643, 0x0637,
    0x0626, 0x0621, 0x0624, 0x0631, 0x0629, 0x0648, 0x0632, 0x0630, 0x062F, 0x0638, //0x061F
  };

  uni_str[1] = '\0';
  for(i = 0; i < KB_MATRIX_KEYS_MAX_COUNT; i++)
  {
    uni_str[0] = uni_char[i];
    mbox_set_content_by_unistr(p_mbox, i, uni_str);
  }
}

static void arabic_symbol_ime_update(control_t *p_mbox)
{
  u8 i = 0;
  u16 uni_str[2];
  u16 uni_char[KB_MATRIX_KEYS_MAX_COUNT] =
  {
#if 0
    '.', 0x00a2, 0x00b4, '!', ';', ':', '"', '\'', '?', '@', 0x00ab, //0x00bb, 
    '+', '-', '=', '%', '_', '*', '#', '~', '^', 0x00a1, 0x00bf, //0x0060, 
    '(', ')', '[', ']', '{', '}', '\\', '/', '<', '>', '|', 
    0x00A3, 0x20AC, '$', 0x00A5, 0x00A4, '&', 0x00a7, 0x0622, 0x0625, 0x061b, 0x00f7, 
#endif
    '.', 0xA2, 0xB4, '!', ':', '"', '\'', '@', 0xAB, 0xBB, 
    '+', '-', '=', '%', '_', '*', '#', '~', '^', 0xA1, 
    0xBF, '`', '(', ')', '[', ']', '{', '}', '\\', '/', 
    '<', '>', '|', '$', '&', 0xA7, 0x0622, 0x0625, 0x061B, 0xF7, 
    0x060C, 0x061F
  };

  uni_str[1] = '\0';
  for(i = 0; i < KB_MATRIX_KEYS_MAX_COUNT; i++)
  {
    uni_str[0] = uni_char[i];
    mbox_set_content_by_unistr(p_mbox, i, uni_str);
  }
}

static void symbol_ime_update(control_t *p_mbox)
{
  u8 i = 0;
  u16 uni_str[2];
  u16 uni_char[KB_MATRIX_KEYS_MAX_COUNT] =
  {
    '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*',
    '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', 
    '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|',
    '}', '~', 0xA1, 0xA2, 0xA3, 0xA5, 0xA9, 0xAB, 0xBB, 0xB7,
    0xBF, 0xF7
  };

  uni_str[1] = '\0';
  for(i = 0; i < KB_MATRIX_KEYS_MAX_COUNT; i++)
  {
    uni_str[0] = uni_char[i];
    mbox_set_content_by_unistr(p_mbox, i, uni_str);
  }
}

static void update_input_method(control_t *p_mbox, u8 mode_id)
{
  switch(mode_id)
  {
    case INPUT_MODE_LITTLE_ENGLISH:
    case INPUT_MODE_DEC_NUM:
    case INPUT_MODE_LITTLE_HEX_NUM:
      abc_ime_update(p_mbox);
      break;
    case INPUT_MODE_CAPITAL_ENGLISH:
    case INPUT_MODE_CAPITAL_HEX_NUM:
      ABC_ime_update(p_mbox);
      break;
    case INPUT_MODE_LITTLE_LATIN:
      latin_ime_update(p_mbox);
      break;
    case INPUT_MODE_CAPITAL_LATIN:
      LATIN_ime_update(p_mbox);
      break;
    case INPUT_MODE_SYMBOL:
      symbol_ime_update(p_mbox);
      break;
    case INPUT_MODE_ARABIC:
      arabic_ime_update(p_mbox);
      break;
    case INPUT_MODE_ARABIC_SYMBOL:
      arabic_symbol_ime_update(p_mbox);
      break;
    default:
      OS_PRINTF("input method update fail!!\n");
      break;
  }
}

BOOL ime_set_required_mode_list(u16 input_type)
{
  u8 i, j;
  
  for (i = 0, j = 0; i < INPUT_MODE_MAX_COUNT; i++)
  {
    if (IS_MASKED(ime_array[i].input_type, input_type)
        && IS_MASKED(ime_array[i].attrib, IME_ATTR_PREFERED))
    {
      cur_allowed_input_mode[j].pref_mode_id = ime_array[i].mode_id;
      j++;
    }
  }

  if (j == 0)
  {
    return FALSE;
  }

  cur_allowed_count = j;
  return TRUE;
}

RET_CODE open_keyboard_v2(u32 para1, u32 para2)
{
  control_t *p_cont = NULL;
  control_t *p_edit = NULL, *p_kb_cont = NULL;
  control_t *p_label_cont = NULL, *p_mbox = NULL;
  control_t *p_ipm = NULL;

  memcpy((void *)&new_name, (void *)para2, sizeof(kb_param_t));

  if (ime_set_required_mode_list(new_name.type) == FALSE)
  {
    return ERR_FAILURE;
  }

  cur_allowed_index = 0;
  pre_input_mode = INPUT_MODE_INVALID;
  cur_input_mode = cur_allowed_input_mode[cur_allowed_index].pref_mode_id;

  ime_enable = KB_ENABLE_MASK_ALL;
  if (new_name.type == KB_INPUT_TYPE_SENTENCE)
  {
    cur_key_type = kb_key_type;
  }
  else if (new_name.type == KB_INPUT_TYPE_DEC_NUMERIC)
  {
    ime_enable &= ~KB_ENABLE_MASK_LANG_SWITCH;
    ime_enable &= ~KB_ENABLE_MASK_CAPSLOCK;
    ime_enable &= ~KB_ENABLE_MASK_SYMBPICKER;
    cur_key_type = kb_num_key_type;
  }
  else if (new_name.type == KB_INPUT_TYPE_HEX_NUMERIC)
  {
    ime_enable &= ~KB_ENABLE_MASK_LANG_SWITCH;
    ime_enable &= ~KB_ENABLE_MASK_SYMBPICKER;
    cur_key_type = kb_hex_num_key_type;
  }
  else if (new_name.type == KB_INPUT_TYPE_ENGLISH)
  {
    ime_enable &= ~KB_ENABLE_MASK_LANG_SWITCH;
    cur_key_type = kb_key_type;
  }

  p_cont = fw_create_mainwin((u8)(ROOT_ID_KEYBOARD_V2),
                          KEYBOARD_V2_MENU_X, KEYBOARD_V2_MENU_Y,
                          KEYBOARD_V2_MENU_W, KEYBOARD_V2_MENU_H,
                          0, 0, OBJ_ATTR_ACTIVE, 0);
  if(p_cont == NULL)
  {
    return ERR_FAILURE;
  }
  ctrl_set_rstyle(p_cont, RSI_BOX1, RSI_BOX1, RSI_BOX1);
  ctrl_set_keymap(p_cont, ui_comm_root_keymap);
  ctrl_set_proc(p_cont, ui_comm_root_proc);

  p_edit = ctrl_create_ctrl(CTRL_EBOX, IDC_EDIT, 
                          NAME_EDIT_X, NAME_EDIT_Y,
                          NAME_EDIT_W, NAME_EDIT_H, 
                          p_cont, 0);
  ctrl_set_rstyle(p_edit, RSI_OTT_IM_SEARCH, RSI_OTT_IM_SEARCH, RSI_OTT_IM_SEARCH);
  ebox_set_font_style(p_edit, FSI_WHITE, FSI_WHITE, FSI_WHITE);
  ebox_set_maxtext(p_edit, new_name.max_len);
  //ebox_set_worktype(p_edit, EBOX_WORKTYPE_NO_CURSOR);
  ebox_set_worktype(p_edit, EBOX_WORKTYPE_SHIFT);
  //ebox_set_arabic_type(p_edit, EBOX_ARABIC_MASK);

  if(new_name.uni_str != NULL)
  {
    ebox_set_content_by_unistr(p_edit, new_name.uni_str);
  }

  p_kb_cont = ctrl_create_ctrl(CTRL_CONT, IDC_KEYBOARD_CONT,
                                KEYBOARD_CONT_X, KEYBOARD_CONT_Y,
                                KEYBOARD_CONT_W, KEYBOARD_CONT_H,
                                p_cont, 0);

  p_label_cont = ctrl_create_ctrl(CTRL_CONT, IDC_LABEL_CONT,
                                  LABEL_CONT_X, LABEL_CONT_Y,
                                  LABEL_CONT_W, LABEL_CONT_H,
                                  p_kb_cont, 0);
  ctrl_set_rstyle(p_label_cont, RSI_PBACK, RSI_PBACK, RSI_PBACK);

  if (IS_MASKED(ime_enable, KB_ENABLE_MASK_LANG_SWITCH))
  {
    ui_comm_help_create_ext(IPM_HELP_X, IPM_HELP_Y,
                                 IPM_HELP_W, IPM_HELP_H,
                                 &ipm_help_data, p_label_cont);
  }

  p_ipm = ctrl_create_ctrl(CTRL_TEXT, (u8)IDC_IPM_1,
                          IPM_COM_X, IPM_COM_Y, 
                          IPM_COM_W, IPM_COM_H,
                          p_label_cont, 0);
  ctrl_set_rstyle(p_ipm, RSI_PBACK, RSI_PBACK, RSI_PBACK);
  text_set_align_type(p_ipm, STL_RIGHT | STL_VCENTER);
  text_set_font_style(p_ipm, FSI_WHITE, FSI_WHITE, FSI_WHITE);
  text_set_content_type(p_ipm, TEXT_STRTYPE_UNICODE);
  text_set_content_by_unistr(p_ipm, ime_array[cur_input_mode].hint);

  /* specific IPM*/
  p_mbox = ctrl_create_ctrl(CTRL_MBOX, IDC_MBOX,
                            KEYBOARD_MBOX_V2_X, KEYBOARD_MBOX_V2_Y,
                            KEYBOARD_MBOX_V2_W, KEYBOARD_MBOX_V2_H,
                            p_kb_cont, 0);
  ctrl_set_keymap(p_mbox, keyboard_input_method_keymap);
  ctrl_set_proc(p_mbox, keyboard_input_method_proc);
  mbox_enable_icon_mode(p_mbox, TRUE);
  mbox_enable_string_mode(p_mbox, TRUE);
  mbox_set_count(p_mbox, KB_MBOX_KEYS_MAX_COUNT, KEYBOARD_MBOX_V2_COL, KEYBOARD_MBOX_V2_ROW);
  ctrl_set_mrect(p_mbox, 0, 0, KEYBOARD_MBOX_V2_W, KEYBOARD_MBOX_V2_H);
  mbox_set_item_interval(p_mbox, KEYBOARD_MBOX_V2_HGAP, KEYBOARD_MBOX_V2_VGAP);
  mbox_set_item_rstyle(p_mbox, RSI_OTT_KEYBOARD_KEY_HL, RSI_OTT_KEYBOARD_KEY_SH, RSI_OTT_KEYBOARD_KEY_HL);
  mbox_set_string_fstyle(p_mbox, FSI_WHITE, FSI_WHITE, FSI_WHITE);
  mbox_set_icon_offset(p_mbox, 0, 0);
  mbox_set_icon_align_type(p_mbox, STL_CENTER | STL_VCENTER);
  mbox_set_string_offset(p_mbox, 0, 0);
  mbox_set_string_align_type(p_mbox, STL_CENTER | STL_VCENTER);
  mbox_set_content_strtype(p_mbox, MBOX_STRTYPE_UNICODE);
  mbox_set_focus(p_mbox, cur_pos);

  if (IS_MASKED(ime_enable, KB_ENABLE_MASK_CAPSLOCK))
  {
    mbox_set_content_by_icon(p_mbox, (u16)(KB_MATRIX_KEYS_MAX_COUNT), IM_KEYBOARD_BUTTON_CAPS, IM_KEYBOARD_BUTTON_CAPS);
  }
  if (IS_MASKED(ime_enable, KB_ENABLE_MASK_SYMBPICKER))
  {
    mbox_set_content_by_icon(p_mbox, (u16)(KB_MATRIX_KEYS_MAX_COUNT+1), IM_KEYBOARD_BUTTON_PAGE, IM_KEYBOARD_BUTTON_PAGE);
  }
  if (cur_key_type[KB_MATRIX_KEYS_MAX_COUNT+2] == KB_KEY_TYPE_CHAR)
  {
    mbox_set_content_by_icon(p_mbox, (u16)(KB_MATRIX_KEYS_MAX_COUNT+2), IM_KEYBOARD_BUTTON_BLANK, IM_KEYBOARD_BUTTON_BLANK);
  }
  mbox_set_content_by_icon(p_mbox, (u16)(KB_MATRIX_KEYS_MAX_COUNT+3), IM_KEYBOARD_BUTTON_LEFT, IM_KEYBOARD_BUTTON_LEFT);
  mbox_set_content_by_icon(p_mbox, (u16)(KB_MATRIX_KEYS_MAX_COUNT+4), IM_KEYBOARD_BUTTON_RIGHT, IM_KEYBOARD_BUTTON_RIGHT);
  mbox_set_content_by_icon(p_mbox, (u16)(KB_MATRIX_KEYS_MAX_COUNT+5), IM_KEYBOARD_BUTTON_OK, IM_KEYBOARD_BUTTON_OK);
  mbox_set_content_by_icon(p_mbox, (u16)(KB_MATRIX_KEYS_MAX_COUNT+6), IM_KEYBOARD_BUTTON_DELETE, IM_KEYBOARD_BUTTON_DELETE);

  update_input_method(p_mbox, cur_input_mode);

  ebox_enter_edit(p_edit);
  ctrl_default_proc(p_mbox, MSG_GETFOCUS, 0, 0);
  ctrl_paint_ctrl(p_cont, FALSE);

  return SUCCESS;
}

static RET_CODE on_label_cap_mbox(control_t *p_mbox, u16 msg, u32 para1, u32 para2)
{
  control_t *p_label_cont = NULL;
  control_t *p_ipm = NULL;
  u32 temp_input_mode;

  if (!IS_MASKED(ime_enable, KB_ENABLE_MASK_CAPSLOCK))
  {
    return SUCCESS;
  }

  temp_input_mode = ime_array[cur_input_mode].attrib;
  temp_input_mode = temp_input_mode >> 24;

  if (temp_input_mode == INPUT_MODE_INVALID)
  {
    return SUCCESS;
  }

  pre_input_mode = cur_input_mode;
  cur_input_mode = (u8) temp_input_mode;

  p_label_cont = ctrl_get_child_by_id(p_mbox->p_parent, IDC_LABEL_CONT);
  p_ipm = ctrl_get_child_by_id(p_label_cont, IDC_IPM_1);

  text_set_content_by_unistr(p_ipm, ime_array[cur_input_mode].hint);
  ctrl_paint_ctrl(p_ipm, TRUE);

  update_input_method(p_mbox, cur_input_mode);
  ctrl_paint_ctrl(p_mbox, TRUE);

  return SUCCESS;
}

static RET_CODE on_label_pick_sym_mbox(control_t *p_mbox, u16 msg, u32 para1, u32 para2)
{
  u32 temp_input_mode;

  if (!IS_MASKED(ime_enable, KB_ENABLE_MASK_SYMBPICKER))
  {
    return SUCCESS;
  }

  if (IS_MASKED(ime_array[cur_input_mode].attrib, IME_ATTR_SYMBOL))
  {
    temp_input_mode = pre_input_mode;
  }
  else
  {
    temp_input_mode = ime_array[cur_input_mode].attrib;
    temp_input_mode = (temp_input_mode << 8);
    temp_input_mode = (temp_input_mode >> 24);
  }

  if (temp_input_mode == INPUT_MODE_INVALID)
  {
    return SUCCESS;
  }

  pre_input_mode = cur_input_mode;
  cur_input_mode = (u8) temp_input_mode;
  update_input_method(p_mbox, cur_input_mode);
  ctrl_paint_ctrl(p_mbox, TRUE);

  return SUCCESS;
}

static RET_CODE on_label_change_mbox(control_t *p_mbox, u16 msg,
                                     u32 para1, u32 para2)
{
  control_t *p_label_cont = NULL;
  control_t *p_ipm = NULL;

  if (!IS_MASKED(ime_enable, KB_ENABLE_MASK_LANG_SWITCH))
  {
    return SUCCESS;
  }

  cur_pos = mbox_get_focus(p_mbox);

  p_label_cont = ctrl_get_child_by_id(p_mbox->p_parent, IDC_LABEL_CONT);
  p_ipm = ctrl_get_child_by_id(p_label_cont, IDC_IPM_1);

  cur_allowed_index++;
  if (cur_allowed_index >= cur_allowed_count)
  {
    cur_allowed_index = 0;
  }
  pre_input_mode = cur_input_mode;
  cur_input_mode = cur_allowed_input_mode[cur_allowed_index].pref_mode_id;

  text_set_content_by_unistr(p_ipm, ime_array[cur_input_mode].hint);
  ctrl_paint_ctrl(p_ipm, TRUE);

  update_input_method(p_mbox, cur_input_mode);
  mbox_set_focus(p_mbox, cur_pos);
  ctrl_paint_ctrl(p_mbox, TRUE);

  return SUCCESS;
}

static RET_CODE on_label_save_mbox(control_t *p_mbox, u16 msg, u32 para1, u32 para2)
{
  control_t *p_edit = NULL;
  u16 *uni_str = NULL;

  p_edit = ctrl_get_child_by_id(p_mbox->p_parent->p_parent, IDC_EDIT);
  uni_str = ebox_get_content(p_edit);
  uni_strncpy(new_name.uni_str, uni_str, new_name.max_len);
  //memcpy(new_name.uni_str, uni_str, new_name.max_len);

  ebox_exit_edit(p_edit);
  ctrl_process_msg(p_mbox->p_parent->p_parent, MSG_EXIT, 0, 0);

  if (new_name.cb != NULL)
  {
    new_name.cb();
  }
  return SUCCESS;
}

#if 0
static RET_CODE on_label_cancel_mbox(control_t *p_mbox, u16 msg,
                                     u32 para1, u32 para2)
{
  control_t *p_edit = NULL;

  p_edit = ctrl_get_child_by_id(p_mbox->p_parent->p_parent, IDC_EDIT);
  ebox_exit_edit(p_edit);
  ctrl_process_msg(p_mbox->p_parent->p_parent, MSG_EXIT, 0, 0);
  return SUCCESS;
}
#endif

static RET_CODE on_change_focus_mbox(control_t *p_mbox, u16 msg,
                                     u32 para1, u32 para2)
{
  s16 new_pos;
  cur_pos = mbox_get_focus(p_mbox);

  switch(msg)
  {
    case MSG_FOCUS_RIGHT:
      
      if (cur_pos == KB_MBOX_KEYS_MAX_COUNT - 1)
      {
        cur_pos = KB_MBOX_KEYS_MAX_COUNT - (KB_MBOX_KEYS_MAX_COUNT % KEYBOARD_MBOX_V2_COL);
        mbox_set_focus(p_mbox, cur_pos);
        ctrl_paint_ctrl(p_mbox, TRUE);
        return SUCCESS;
      }
      break;

    case MSG_FOCUS_LEFT:
      if (cur_pos == KB_MBOX_KEYS_MAX_COUNT - (KB_MBOX_KEYS_MAX_COUNT % KEYBOARD_MBOX_V2_COL))
      {
        cur_pos = KB_MBOX_KEYS_MAX_COUNT - 1;
        mbox_set_focus(p_mbox, cur_pos);
        ctrl_paint_ctrl(p_mbox, TRUE);
        return SUCCESS;
      }
      break;

    case MSG_FOCUS_DOWN:
      new_pos = cur_pos + KEYBOARD_MBOX_V2_COL;
      if(new_pos >= KB_MBOX_KEYS_MAX_COUNT)
      {
        cur_pos = new_pos % KEYBOARD_MBOX_V2_COL;
        mbox_set_focus(p_mbox, cur_pos);
        ctrl_paint_ctrl(p_mbox, TRUE);
        return SUCCESS;
      }
      break;

    case MSG_FOCUS_UP:
      new_pos = cur_pos - KEYBOARD_MBOX_V2_COL;
      if (new_pos < 0)
      {
        new_pos += KEYBOARD_MBOX_V2_ROW * KEYBOARD_MBOX_V2_COL;
      }
      if (new_pos >= KB_MBOX_KEYS_MAX_COUNT)
      {
        cur_pos = new_pos - KEYBOARD_MBOX_V2_COL;
        mbox_set_focus(p_mbox, cur_pos);
        ctrl_paint_ctrl(p_mbox, TRUE);
        return SUCCESS;
      }
      break;

    default:
      break;
  }


  return ERR_NOFEATURE;
}

static RET_CODE on_input_letter(control_t *p_mbox, u16 msg, u32 para1, u32 para2)
{
  control_t *p_edit = NULL;
  control_t *p_kb_cont = NULL;
  u32 *p_str = NULL;
  u16 uni_str[2] = {0};
 
  p_kb_cont = p_mbox->p_parent;
  p_edit = ctrl_get_child_by_id(p_kb_cont->p_parent, IDC_EDIT);

  cur_pos = mbox_get_focus(p_mbox);

  if (cur_pos < KB_MATRIX_KEYS_MAX_COUNT)
  {// matrix key
    if (cur_key_type[cur_pos] == KB_KEY_TYPE_CHAR)              /* enter letter */
    {// fetch input char
      p_str = mbox_get_content_str(p_mbox, cur_pos);
      uni_strcpy(uni_str, (u16 *)(*p_str));

      ebox_input_uchar(p_edit, uni_str[0]);
      //ebox_class_proc(p_edit, MSG_KEYBOARD_CHAR, uni_str[0], 0);
      
      ctrl_paint_ctrl(p_edit, TRUE);
    }
  }
  else
  {// custom key
    if (cur_key_type[cur_pos] == KB_KEY_TYPE_CHAR)
    {// fetch input char
      uni_str[0] = kb_custom_string[cur_pos - KB_MATRIX_KEYS_MAX_COUNT];

      ebox_input_uchar(p_edit, uni_str[0]);
      //ebox_class_proc(p_edit, MSG_KEYBOARD_CHAR, uni_str[0], 0);
      
      ctrl_paint_ctrl(p_edit, TRUE);
    }
    else if (cur_key_type[cur_pos] == KB_KEY_TYPE_CAPSLOCK)
    {
        on_label_cap_mbox(p_mbox, msg, para1, para2);
    }
    else if (cur_key_type[cur_pos] == KB_KEY_TYPE_SYMBPICKER)
    {
        on_label_pick_sym_mbox(p_mbox, msg, para1, para2);
    }
    else if (cur_key_type[cur_pos] == KB_KEY_TYPE_CURSER_LEFT)
    {
      ctrl_process_msg(p_edit, MSG_FOCUS_LEFT, 0, 0);
    }
    else if (cur_key_type[cur_pos] == KB_KEY_TYPE_CURSER_RIGHT)
    {
      ctrl_process_msg(p_edit, MSG_FOCUS_RIGHT, 0, 0);
    }
    else if (cur_key_type[cur_pos] == KB_KEY_TYPE_DONE)
    {
        on_label_save_mbox(p_mbox, msg, para1, para2);
    }
    else if (cur_key_type[cur_pos] == KB_KEY_TYPE_DELETE)
    {
        on_del_letter(p_mbox, msg, para1, para2);
    }
  }
  #if 0
  if(cur_pos == 49)
  {
    ebox_back_space(p_edit);
    ctrl_paint_ctrl(p_edit, TRUE);
  }

  if (cur_key_type[cur_pos] == KB_KEY_TYPE_CURSER_LEFT)
  {
    if (ipm_status & IPM_STATUS_MASK_SYMBOL)
    {
      status = ipm_status & ~IPM_STATUS_MASK_SYMBOL;
      pref_ipm = get_ipm_cat_by_status(status);

      if (pref_ipm != IPM_CAT_INVALID)
      {
        ipm_status = status;
        p_label_cont = ctrl_get_child_by_id(p_mbox->p_parent, IDC_LABEL_CONT);
        p_ipm = ctrl_get_child_by_id(p_label_cont, IDC_IPM_1);

        text_set_content_by_unistr(p_ipm, input_method_data[pref_ipm].label);
        ctrl_paint_ctrl(p_ipm, TRUE);

        update_input_method(p_mbox, pref_ipm);
        ctrl_paint_ctrl(p_mbox, TRUE);
      }
    }
    //ctrl_process_msg(p_edit, MSG_FOCUS_LEFT, 0, 0);
  }

  if (cur_key_type[cur_pos] == KB_KEY_TYPE_CURSER_RIGHT)
  {
    if ((ipm_status & IPM_STATUS_MASK_SYMBOL) == 0)
    {
      status = ipm_status | IPM_STATUS_MASK_SYMBOL;
      pref_ipm = get_ipm_cat_by_status(status);

      if (pref_ipm != IPM_CAT_INVALID)
      {
        ipm_status = status;
        p_label_cont = ctrl_get_child_by_id(p_mbox->p_parent, IDC_LABEL_CONT);
        p_ipm = ctrl_get_child_by_id(p_label_cont, IDC_IPM_1);

        text_set_content_by_unistr(p_ipm, input_method_data[pref_ipm].label);
        ctrl_paint_ctrl(p_ipm, TRUE);

        update_input_method(p_mbox, pref_ipm);
        ctrl_paint_ctrl(p_mbox, TRUE);
      }
    }
    //ctrl_process_msg(p_edit, MSG_FOCUS_RIGHT, 0, 0);
  }
  #endif
  return SUCCESS;
}

static RET_CODE on_input_number(control_t *p_mbox, u16 msg,
                                u32 para1, u32 para2)
{
  control_t *p_edit = NULL;
  control_t *p_kb_cont = NULL;
  u16 uchar;
  u8 num;

  p_kb_cont = p_mbox->p_parent;
  p_edit = ctrl_get_child_by_id(p_kb_cont->p_parent, IDC_EDIT);

  num = (u8)(MSG_DATA_MASK & msg);

  OS_PRINTF("@@@on_input_number cur_input_mode=%d\n", cur_input_mode);

  switch(cur_input_mode)
  {
    case INPUT_MODE_LITTLE_ENGLISH:
    case INPUT_MODE_CAPITAL_ENGLISH:
    case INPUT_MODE_DEC_NUM:
    case INPUT_MODE_LITTLE_HEX_NUM:
    case INPUT_MODE_CAPITAL_HEX_NUM:
      uchar = (u16)('0' + num);
      ebox_input_uchar(p_edit, uchar);

      ctrl_paint_ctrl(p_edit, TRUE);
      break;

    default:
      break;
  }

  return SUCCESS;
}

static RET_CODE on_del_letter(control_t *p_mbox, u16 msg, u32 para1, u32 para2)
{
  control_t *p_edit = NULL;
  control_t *p_kb_cont = NULL;

  p_kb_cont = p_mbox->p_parent;
  p_edit = ctrl_get_child_by_id(p_kb_cont->p_parent, IDC_EDIT);

  ebox_back_space(p_edit);
  ctrl_paint_ctrl(p_edit, TRUE);

  return SUCCESS;
}


BEGIN_KEYMAP(keyboard_input_method_keymap, NULL)
ON_EVENT(V_KEY_UP, MSG_FOCUS_UP)
ON_EVENT(V_KEY_DOWN, MSG_FOCUS_DOWN)
ON_EVENT(V_KEY_LEFT, MSG_FOCUS_LEFT)
ON_EVENT(V_KEY_RIGHT, MSG_FOCUS_RIGHT)
ON_EVENT(V_KEY_OK, MSG_SELECT)
ON_EVENT(V_KEY_INFO, MSG_LABEL_CHANGE)
//ON_EVENT(V_KEY_GREEN, MSG_CAP)
//ON_EVENT(V_KEY_PAGE_UP, MSG_PAGE_UP)
//ON_EVENT(V_KEY_PAGE_DOWN, MSG_PAGE_DOWN)
ON_EVENT(V_KEY_0, MSG_NUMBER | 0)
ON_EVENT(V_KEY_1, MSG_NUMBER | 1)
ON_EVENT(V_KEY_2, MSG_NUMBER | 2)
ON_EVENT(V_KEY_3, MSG_NUMBER | 3)
ON_EVENT(V_KEY_4, MSG_NUMBER | 4)
ON_EVENT(V_KEY_5, MSG_NUMBER | 5)
ON_EVENT(V_KEY_6, MSG_NUMBER | 6)
ON_EVENT(V_KEY_7, MSG_NUMBER | 7)
ON_EVENT(V_KEY_8, MSG_NUMBER | 8)
ON_EVENT(V_KEY_9, MSG_NUMBER | 9)
//ON_EVENT(V_KEY_RED, MSG_DEL_LETTER)
//ON_EVENT(V_KEY_YELLOW, MSG_LABEL_SAVE)
//ON_EVENT(V_KEY_BLUE, MSG_LABEL_CANCEL)
END_KEYMAP(keyboard_input_method_keymap, NULL)

BEGIN_MSGPROC(keyboard_input_method_proc, mbox_class_proc)
ON_COMMAND(MSG_FOCUS_UP, on_change_focus_mbox)
ON_COMMAND(MSG_FOCUS_DOWN, on_change_focus_mbox)
ON_COMMAND(MSG_FOCUS_LEFT, on_change_focus_mbox)
ON_COMMAND(MSG_FOCUS_RIGHT, on_change_focus_mbox)
ON_COMMAND(MSG_SELECT, on_input_letter)
ON_COMMAND(MSG_LABEL_CHANGE, on_label_change_mbox)
//ON_COMMAND(MSG_CAP, on_label_cap_mbox)
//ON_COMMAND(MSG_PAGE_UP, on_label_pick_sym_mbox)
//ON_COMMAND(MSG_PAGE_DOWN, on_label_pick_sym_mbox)
ON_COMMAND(MSG_NUMBER, on_input_number)
//ON_COMMAND(MSG_DEL_LETTER, on_del_letter)
//ON_COMMAND(MSG_LABEL_SAVE, on_label_save_mbox)
//ON_COMMAND(MSG_LABEL_CANCEL, on_label_cancel_mbox)
END_MSGPROC(keyboard_input_method_proc, mbox_class_proc)

