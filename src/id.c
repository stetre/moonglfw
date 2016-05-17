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


/****************************************************************************
 * Id reservation                                                           *
 ****************************************************************************/

#include "internal.h"

static id_t *id_search(int key);

static int key_new(void)
/* returns a free key or -1 if none are found (unlikely) */
    {
    static int keyfree = 0;
    int guard = keyfree-1;
    if((keyfree==-1)||(keyfree==0)||(keyfree==1))
        { keyfree=1; guard=-1; }
    while(1)
        {
        if(keyfree==guard) /* we made a complete round */
            return -1;
        if(keyfree==-1) keyfree=1;
        if(id_search(keyfree)==NULL) return keyfree++;
        keyfree++;
        }
    return -1;
    }

static int cmp(id_t *id1, id_t *id2) /* the compare function */
    { return (id1->key < id2->key ? -1 : id1->key > id2->key); } 

static RB_HEAD(idtree_s, id_s) Head = RB_INITIALIZER(&Head);

RB_PROTOTYPE_STATIC(idtree_s, id_s, entry, cmp) 
RB_GENERATE_STATIC(idtree_s, id_s, entry, cmp) 
 
static id_t *id_remove(id_t *id) 
    { return RB_REMOVE(idtree_s, &Head, id); }
static id_t *id_insert(id_t *id) 
    { return RB_INSERT(idtree_s, &Head, id); }
static id_t *id_search(int key) 
    { id_t tmp; tmp.key = key; return RB_FIND(idtree_s, &Head, &tmp); }
static id_t *id_first(int key) 
    { id_t tmp; tmp.key = key; return RB_NFIND(idtree_s, &Head, &tmp); }
#if 0
id_t *id_next(id_t *id)
    { return RB_NEXT(idtree_s, &Head, id); }
id_t *id_prev(id_t *id)
    { return RB_PREV(idtree_s, &Head, id); }
id_t *id_min(void)
    { return RB_MIN(idtree_s, &Head); }
id_t *id_max(void)
    { return RB_MAX(idtree_s, &Head); }
id_t *id_root(void)
    { return RB_ROOT(&Head); }
#endif


int id_new(void)
    {
    id_t *id;
    int key =  key_new(); /* allocate a free key */
    if(key == -1)
        return -1;
    if((id = (id_t*)malloc(sizeof(id_t))) == NULL) return -1;
    id->key = key;
    id_insert(id);
    return id->key;
    }

void id_free(int key)
    {
    id_t* id = id_search(key);
    if(id)
        {
        id_remove(id);
        free(id);
        }
    }

void id_free_all(void)
    {
    id_t *id;
    while((id = id_first(0)))
        {
        id_remove(id);
        free(id);
        }
    }

