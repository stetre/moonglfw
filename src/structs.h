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

/********************************************************************************
 * Structs definitions                                                          *
 ********************************************************************************/

#ifndef structsDEFINED
#define structsDEFINED

/* Types redefinitions (I know, I know... ) */
#define id_t   moonglfw_id_t
#define id_s   moonglfw_id_s
#define win_t  moonglfw_win_t
#define win_s  moonglfw_win_s
#define mon_t  moonglfw_mon_t
#define mon_s  moonglfw_mon_s
#define cur_t  moonglfw_cur_t
#define cur_s  moonglfw_cur_s

#define moonglfw_id_t struct moonglfw_id_s
struct moonglfw_id_s {
    RB_ENTRY(moonglfw_id_s) entry;
    int key;     /* search key */
};

#define moonglfw_win_t struct moonglfw_win_s
struct moonglfw_win_s {
    RB_ENTRY(moonglfw_win_s) entry;
    int id;         /* window id (search key) */
    GLFWwindow  *window; /* self */
    int share_id; /* share window id */
    /* references for callbacks on the Lua registry */
    int WindowPos;
    int WindowSize;
    int WindowClose;
    int WindowRefresh;
    int WindowFocus;
    int WindowIconify;
    int FramebufferSize;
    int Key;
    int Char;
    int CharMods;
    int MouseButton;
    int CursorPos;
    int CursorEnter;
    int Scroll;
    int Drop;
};

#define moonglfw_mon_t struct moonglfw_mon_s
struct moonglfw_mon_s {
    RB_ENTRY(moonglfw_mon_s) entry;
    int id;         /* monitor id (search key) */
    GLFWmonitor     *monitor; /* self */
    /* references for callbacks on the Lua registry */
    //int Monitor;
};

#define moonglfw_cur_t struct moonglfw_cur_s
struct moonglfw_cur_s {
    RB_ENTRY(moonglfw_cur_s) entry;
    int id;         /* cursor id (search key) */
    GLFWcursor  *cursor; /* self */
};

#endif /* structsDEFINED */
