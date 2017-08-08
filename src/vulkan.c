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

/* NOTE: we cannot use lightuserdata to pass Vulkan handles between C and Lua,
 *       because handles are not (in general) pointers.
 *       In particular, handles of non-dispatchable objects are defined as uint64_t
 *       also on 32-bit platforms, so they do not fit into a void*.
 *       So we pass them as lua_Integer, which is hopefully large enough.
 */

static uintptr_t checkhandle(lua_State *L, int arg)
    {
    return (uintptr_t)luaL_checkinteger(L, arg);
    }

#if 0
static uint64_t opthandle(lua_State *L, int arg)
    {
    if(lua_isnoneornil(L, arg)) return 0;
    return checkhandle(L, arg);
    }
#endif

static int pushhandle(lua_State *L, uint64_t handle)
    {
    lua_pushinteger(L, handle);
    return 1;
    }


static const char* ResultString(VkResult rc)
    {
    switch(rc)
        {
        case VK_SUCCESS: return "success";
        case VK_NOT_READY: return "not ready";
        case VK_TIMEOUT: return "timeout";
        case VK_EVENT_SET: return "event set";
        case VK_EVENT_RESET: return "event reset";
        case VK_INCOMPLETE: return "incomplete";
        case VK_ERROR_OUT_OF_HOST_MEMORY: return "out of host memory";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "out of device memory";
        case VK_ERROR_INITIALIZATION_FAILED: return "initialization failed";
        case VK_ERROR_DEVICE_LOST: return "device lost";
        case VK_ERROR_MEMORY_MAP_FAILED: return "memory map failed";
        case VK_ERROR_LAYER_NOT_PRESENT: return "layer not present";
        case VK_ERROR_EXTENSION_NOT_PRESENT: return "extension not present";
        case VK_ERROR_FEATURE_NOT_PRESENT: return "feature not present";
        case VK_ERROR_INCOMPATIBLE_DRIVER: return "incompatible driver";
        case VK_ERROR_TOO_MANY_OBJECTS: return "too many objects";
        case VK_ERROR_FORMAT_NOT_SUPPORTED: return "format not supported";
        case VK_ERROR_FRAGMENTED_POOL: return "fragmented pool";
        case VK_ERROR_SURFACE_LOST_KHR: return "surface lost khr";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "native window in use khr";
        case VK_SUBOPTIMAL_KHR: return "suboptimal khr";
        case VK_ERROR_OUT_OF_DATE_KHR: return "out of date khr";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "incompatible display khr";
        case VK_ERROR_VALIDATION_FAILED_EXT: return "validation failed ext";
        case VK_ERROR_INVALID_SHADER_NV: return "invalid shader nv";
        case VK_ERROR_OUT_OF_POOL_MEMORY_KHR: return "out of pool memory";

        default:
            return "unknown vulkan error code";
        }
    return NULL; /* unreachable */
    }

    
static int VulkanSupported(lua_State *L)
    {
    if(glfw.VulkanSupported)
        lua_pushboolean(L, glfw.VulkanSupported());
    else
        lua_pushboolean(L, 0);
    return 1;
    }


static int GetRequiredInstanceExtensions(lua_State *L)
    {
    uint32_t count, i;
    const char **names;
    CheckVulkanPfn(L, GetRequiredInstanceExtensions);
    names = glfw.GetRequiredInstanceExtensions(&count);
    lua_newtable(L);
    if((count == 0) || names==NULL)
        return 1;
    for(i=0; i< count; i++)
        {
        lua_pushstring(L, names[i]);
        lua_rawseti(L, -2, i+1);
        }
    return 1;
    }


static int GetPhysicalDevicePresentationSupport(lua_State *L)
    {
    int result;
    VkInstance instance = (VkInstance)checkhandle(L, 1);
    VkPhysicalDevice device = (VkPhysicalDevice)checkhandle(L, 2);
    uint32_t queuefamily = luaL_checkinteger(L, 3);
    CheckVulkanPfn(L, GetPhysicalDevicePresentationSupport);
    result = glfw.GetPhysicalDevicePresentationSupport(instance, device, queuefamily);
    lua_pushboolean(L, result);
    return 1;
    }


static int CreateWindowSurface(lua_State *L)
    {
    VkResult ec;
    VkSurfaceKHR surface;
    win_t *win = checkwindow(L, 1);
    VkInstance instance = (VkInstance)checkhandle(L, 2);
    VkAllocationCallbacks* allocator = (VkAllocationCallbacks*)optlightuserdata(L, 3);

    CheckVulkanPfn(L, CreateWindowSurface);
    ec = glfw.CreateWindowSurface(instance, win->window, allocator, &surface);

    if(ec != VK_SUCCESS)
        return luaL_error(L, ResultString(ec));
    pushhandle(L, (uint64_t)surface);
    return 1;
    }

#if 0
static int DestroySurface(lua_State *L)
    {
    VkInstance instance = (VkInstance)checkhandle(L, 1);
    VkSurfaceKHR surface = (VkSurfaceKHR)checkhandle(L, 2);
    VkAllocationCallbacks* allocator = (VkAllocationCallbacks*)optlightuserdata(L, 3);
    vkDestroySurfaceKHR(instance, surface, allocator);
    return 0;
    }
#endif

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "vulkan_supported", VulkanSupported },
        { "get_required_instance_extensions", GetRequiredInstanceExtensions },
        { "get_physical_device_presentation_support", GetPhysicalDevicePresentationSupport },
        { "create_window_surface", CreateWindowSurface },
#if 0
        { "destroy_surface", DestroySurface },
#endif
        { NULL, NULL } /* sentinel */
    };

void moonglfw_open_vulkan(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    if(glfw.VulkanSupported && (sizeof(lua_Integer) < sizeof(uint64_t)))
        luaL_error(L, "Vulkan support requires sizeof(lua_Integer) >= sizeof(uint64_t)");
    }

