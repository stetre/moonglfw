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

#ifndef enumDEFINED
#define enumDEFINED

/*--------------------------------------------------------------------------*
 | int <-> string mappings                                                  |
 *--------------------------------------------------------------------------*/

#define enum_t moonglfw_enum_t
typedef struct {
    const char **strings;
    int *codes;
    size_t n;
} enum_t;

#define enumCheck moonglfw_enumCheck
int enumCheck(lua_State *L, int arg, enum_t *e);
#define enumPush moonglfw_enumPush
int enumPush(lua_State *L, int code, enum_t *e);
#define enumToString moonglfw_enumToString
const char* enumToString(int code, enum_t *e);
#define enumAssert moonglfw_enumAssert
int enumAssert(lua_State *L, enum_t *e, int code, const char *string);
#define enumOrUint moonglfw_enumOrUint
int enumOrUint(lua_State *L, int arg, int *dst, enum_t *e, int def);

#define ENUM_STRINGS(strings)   static const char* (strings) []
#define ENUM_CODES(codes)       static int (codes) []
#define ENUM_T(name, strings, codes)    \
    static enum_t name = { (strings), (codes), sizeof((codes))/sizeof(int) };

#define enumButton moonglfw_enumButton
enum_t *enumButton(void);
#define enumAction moonglfw_enumAction
enum_t *enumAction(void);
#define enumKey moonglfw_enumKey
enum_t *enumKey(void);

#if 0
#define enumZzz moonglfw_enumZzz
enum_t *enumZzz(void);
enum_t *enumZzz(void)
    { return &ZzzEnum; }


#define  moonglfw_
#endif

#if 0 /* @@ 11yy */
ENUM_STRINGS(ZzzStrings) = {
    "",
    NULL
};
ENUM_CODES(ZzzCodes) = {
    GLFW_,
};
ENUM_T(ZzzEnum, ZzzStrings, ZzzCodes)
#define CheckZzz(L, arg) enumCheck((L), (arg), &ZzzEnum)
#define PushZzz(L, code) enumPush((L), (code), &ZzzEnum)

#define enumZzz moonglfw_enumZzz
enum_t *enumZzz(void);
enum_t *enumZzz(void)
    { return &ZzzEnum; }


#define ToStringZzz(code) enumToString((code), &ZzzEnum)

#define CheckZzzOrNnn(L, arg, dst) enumOrUint((L), (arg), (dst), &ZzzEnum, 0)

enum_t *enumZzz(void)
    { return &ZzzEnum; }

#define ZzzEnum yyyZzz()
#define CheckZzz(L, arg) enumCheck((L), (arg), ZzzEnum)
#define PushZzz(L, code) enumPush((L), (code), ZzzEnum)
#define ToStringZzz(code) enumToString((code), ZzzEnum)

#endif

#endif /* enumDEFINED */
