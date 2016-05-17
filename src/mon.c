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

static int cmp(mon_t *mon1, mon_t *mon2) /* the compare function */
    { return (mon1->id < mon2->id ? -1 : mon1->id > mon2->id); } 

static RB_HEAD(montree_s, mon_s) Head = RB_INITIALIZER(&Head);

RB_PROTOTYPE_STATIC(montree_s, mon_s, entry, cmp) 
RB_GENERATE_STATIC(montree_s, mon_s, entry, cmp) 
 
static mon_t *mon_remove(mon_t *mon) 
    { return RB_REMOVE(montree_s, &Head, mon); }
static mon_t *mon_insert(mon_t *mon) 
    { return RB_INSERT(montree_s, &Head, mon); }
mon_t *mon_search(int id) 
    { mon_t tmp; tmp.id = id; return RB_FIND(montree_s, &Head, &tmp); }
mon_t *mon_first(int id) 
    { mon_t tmp; tmp.id = id; return RB_NFIND(montree_s, &Head, &tmp); }
mon_t *mon_next(mon_t *mon)
    { return RB_NEXT(montree_s, &Head, mon); }
#if 0
mon_t *mon_prev(mon_t *mon)
    { return RB_PREV(montree_s, &Head, mon); }
mon_t *mon_min(void)
    { return RB_MIN(montree_s, &Head); }
mon_t *mon_max(void)
    { return RB_MAX(montree_s, &Head); }
mon_t *mon_root(void)
    { return RB_ROOT(&Head); }
#endif
mon_t *mon_new(void)
    {
    mon_t *mon;
    int id = id_new();
    if(id == -1)
        return NULL;
    if((mon = (mon_t*)malloc(sizeof(mon_t))) == NULL) 
        { id_free(id); return NULL; }
    memset(mon, 0, sizeof(mon_t));
    mon->id = id;
    if(mon_search(mon->id))
        { free(mon); id_free(id); return NULL; }
    //mon->Monitor = LUA_NOREF;
    mon_insert(mon);
    return mon;
    }

void mon_free(mon_t* mon)
    {
    if(mon_search(mon->id) == mon)
        {
        /* release callback references */
        //CALLBACK_UNREF(mon->Monitor);
        mon_remove(mon);
        }
    id_free(mon->id);
    free(mon);
    }

void mon_free_all(void)
    {
    mon_t *mon;
    while((mon = mon_first(0)))
        mon_free(mon);
    }

mon_t* mon_check(lua_State *L, int arg)
    {
    int id = luaL_checkinteger(L, arg);
    mon_t *mon = mon_search(id);
    if(!mon)
        luaL_error(L, "invalid monitor id");
    return mon;
    }

