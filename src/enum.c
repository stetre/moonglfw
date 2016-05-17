/* The MIT License (MIT)
 *
 * Copyright (c) 2016 Stefano Trettel
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

int enumCheck(lua_State *L, int arg, enum_t *e)
    { return e->codes[checkoption(L, arg, NULL, e->strings)]; }

int enumPush(lua_State *L, int code, enum_t *e)
    {
    size_t i;
    for(i = 0; i < e->n; i++) 
        { if(e->codes[i]==code) 
            { 
            lua_pushstring(L, e->strings[i]); 
            return 1; 
            } 
        }
    //return luaL_error(L, "invalid enum value %d", code);
    lua_pushfstring(L, "%d???", code); 
    return 1;
    }

const char* enumToString(int code, enum_t *e)
    {
    size_t i;
    for(i = 0; i < e->n; i++) 
        if(e->codes[i]==code) return e->strings[i];
    return NULL;
    }

int enumAssert(lua_State *L, enum_t *e, int code, const char *string)
    {
    const char *s = enumToString(code, e);
    if( (s==NULL) || (strcmp(s, string) !=0))
        return unexpected(L);       
    return 0;
    }

int enumOrUint(lua_State *L, int arg, int *dst, enum_t *e, int def)
/* The element at the index arg on the Lua stack may be an enum (string),
 * or an integer. In the first case the enum code is stored in 'code' and
 * 'def' is returned. In the second case the integer value is returned.
 */
    {
    if(lua_type(L, arg) == LUA_TSTRING)
        {
        *dst = enumCheck(L, arg, e);
        return def;
        }
    return (int)luaL_checkinteger(L, arg);
    }



ENUM_STRINGS(ActionStrings) = {
    "press",
    "repeat",
    "release",
    NULL
};
ENUM_CODES(ActionCodes) = {
    GLFW_PRESS,
    GLFW_REPEAT,
    GLFW_RELEASE,
};
ENUM_T(ActionEnum, ActionStrings, ActionCodes)
#define CheckAction(L, arg) enumCheck((L), (arg), &ActionEnum)
#define PushAction(L, code) enumPush((L), (code), &ActionEnum)

enum_t *enumAction(void)
    { return &ActionEnum; }

ENUM_STRINGS(ButtonStrings) = {
    "left",
    "right",
    "middle",
    "button 4",
    "button 5",
    "button 6",
    "button 7",
    "button 8",
    NULL
};
ENUM_CODES(ButtonCodes) = {
    GLFW_MOUSE_BUTTON_1,
    GLFW_MOUSE_BUTTON_2,
    GLFW_MOUSE_BUTTON_3,
    GLFW_MOUSE_BUTTON_4,
    GLFW_MOUSE_BUTTON_5,
    GLFW_MOUSE_BUTTON_6,
    GLFW_MOUSE_BUTTON_7,
    GLFW_MOUSE_BUTTON_8,
};
ENUM_T(ButtonEnum, ButtonStrings, ButtonCodes)
#define CheckButton(L, arg) enumCheck((L), (arg), &ButtonEnum)
#define PushButton(L, code) enumPush((L), (code), &ButtonEnum)

enum_t *enumButton(void)
    { return &ButtonEnum; }


ENUM_STRINGS(KeyStrings) = {
    "unknown",
    "space",
    "apostrophe",
    "comma",
    "minus",
    "period",
    "slash",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "semicolon",
    "equal",
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "t",
    "u",
    "v",
    "w",
    "x",
    "y",
    "z",
    "left bracket",
    "backslash",
    "right bracket",
    "grave accent",
    "world 1",
    "world 2",
    "escape",
    "enter",
    "tab",
    "backspace",
    "insert",
    "delete",
    "right",
    "left",
    "down",
    "up",
    "page up",
    "page down",
    "home",
    "end",
    "caps lock",
    "scroll lock",
    "num lock",
    "print screen",
    "pause",
    "f1",
    "f2",
    "f3",
    "f4",
    "f5",
    "f6",
    "f7",
    "f8",
    "f9",
    "f10",
    "f11",
    "f12",
    "f13",
    "f14",
    "f15",
    "f16",
    "f17",
    "f18",
    "f19",
    "f20",
    "f21",
    "f22",
    "f23",
    "f24",
    "f25",
    "kp 0",
    "kp 1",
    "kp 2",
    "kp 3",
    "kp 4",
    "kp 5",
    "kp 6",
    "kp 7",
    "kp 8",
    "kp 9",
    "kp decimal",
    "kp divide",
    "kp multiply",
    "kp subtract",
    "kp add",
    "kp enter",
    "kp equal",
    "left shift",
    "left control",
    "left alt",
    "left super",
    "right shift",
    "right control",
    "right alt",
    "right super",
    "menu",
    NULL
};
ENUM_CODES(KeyCodes) = {
    GLFW_KEY_UNKNOWN,
    GLFW_KEY_SPACE,
    GLFW_KEY_APOSTROPHE,
    GLFW_KEY_COMMA,
    GLFW_KEY_MINUS,
    GLFW_KEY_PERIOD,
    GLFW_KEY_SLASH,
    GLFW_KEY_0,
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3,
    GLFW_KEY_4,
    GLFW_KEY_5,
    GLFW_KEY_6,
    GLFW_KEY_7,
    GLFW_KEY_8,
    GLFW_KEY_9,
    GLFW_KEY_SEMICOLON,
    GLFW_KEY_EQUAL,
    GLFW_KEY_A,
    GLFW_KEY_B,
    GLFW_KEY_C,
    GLFW_KEY_D,
    GLFW_KEY_E,
    GLFW_KEY_F,
    GLFW_KEY_G,
    GLFW_KEY_H,
    GLFW_KEY_I,
    GLFW_KEY_J,
    GLFW_KEY_K,
    GLFW_KEY_L,
    GLFW_KEY_M,
    GLFW_KEY_N,
    GLFW_KEY_O,
    GLFW_KEY_P,
    GLFW_KEY_Q,
    GLFW_KEY_R,
    GLFW_KEY_S,
    GLFW_KEY_T,
    GLFW_KEY_U,
    GLFW_KEY_V,
    GLFW_KEY_W,
    GLFW_KEY_X,
    GLFW_KEY_Y,
    GLFW_KEY_Z,
    GLFW_KEY_LEFT_BRACKET,
    GLFW_KEY_BACKSLASH,
    GLFW_KEY_RIGHT_BRACKET,
    GLFW_KEY_GRAVE_ACCENT,
    GLFW_KEY_WORLD_1,
    GLFW_KEY_WORLD_2,
    GLFW_KEY_ESCAPE,
    GLFW_KEY_ENTER,
    GLFW_KEY_TAB,
    GLFW_KEY_BACKSPACE,
    GLFW_KEY_INSERT,
    GLFW_KEY_DELETE,
    GLFW_KEY_RIGHT,
    GLFW_KEY_LEFT,
    GLFW_KEY_DOWN,
    GLFW_KEY_UP,
    GLFW_KEY_PAGE_UP,
    GLFW_KEY_PAGE_DOWN,
    GLFW_KEY_HOME,
    GLFW_KEY_END,
    GLFW_KEY_CAPS_LOCK,
    GLFW_KEY_SCROLL_LOCK,
    GLFW_KEY_NUM_LOCK,
    GLFW_KEY_PRINT_SCREEN,
    GLFW_KEY_PAUSE,
    GLFW_KEY_F1,
    GLFW_KEY_F2,
    GLFW_KEY_F3,
    GLFW_KEY_F4,
    GLFW_KEY_F5,
    GLFW_KEY_F6,
    GLFW_KEY_F7,
    GLFW_KEY_F8,
    GLFW_KEY_F9,
    GLFW_KEY_F10,
    GLFW_KEY_F11,
    GLFW_KEY_F12,
    GLFW_KEY_F13,
    GLFW_KEY_F14,
    GLFW_KEY_F15,
    GLFW_KEY_F16,
    GLFW_KEY_F17,
    GLFW_KEY_F18,
    GLFW_KEY_F19,
    GLFW_KEY_F20,
    GLFW_KEY_F21,
    GLFW_KEY_F22,
    GLFW_KEY_F23,
    GLFW_KEY_F24,
    GLFW_KEY_F25,
    GLFW_KEY_KP_0,
    GLFW_KEY_KP_1,
    GLFW_KEY_KP_2,
    GLFW_KEY_KP_3,
    GLFW_KEY_KP_4,
    GLFW_KEY_KP_5,
    GLFW_KEY_KP_6,
    GLFW_KEY_KP_7,
    GLFW_KEY_KP_8,
    GLFW_KEY_KP_9,
    GLFW_KEY_KP_DECIMAL,
    GLFW_KEY_KP_DIVIDE,
    GLFW_KEY_KP_MULTIPLY,
    GLFW_KEY_KP_SUBTRACT,
    GLFW_KEY_KP_ADD,
    GLFW_KEY_KP_ENTER,
    GLFW_KEY_KP_EQUAL,
    GLFW_KEY_LEFT_SHIFT,
    GLFW_KEY_LEFT_CONTROL,
    GLFW_KEY_LEFT_ALT,
    GLFW_KEY_LEFT_SUPER,
    GLFW_KEY_RIGHT_SHIFT,
    GLFW_KEY_RIGHT_CONTROL,
    GLFW_KEY_RIGHT_ALT,
    GLFW_KEY_RIGHT_SUPER,
    GLFW_KEY_MENU,
};
ENUM_T(KeyEnum, KeyStrings, KeyCodes)
#define CheckKey(L, arg) enumCheck((L), (arg), &KeyEnum)
#define PushKey(L, code) enumPush((L), (code), &KeyEnum)

enum_t *enumKey(void)
    { return &KeyEnum; }


