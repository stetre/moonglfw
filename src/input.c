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

/*------------------------------------------------------------------------------*
 | Input mode                                                                   |
 *------------------------------------------------------------------------------*/

static int GetInputMode(lua_State *L)
    {
    int val;
    win_t *win = checkwindow(L, 1);
    int mode = checkinputmode(L, 2);
    val = glfw.GetInputMode(win->window,mode);
    switch(mode)
        {
        case GLFW_CURSOR:   pushcursormode(L, val); return 1;
        case GLFW_STICKY_KEYS:
        case GLFW_STICKY_MOUSE_BUTTONS:
        case GLFW_LOCK_KEY_MODS:
        case GLFW_RAW_MOUSE_MOTION:
                            lua_pushboolean(L, val); return 1;
        default:
            return unexpected(L);
        }
    return 0;
    }

static int SetInputMode(lua_State *L)
    {
    int value;
    win_t *win = checkwindow(L, 1);
    int mode = checkinputmode(L, 2);
    switch(mode)
        {
        case GLFW_CURSOR:   value = checkcursormode(L, 3); break;
        case GLFW_STICKY_KEYS:
        case GLFW_STICKY_MOUSE_BUTTONS:
        case GLFW_LOCK_KEY_MODS:
        case GLFW_RAW_MOUSE_MOTION:
                            value = checkboolean(L, 3); break;
        default:
            return unexpected(L);
        }
    glfw.SetInputMode(win->window, mode, value);
    return 0;
    }

static int RawMouseMotionSupported(lua_State *L)
    {
    CheckPfn(L, RawMouseMotionSupported, 3, 3, 0);
    lua_pushboolean(L, glfw.RawMouseMotionSupported());
    return 1;
    }

/*------------------------------------------------------------------------------*
 | Keyboard                                                                     |
 *------------------------------------------------------------------------------*/

static int GetKey(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int key = checkkey(L, 2);
    int state = glfw.GetKey(win->window, key);
    lua_pushstring(L, state==GLFW_PRESS ? "press" : "release");
    return 1;
    }

static int GetKeyName(lua_State *L)
    {
    int key, scancode;
    const char *name = NULL;
    CheckPfn(L, GetKeyName, 3, 2, 0);
    if(lua_type(L, 1) == LUA_TSTRING)
        {
        key = checkkey(L, 1);
        name = glfw.GetKeyName(key, 0);
        }
    else if(lua_isinteger(L, 1))
        {
        scancode = lua_tointeger(L, 1);
        name = glfw.GetKeyName(GLFW_KEY_UNKNOWN, scancode);
        }
    else
        return luaL_argerror(L, 1, "expected key (a string) or scancode (an integer)");

    if(!name)
        return 0;
    lua_pushstring(L, name);
    return 1;
    }

static int GetKeyScancode(lua_State *L)
    {
    int key = checkkey(L, 1);
    CheckPfn(L, GetKeyScancode, 3, 3, 0);
    lua_pushinteger(L, glfw.GetKeyScancode(key));
    return 1;
    }

/*------------------------------------------------------------------------------*
 | Mouse                                                                        |
 *------------------------------------------------------------------------------*/

static int GetMouseButton(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int button = checkbutton(L, 2);
    int state = glfw.GetMouseButton(win->window, button);
    lua_pushstring(L, state==GLFW_PRESS ? "press" : "release");
    return 1;
    }

static int GetCursorPos(lua_State *L)
    {
    double xpos, ypos;
    win_t *win = checkwindow(L, 1);
    glfw.GetCursorPos(win->window, &xpos, &ypos);
    lua_pushnumber(L, xpos);
    lua_pushnumber(L, ypos);
    return 2;
    }

static int SetCursorPos(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    double xpos = luaL_checknumber(L, 2);
    double ypos = luaL_checknumber(L, 3);
    glfw.SetCursorPos(win->window, xpos, ypos);
    return 0;
    }

static int CreateCursor(lua_State *L)
/* create_cursor(image_width, image_height, image_pixels, xhot, yhot)
 * -> cursor handle
 */
    {
    size_t len;
    int xhot, yhot;
    GLFWimage image;
    cur_t *cur;
    image.width = luaL_checkinteger(L, 1);
    image.height = luaL_checkinteger(L, 2);
    image.pixels = (unsigned char*)luaL_checklstring(L, 3, &len); /* yeah, it's const... */
    xhot = luaL_checkinteger(L, 4);
    yhot = luaL_checkinteger(L, 5);

    cur = cur_new();
    if(!cur)
        return luaL_error(L, "cannot create cursor");
    
    cur->cursor = glfw.CreateCursor(&image, xhot, yhot);
    if(!cur->cursor)
        {
        cur_free(cur);
        return luaL_error(L, "cannot create cursor");
        }
    lua_pushinteger(L, cur->id);
    return 1;
    }

static int CreateStandardCursor(lua_State *L)
    {
    int shape = checkcursorshape(L, 1);
    cur_t *cur = cur_new();
    if(!cur)
        return luaL_error(L, "cannot create cursor");
    
    cur->cursor = glfw.CreateStandardCursor(shape);
    if(!cur->cursor)
        {
        cur_free(cur);
        return luaL_error(L, "cannot create cursor");
        }
    lua_pushinteger(L, cur->id);
    return 1;
    }

int input_destroy_cursor(cur_t *cur)
    {
    glfw.DestroyCursor(cur->cursor);
    cur_free(cur);
    return 0;
    }

static int DestroyCursor(lua_State *L)
    {
    cur_t *cur = checkcursor(L, 1);
    input_destroy_cursor(cur);
    return 0;
    }

static int SetCursor(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    cur_t *cur = checkcursor(L, 2);
    glfw.SetCursor(win->window, cur->cursor);
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Joystick                                                                     |
 *------------------------------------------------------------------------------*/


static int JoystickPresent(lua_State *L)
    {
    int jid = CheckJoystick(L, 1);
    int val = glfw.JoystickPresent(jid);
    lua_pushboolean(L, val);
    return 1;
    }

static int GetJoystickAxes(lua_State *L)
    {
    int i, count;
    int jid = CheckJoystick(L, 1);
    const float *axis = glfw.GetJoystickAxes(jid, &count);
    if(!axis)
        return 0;
    luaL_checkstack(L, count, NULL);
    for(i=0; i< count; i++)
        lua_pushnumber(L, axis[i]);
    return count;
    }

static int GetJoystickButtons(lua_State *L)
    {
    int i, count;
    int jid = CheckJoystick(L, 1);
    const unsigned char *state = glfw.GetJoystickButtons(jid, &count);
    if(!state)
        return 0;
    luaL_checkstack(L, count, NULL);
    for(i=0; i< count; i++)
        lua_pushstring(L, state[i]==GLFW_PRESS ? "press" : "release");
    return count;
    }

static int GetJoystickHats(lua_State *L)
    {
    int i, count;
    int jid = CheckJoystick(L, 1);
    CheckPfn(L, GetJoystickHats, 3, 3, 0);
    const unsigned char *hat = glfw.GetJoystickHats(jid, &count);
    if(!hat)
        return 0;
    luaL_checkstack(L, count, NULL);
    for(i=0; i< count; i++)
        pushhat(L, hat[i]);
    return count;
    }

static int GetJoystickName(lua_State *L)
    {
    int jid = CheckJoystick(L, 1);
    const char *name = glfw.GetJoystickName(jid);
    lua_pushstring(L, name ? name : "???");
    return 1;
    }

static int GetJoystickGUID(lua_State *L)
    {
    int jid = CheckJoystick(L, 1);
    const char *guid;
    CheckPfn(L, GetJoystickGUID, 3, 3, 0);
    guid = glfw.GetJoystickGUID(jid);
    if(!guid) return 0;
    lua_pushstring(L, guid);
    return 1;
    }

static int JoystickIsGamepad(lua_State *L)
    {
    int jid = CheckJoystick(L, 1);
    CheckPfn(L, JoystickIsGamepad, 3, 3, 0);
    lua_pushboolean(L, glfw.JoystickIsGamepad(jid));
    return 1;
    }

static int UpdateGamepadMappings(lua_State *L)
    {
    int rc;
    const char *s = luaL_checkstring(L, 1);
    CheckPfn(L, UpdateGamepadMappings, 3, 3, 0);
    rc = glfw.UpdateGamepadMappings(s);
    lua_pushboolean(L, rc);
    return 1;
    }

static int GetGamepadName(lua_State *L)
    {
    const char* name;
    int jid = CheckJoystick(L, 1);
    CheckPfn(L, GetGamepadName, 3, 3, 0);
    name = glfw.GetGamepadName(jid);
    if(!name) return 0;
    lua_pushstring(L, name);
    return 1;
    }

static int GetGamepadState(lua_State *L)
    {
    GLFWgamepadstate state;
    int rc;
    int jid = CheckJoystick(L, 1);
    CheckPfn(L, GetGamepadState, 3, 3, 0);
    rc = glfw.GetGamepadState(jid, &state);
    if(!rc) return 0;
    pushgamepadbuttons(L, &state);
    pushgamepadaxes(L, &state);
    return 2;
    }

//@@NO: void glfwSetJoystickUserPointer(int jid, void* pointer);
//@@NO: void* glfwGetJoystickUserPointer(int jid);

/*------------------------------------------------------------------------------*
 | Clipboard                                                                    |
 *------------------------------------------------------------------------------*/

static int SetClipboardString(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    const char* string = luaL_checkstring(L, 2);
    glfw.SetClipboardString(win->window, string);
    return 0;
    }

static int GetClipboardString(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    const char* string = glfw.GetClipboardString(win->window);
    lua_pushstring(L, string);
    return 1;
    }

/*------------------------------------------------------------------------------*
 | Time                                                                         |
 *------------------------------------------------------------------------------*/


static int GetTime(lua_State *L)
    {
    lua_pushnumber(L, glfw.GetTime());
    return 1;
    }

static int SetTime(lua_State *L)
    {
    double time = luaL_checknumber(L, 1);
    glfw.SetTime(time);
    return 0;
    }

static int GetTimerFrequency(lua_State *L)
    {
    uint64_t hz;
    CheckPfn(L, GetTimerFrequency, 3, 2, 0);
    hz = glfw.GetTimerFrequency();
    lua_pushinteger(L, hz);
    return 1;
    }

static int GetTimerValue(lua_State *L)
    {
    uint64_t tics;
    CheckPfn(L, GetTimerValue, 3, 2, 0);
    tics = glfw.GetTimerValue();
    lua_pushinteger(L, tics);
    return 1;
    }

static int GetTimerSeconds(lua_State *L) /* NONGLFW */
    {
    double seconds;
    CheckPfn(L, GetTimerValue, 3, 2, 0);
    seconds = (double)glfw.GetTimerValue()/(double)glfw.GetTimerFrequency();
    lua_pushnumber(L, seconds);
    return 1;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "get_input_mode", GetInputMode },
        { "set_input_mode", SetInputMode },
        { "raw_mouse_motion_supported", RawMouseMotionSupported },
        { "get_key", GetKey },
        { "get_key_name", GetKeyName },
        { "get_key_scancode", GetKeyScancode },
        { "get_mouse_button", GetMouseButton },
        { "get_cursor_pos", GetCursorPos },
        { "set_cursor_pos", SetCursorPos },
        { "create_cursor", CreateCursor },
        { "create_standard_cursor", CreateStandardCursor },
        { "destroy_cursor", DestroyCursor },
        { "set_cursor", SetCursor },
        { "joystick_present", JoystickPresent },
        { "get_joystick_axes", GetJoystickAxes },
        { "get_joystick_buttons", GetJoystickButtons },
        { "get_joystick_hats", GetJoystickHats },
        { "get_joystick_name", GetJoystickName },
        { "get_joystick_guid", GetJoystickGUID },
        { "joystick_is_gamepad", JoystickIsGamepad },
        { "update_gamepad_mappings", UpdateGamepadMappings },
        { "get_gamepad_name", GetGamepadName },
        { "get_gamepad_state", GetGamepadState },
        { "set_clipboard_string", SetClipboardString },
        { "get_clipboard_string", GetClipboardString },
        { "get_time", GetTime },
        { "set_time", SetTime },
        { "get_timer_frequency", GetTimerFrequency },
        { "get_timer_seconds", GetTimerSeconds },
        { "get_timer_value", GetTimerValue },
        { NULL, NULL } /* sentinel */
    };

void moonglfw_open_input(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

