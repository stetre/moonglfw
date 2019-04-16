/* The MIT License (MIT)
 *
 * Copyright (c) 2019 Stefano Trettel
 *
 * Software repository: MoonGLFW, https://github.com/stetre/moonglfw
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "internal.h"

/*------------------------------------------------------------------------------*
 | Code<->string map for enumerations                                           |
 *------------------------------------------------------------------------------*/


/* code <-> string record */
#define rec_t struct rec_s
struct rec_s {
    RB_ENTRY(rec_s) CodeEntry;
    RB_ENTRY(rec_s) StringEntry;
    int domain;
    int code;  /* (domain, code) = search key in code tree */
    char     *str;  /* (domain, str) = search key in string tree */
};

/* compare functions */
static int cmp_code(rec_t *rec1, rec_t *rec2) 
    { 
    if(rec1->domain != rec2->domain)
        return (rec1->domain < rec2->domain ? -1 : rec1->domain > rec2->domain);
    return (rec1->code < rec2->code ? -1 : rec1->code > rec2->code);
    } 

static int cmp_str(rec_t *rec1, rec_t *rec2) 
    { 
    if(rec1->domain != rec2->domain)
        return (rec1->domain < rec2->domain ? -1 : rec1->domain > rec2->domain);
    return strcmp(rec1->str, rec2->str);
    } 

static RB_HEAD(CodeTree, rec_s) CodeHead = RB_INITIALIZER(&CodeHead);
RB_PROTOTYPE_STATIC(CodeTree, rec_s, CodeEntry, cmp_code) 
RB_GENERATE_STATIC(CodeTree, rec_s, CodeEntry, cmp_code) 

static RB_HEAD(StringTree, rec_s) StringHead = RB_INITIALIZER(&StringHead);
RB_PROTOTYPE_STATIC(StringTree, rec_s, StringEntry, cmp_str) 
RB_GENERATE_STATIC(StringTree, rec_s, StringEntry, cmp_str) 
 
static rec_t *code_remove(rec_t *rec)
    { return RB_REMOVE(CodeTree, &CodeHead, rec); }
static rec_t *code_insert(rec_t *rec)
    { return RB_INSERT(CodeTree, &CodeHead, rec); }
static rec_t *code_search(int domain, int code)
    { rec_t tmp; tmp.domain = domain; tmp.code = code; return RB_FIND(CodeTree, &CodeHead, &tmp); }
static rec_t *code_first(int domain, int code)
    { rec_t tmp; tmp.domain = domain; tmp.code = code; return RB_NFIND(CodeTree, &CodeHead, &tmp); }
static rec_t *code_next(rec_t *rec)
    { return RB_NEXT(CodeTree, &CodeHead, rec); }
#if 0
static rec_t *code_prev(rec_t *rec)
    { return RB_PREV(CodeTree, &CodeHead, rec); }
static rec_t *code_min(void)
    { return RB_MIN(CodeTree, &CodeHead); }
static rec_t *code_max(void)
    { return RB_MAX(CodeTree, &CodeHead); }
static rec_t *code_root(void)
    { return RB_ROOT(&CodeHead); }
#endif
 
static rec_t *str_remove(rec_t *rec)
    { return RB_REMOVE(StringTree, &StringHead, rec); }
static rec_t *str_insert(rec_t *rec)
    { return RB_INSERT(StringTree, &StringHead, rec); }
static rec_t *str_search(int domain, const char* str)
    { rec_t tmp; tmp.domain = domain; tmp.str = (char*)str; return RB_FIND(StringTree, &StringHead, &tmp); }
#if 0
static rec_t *str_first(int domain, const char* str )
    { rec_t tmp; tmp.domain = domain; tmp.str = str; return RB_NFIND(StringTree, &StringHead, &tmp); }
static rec_t *str_next(rec_t *rec)
    { return RB_NEXT(StringTree, &StringHead, rec); }
static rec_t *str_prev(rec_t *rec)
    { return RB_PREV(StringTree, &StringHead, rec); }
static rec_t *str_min(void)
    { return RB_MIN(StringTree, &StringHead); }
static rec_t *str_max(void)
    { return RB_MAX(StringTree, &StringHead); }
static rec_t *str_root(void)
    { return RB_ROOT(&StringHead); }
#endif


static int enums_new(lua_State *L, int domain, int code, const char *str)
    {
    rec_t *rec;
    if((rec = (rec_t*)Malloc(L, sizeof(rec_t))) == NULL) 
        return luaL_error(L, errstring(ERR_MEMORY));

    memset(rec, 0, sizeof(rec_t));
    rec->domain = domain;
    rec->code = code;
    rec->str = Strdup(L, str);
    if(code_search(domain, code) || str_search(domain, str))
        { 
        Free(L, rec->str);
        Free(L, rec); 
        return unexpected(L); /* duplicate value */
        }
    code_insert(rec);
    str_insert(rec);
    return 0;
    }

static void enums_free(lua_State *L, rec_t* rec)
    {
    if(code_search(rec->domain, rec->code) == rec)
        code_remove(rec);
    if(str_search(rec->domain, rec->str) == rec)
        str_remove(rec);
    Free(L, rec->str);
    Free(L, rec);   
    }

void enums_free_all(lua_State *L)
    {
    rec_t *rec;
    while((rec = code_first(0, 0)))
        enums_free(L, rec);
    }

#if 0
int enums_code(int domain, const char *str, int* found)
    {
    rec_t *rec = str_search(domain, str);
    if(!rec)
        { *found = 0; return 0; }
    *found = 1; 
    return rec->code;
    }

const char* enums_string(int domain, int code)
    {
    rec_t *rec = code_search(domain, code);
    if(!rec)
        return NULL;
    return rec->str;
    }

#endif


int enums_test(lua_State *L, int domain, int arg, int *err)
    {
    rec_t *rec;
    const char *s = luaL_optstring(L, arg, NULL);

    if(!s)
        { *err = ERR_NOTPRESENT; return 0; }

    rec = str_search(domain, s);
    if(!rec)
        { *err = ERR_VALUE; return 0; }
    
    *err = ERR_SUCCESS;
    return rec->code;
    }

int enums_check(lua_State *L, int domain, int arg)
    {
    rec_t *rec;
    const char *s = luaL_checkstring(L, arg);

    rec = str_search(domain, s);
    if(!rec)
        return luaL_argerror(L, arg, badvalue(L, s));
    
    return rec->code;
    }

int enums_push(lua_State *L, int domain, int code)
    {
    rec_t *rec = code_search(domain, code);

    if(!rec)
        return unexpected(L);

    lua_pushstring(L, rec->str);
    return 1;
    }

int enums_values(lua_State *L, int domain)
    {
    int i;
    rec_t *rec;

    lua_newtable(L);
    i = 1;
    rec = code_first(domain, 0);
    while(rec)
        {
        if(rec->domain == domain)
            {
            lua_pushstring(L, rec->str);
            lua_rawseti(L, -2, i++);
            }
        rec = code_next(rec);
        }

    return 1;
    }


int* enums_checklist(lua_State *L, int domain, int arg, int *count, int *err)
    {
    int* list;
    int i;

    *count = 0;
    *err = 0;
    if(lua_isnoneornil(L, arg))
        { *err = ERR_NOTPRESENT; return NULL; }
    if(lua_type(L, arg) != LUA_TTABLE)
        { *err = ERR_TABLE; return NULL; }

    *count = luaL_len(L, arg);
    if(*count == 0)
        { *err = ERR_NOTPRESENT; return NULL; }

    list = (int*)MallocNoErr(L, sizeof(int) * (*count));
    if(!list)
        { *count = 0; *err = ERR_MEMORY; return NULL; }

    for(i=0; i<*count; i++)
        {
        lua_rawgeti(L, arg, i+1);
        list[i] = enums_test(L, domain, -1, err);
        lua_pop(L, 1);
        if(*err)
            { Free(L, list); *count = 0; return NULL; }
        }
    return list;
    }

void enums_freelist(lua_State *L, int *list)
    {
    if(!list)
        return;
    Free(L, list);
    }

/*------------------------------------------------------------------------------*
 |                                                                              |
 *------------------------------------------------------------------------------*/

static int Enum(lua_State *L)
/* { strings } = glfw.enum('type') lists all the values for a given enum type */
    { 
    const char *s = luaL_checkstring(L, 1); 
#define CASE(xxx) if(strcmp(s, ""#xxx) == 0) return values##xxx(L)
    CASE(action);
    CASE(button);
    CASE(key);
    CASE(target);
    CASE(api);
    CASE(robustness);
    CASE(releasebehavior);
    CASE(profile);
    CASE(inputmode);
    CASE(cursormode);
    CASE(cursorshape);
//  CASE(gamepadbutton);
//  CASE(gamepadaxis);
#undef CASE
    return 0;
    }

static const struct luaL_Reg Functions[] = 
    {
        { "enum", Enum },
        { NULL, NULL } /* sentinel */
    };


void moonglfw_open_enums(lua_State *L)
    {
    int domain;

    luaL_setfuncs(L, Functions, 0);

    /* Add all the code<->string mappings */
#define ADD(what, s) do { enums_new(L, domain, GLFW_##what, s); } while(0)
    domain = DOMAIN_ACTION; 
    ADD(PRESS, "press");
    ADD(REPEAT, "repeat");
    ADD(RELEASE, "release");

    domain = DOMAIN_BUTTON;
    ADD(MOUSE_BUTTON_1, "left");
    ADD(MOUSE_BUTTON_2, "right");
    ADD(MOUSE_BUTTON_3, "middle");
    ADD(MOUSE_BUTTON_4, "button 4");
    ADD(MOUSE_BUTTON_5, "button 5");
    ADD(MOUSE_BUTTON_6, "button 6");
    ADD(MOUSE_BUTTON_7, "button 7");
    ADD(MOUSE_BUTTON_8, "button 8");

    domain = DOMAIN_KEY;
    ADD(KEY_UNKNOWN, "unknown");
    ADD(KEY_SPACE, "space");
    ADD(KEY_APOSTROPHE, "apostrophe");
    ADD(KEY_COMMA, "comma");
    ADD(KEY_MINUS, "minus");
    ADD(KEY_PERIOD, "period");
    ADD(KEY_SLASH, "slash");
    ADD(KEY_0, "0");
    ADD(KEY_1, "1");
    ADD(KEY_2, "2");
    ADD(KEY_3, "3");
    ADD(KEY_4, "4");
    ADD(KEY_5, "5");
    ADD(KEY_6, "6");
    ADD(KEY_7, "7");
    ADD(KEY_8, "8");
    ADD(KEY_9, "9");
    ADD(KEY_SEMICOLON, "semicolon");
    ADD(KEY_EQUAL, "equal");
    ADD(KEY_A, "a");
    ADD(KEY_B, "b");
    ADD(KEY_C, "c");
    ADD(KEY_D, "d");
    ADD(KEY_E, "e");
    ADD(KEY_F, "f");
    ADD(KEY_G, "g");
    ADD(KEY_H, "h");
    ADD(KEY_I, "i");
    ADD(KEY_J, "j");
    ADD(KEY_K, "k");
    ADD(KEY_L, "l");
    ADD(KEY_M, "m");
    ADD(KEY_N, "n");
    ADD(KEY_O, "o");
    ADD(KEY_P, "p");
    ADD(KEY_Q, "q");
    ADD(KEY_R, "r");
    ADD(KEY_S, "s");
    ADD(KEY_T, "t");
    ADD(KEY_U, "u");
    ADD(KEY_V, "v");
    ADD(KEY_W, "w");
    ADD(KEY_X, "x");
    ADD(KEY_Y, "y");
    ADD(KEY_Z, "z");
    ADD(KEY_LEFT_BRACKET, "left bracket");
    ADD(KEY_BACKSLASH, "backslash");
    ADD(KEY_RIGHT_BRACKET, "right bracket");
    ADD(KEY_GRAVE_ACCENT, "grave accent");
    ADD(KEY_WORLD_1, "world 1");
    ADD(KEY_WORLD_2, "world 2");
    ADD(KEY_ESCAPE, "escape");
    ADD(KEY_ENTER, "enter");
    ADD(KEY_TAB, "tab");
    ADD(KEY_BACKSPACE, "backspace");
    ADD(KEY_INSERT, "insert");
    ADD(KEY_DELETE, "delete");
    ADD(KEY_RIGHT, "right");
    ADD(KEY_LEFT, "left");
    ADD(KEY_DOWN, "down");
    ADD(KEY_UP, "up");
    ADD(KEY_PAGE_UP, "page up");
    ADD(KEY_PAGE_DOWN, "page down");
    ADD(KEY_HOME, "home");
    ADD(KEY_END, "end");
    ADD(KEY_CAPS_LOCK, "caps lock");
    ADD(KEY_SCROLL_LOCK, "scroll lock");
    ADD(KEY_NUM_LOCK, "num lock");
    ADD(KEY_PRINT_SCREEN, "print screen");
    ADD(KEY_PAUSE, "pause");
    ADD(KEY_F1, "f1");
    ADD(KEY_F2, "f2");
    ADD(KEY_F3, "f3");
    ADD(KEY_F4, "f4");
    ADD(KEY_F5, "f5");
    ADD(KEY_F6, "f6");
    ADD(KEY_F7, "f7");
    ADD(KEY_F8, "f8");
    ADD(KEY_F9, "f9");
    ADD(KEY_F10, "f10");
    ADD(KEY_F11, "f11");
    ADD(KEY_F12, "f12");
    ADD(KEY_F13, "f13");
    ADD(KEY_F14, "f14");
    ADD(KEY_F15, "f15");
    ADD(KEY_F16, "f16");
    ADD(KEY_F17, "f17");
    ADD(KEY_F18, "f18");
    ADD(KEY_F19, "f19");
    ADD(KEY_F20, "f20");
    ADD(KEY_F21, "f21");
    ADD(KEY_F22, "f22");
    ADD(KEY_F23, "f23");
    ADD(KEY_F24, "f24");
    ADD(KEY_F25, "f25");
    ADD(KEY_KP_0, "kp 0");
    ADD(KEY_KP_1, "kp 1");
    ADD(KEY_KP_2, "kp 2");
    ADD(KEY_KP_3, "kp 3");
    ADD(KEY_KP_4, "kp 4");
    ADD(KEY_KP_5, "kp 5");
    ADD(KEY_KP_6, "kp 6");
    ADD(KEY_KP_7, "kp 7");
    ADD(KEY_KP_8, "kp 8");
    ADD(KEY_KP_9, "kp 9");
    ADD(KEY_KP_DECIMAL, "kp decimal");
    ADD(KEY_KP_DIVIDE, "kp divide");
    ADD(KEY_KP_MULTIPLY, "kp multiply");
    ADD(KEY_KP_SUBTRACT, "kp subtract");
    ADD(KEY_KP_ADD, "kp add");
    ADD(KEY_KP_ENTER, "kp enter");
    ADD(KEY_KP_EQUAL, "kp equal");
    ADD(KEY_LEFT_SHIFT, "left shift");
    ADD(KEY_LEFT_CONTROL, "left control");
    ADD(KEY_LEFT_ALT, "left alt");
    ADD(KEY_LEFT_SUPER, "left super");
    ADD(KEY_RIGHT_SHIFT, "right shift");
    ADD(KEY_RIGHT_CONTROL, "right control");
    ADD(KEY_RIGHT_ALT, "right alt");
    ADD(KEY_RIGHT_SUPER, "right super");
    ADD(KEY_MENU, "menu");

    domain = DOMAIN_TARGET;
    ADD(RESIZABLE, "resizable");
    ADD(VISIBLE, "visible");
    ADD(DECORATED, "decorated");
    ADD(FOCUSED, "focused");
    ADD(AUTO_ICONIFY, "auto iconify");
    ADD(FLOATING, "floating");
    ADD(MAXIMIZED, "maximized");
    ADD(CENTER_CURSOR, "center cursor");
    ADD(TRANSPARENT_FRAMEBUFFER, "transparent framebuffer");
    ADD(HOVERED, "hovered");
    ADD(FOCUS_ON_SHOW, "focus on show");
    ADD(SCALE_TO_MONITOR, "scale to monitor");
    ADD(RED_BITS, "red bits");
    ADD(GREEN_BITS, "green bits");
    ADD(BLUE_BITS, "blue bits");
    ADD(ALPHA_BITS, "alpha bits");
    ADD(DEPTH_BITS, "depth bits");
    ADD(STENCIL_BITS, "stencil bits");
    ADD(ACCUM_RED_BITS, "accum red bits");
    ADD(ACCUM_GREEN_BITS, "accum green bits");
    ADD(ACCUM_BLUE_BITS, "accum blue bits");
    ADD(ACCUM_ALPHA_BITS, "accum alpha bits");
    ADD(AUX_BUFFERS, "aux buffers");
    ADD(SAMPLES, "samples");
    ADD(REFRESH_RATE, "refresh rate");
    ADD(STEREO, "stereo");
    ADD(SRGB_CAPABLE, "srgb capable");
    ADD(DOUBLEBUFFER, "doublebuffer");
    ADD(CLIENT_API, "client api");
    ADD(CONTEXT_CREATION_API, "context creation api"); //@@
    ADD(CONTEXT_VERSION_MAJOR, "context version major");
    ADD(CONTEXT_VERSION_MINOR, "context version minor");
    ADD(CONTEXT_ROBUSTNESS, "context robustness");
    ADD(CONTEXT_RELEASE_BEHAVIOR, "context release behavior");
    ADD(OPENGL_FORWARD_COMPAT, "opengl forward compat");
    ADD(OPENGL_DEBUG_CONTEXT, "opengl debug context");
    ADD(OPENGL_PROFILE, "opengl profile");
    ADD(ICONIFIED, "iconified");
    ADD(CONTEXT_REVISION, "context revision");
    ADD(X11_CLASS_NAME, "x11 class name");
    ADD(X11_INSTANCE_NAME, "x11 instance name");

    domain = DOMAIN_API;
    ADD(NO_API, "no api");
    ADD(OPENGL_API, "opengl");
    ADD(OPENGL_ES_API, "opengl es");

    domain = DOMAIN_ROBUSTNESS;
    ADD(NO_ROBUSTNESS, "no robustness");
    ADD(NO_RESET_NOTIFICATION, "no reset notification");
    ADD(LOSE_CONTEXT_ON_RESET, "lose context on reset");

    domain = DOMAIN_RELEASE_BEHAVIOR;
    ADD(ANY_RELEASE_BEHAVIOR, "any");
    ADD(RELEASE_BEHAVIOR_FLUSH, "flush");
    ADD(RELEASE_BEHAVIOR_NONE, "none");

    domain = DOMAIN_PROFILE;
    ADD(OPENGL_ANY_PROFILE, "any");
    ADD(OPENGL_CORE_PROFILE, "core");
    ADD(OPENGL_COMPAT_PROFILE, "compat");

    domain = DOMAIN_INPUT_MODE;
    ADD(CURSOR, "cursor");
    ADD(STICKY_KEYS, "sticky keys");
    ADD(STICKY_MOUSE_BUTTONS, "sticky mouse buttons");
    ADD(LOCK_KEY_MODS, "lock key mods");
    ADD(RAW_MOUSE_MOTION, "raw mouse motion");

    domain = DOMAIN_CURSOR_MODE;
    ADD(CURSOR_NORMAL, "normal");
    ADD(CURSOR_HIDDEN, "hidden");
    ADD(CURSOR_DISABLED, "disabled");

    domain = DOMAIN_CURSOR_SHAPE;
    ADD(ARROW_CURSOR, "arrow");
    ADD(IBEAM_CURSOR, "ibeam");
    ADD(CROSSHAIR_CURSOR, "crosshair");
    ADD(HAND_CURSOR, "hand");
    ADD(HRESIZE_CURSOR, "hresize");
    ADD(VRESIZE_CURSOR, "vresize");

#if 0
    domain = DOMAIN_GAMEPAD_BUTTON;
    ADD(GAMEPAD_BUTTON_A, "a"); /* cross */
    ADD(GAMEPAD_BUTTON_B, "b"); /* circle */
    ADD(GAMEPAD_BUTTON_X, "x"); /* square */
    ADD(GAMEPAD_BUTTON_Y, "y"); /* triangle */
    ADD(GAMEPAD_BUTTON_LEFT_BUMPER, "left bumper");
    ADD(GAMEPAD_BUTTON_RIGHT_BUMPER, "right bumper");
    ADD(GAMEPAD_BUTTON_BACK, "back");
    ADD(GAMEPAD_BUTTON_START, "start");
    ADD(GAMEPAD_BUTTON_GUIDE, "guide");
    ADD(GAMEPAD_BUTTON_LEFT_THUMB, "left thumb");
    ADD(GAMEPAD_BUTTON_RIGHT_THUMB, "right thumb");
    ADD(GAMEPAD_BUTTON_DPAD_UP, "dpad up");
    ADD(GAMEPAD_BUTTON_DPAD_RIGHT, "dpad right");
    ADD(GAMEPAD_BUTTON_DPAD_DOWN, "dpad down");
    ADD(GAMEPAD_BUTTON_DPAD_LEFT, "dpad left");

    domain = DOMAIN_GAMEPAD_AXIS;
    ADD(GAMEPAD_AXIS_LEFT_X, "left x");
    ADD(GAMEPAD_AXIS_LEFT_Y, "left y");
    ADD(GAMEPAD_AXIS_RIGHT_X, "right x");
    ADD(GAMEPAD_AXIS_RIGHT_Y, "right y");
    ADD(GAMEPAD_AXIS_LEFT_TRIGGER, "left trigger");
    ADD(GAMEPAD_AXIS_RIGHT_TRIGGER, "right trigger");
#endif

#undef ADD
    }

