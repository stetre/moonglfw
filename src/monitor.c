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

int monitorAdd(lua_State *L, GLFWmonitor *monitor)
    {
    mon_t *mon = mon_first(0);
    while(mon)
        {
        if(mon->monitor == monitor) /* already in */
            return mon->id;
        mon = mon_next(mon);
        }
    /* not found */
    mon = mon_new();
    if(!mon)
        return luaL_error(L, "cannot create monitor entry");
    mon->monitor = monitor;
    return mon->id;
    }

#if 0
static void Delete(GLFWmonitor *monitor)
    {
    mon_t *mon = mon_first(0);
    while(mon)
        {
        if(mon->monitor == monitor) /* already in */
            { mon_free(mon); return; }
        mon = mon_next(mon);
        }
    }
#endif

void monitorCallback(GLFWmonitor *monitor, int event)
    {
    (void)monitor;
    (void)event;
#if 0
    switch(event)
        {
        case GLFW_CONNECTED: /* ignore */ return;
        case GLFW_DISCONNECTED: Delete(monitor); return;
        }
#endif
    }

static int GetPrimaryMonitor(lua_State *L)
    {
    int id;
    GLFWmonitor* monitor = glfw.GetPrimaryMonitor();
    if(!monitor)
        return 0;
    id = monitorAdd(L, monitor);
    lua_pushinteger(L, id);
    return 1;
    }

static int GetMonitors(lua_State *L)
    {
    int i, id, count;
    GLFWmonitor** monitor;
    /* no mon_free_all(); */
    monitor = glfw.GetMonitors(&count);
    if(!monitor)
        return 0;
    for(i=0; i<count; i++)
        {
        id = monitorAdd(L, monitor[i]);
        lua_pushinteger(L, id);
        }
    return count;
    }


static int GetMonitorPos(lua_State *L)
    {
    int xpos, ypos;
    mon_t *mon = checkmonitor(L, 1);
    glfw.GetMonitorPos(mon->monitor, &xpos, &ypos);
    lua_pushinteger(L, xpos);
    lua_pushinteger(L, ypos);
    return 2;
    }

static int GetMonitorPhysicalSize(lua_State *L)
    {
    int widthmm, heightmm;
    mon_t *mon = checkmonitor(L, 1);
    glfw.GetMonitorPhysicalSize(mon->monitor, &widthmm, &heightmm);
    lua_pushinteger(L, widthmm);
    lua_pushinteger(L, heightmm);
    return 2;
    }

static int GetMonitorName(lua_State *L)
    {
    mon_t *mon = checkmonitor(L, 1);
    const char* name = glfw.GetMonitorName(mon->monitor);
    lua_pushstring(L, name ? name : "???");
    return 1;
    }

static int GetVideoModes(lua_State *L)
    {
    int i, count;
    mon_t *mon = checkmonitor(L, 1);
    const GLFWvidmode *mode = glfw.GetVideoModes(mon->monitor, &count);
    if(!mode) 
        return 0;
    for(i=0; i<count; i++)
        pushvidmode(L, &mode[i]);
    return count;
    }

static int GetVideoMode(lua_State *L)
    {
    mon_t *mon = checkmonitor(L, 1);
    const GLFWvidmode *mode = glfw.GetVideoMode(mon->monitor);
    if(!mode) 
        return 0;
    pushvidmode(L, mode);
    return 1;
    }

static int SetGamma(lua_State *L)
    {
    mon_t *mon = checkmonitor(L, 1);
    float gamma = luaL_checknumber(L, 2);
    glfw.SetGamma(mon->monitor, gamma);
    return 0;
    }

static int GetGammaRamp(lua_State *L)
    {
    mon_t *mon = checkmonitor(L, 1);
    const GLFWgammaramp *ramp = glfw.GetGammaRamp(mon->monitor);
    if(!ramp) 
        return 0;
    return pushgammaramp(L, ramp);
    }

static int SetGammaRamp(lua_State *L)
    {
    GLFWgammaramp ramp;
    mon_t *mon = checkmonitor(L, 1);
    checkgammaramp(L, 2, &ramp);
    glfw.SetGammaRamp(mon->monitor, &ramp);
    free(ramp.red);
    free(ramp.green);
    free(ramp.blue);
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "get_primary_monitor", GetPrimaryMonitor },
        { "get_monitors", GetMonitors },
        { "get_monitor_pos", GetMonitorPos },
        { "get_monitor_physical_size", GetMonitorPhysicalSize },
        { "get_monitor_name", GetMonitorName },
        { "get_video_modes", GetVideoModes },
        { "get_video_mode", GetVideoMode },
        { "set_gamma", SetGamma },
        { "get_gamma_ramp", GetGammaRamp },
        { "set_gamma_ramp", SetGammaRamp },
        { NULL, NULL } /* sentinel */
    };

void moonglfw_open_monitor(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    glfw.SetMonitorCallback(monitorCallback);
    }

