/* This file stores macros shared accross HSVM and SVM */

#pragma warning(disable: 4005)

#undef lconfig_h
#define LUA_CORE
#include "../../../Source Dependencies/Lua/luaconf.h"

#define l_isfalse(o) (ttype(o) == R_LUA_TNIL || (ttype(o) == R_LUA_TBOOLEAN && bvalue(o) == 0))
#define dojump(pc, i) ((pc) += (i))

/* HSVM Dependent */
#define VMJmp	0x843735
#define VMCall	0x843730

//OpCode 48, re-entry.
#define VMLuaUOpCallJmp	0xBE0650
#define VMLuaUOpCallCall 0xBE064B

#define VMLuaUJmp	0xBDF66E
#define VMLuaUCall	0xBDF669

#define VMLuaUDispatcher 0xBDF669

/* Offsets */
#define OpenUpV 100

#define LuaHNew 0x840C70 //55 8B EC 56 57 8B 7D 08 6A 08 6A 24 6A 00 6A 00 57 E8 ? ? ? ?
#define LuaGetN 0x840AC0 //
#define VMBinTM 0x842C60 //
#define BarRF   0x83ECE0 //
#define GrowStk 0x83F5F0 //
#define LCStep  0x83F0A0 //
#define VMArith 0x8432D0 //
#define TypeErr 0x840620 //
#define RunErr  0x8405F0 //
#define LVConcat 0x842EA0 //
#define VEqualV 0x8435E0 //
#define VLessTh 0x846740 //
#define VLessEq 0x846680 //
#define PreCall 0x83F7B0 //
#define PreCallU 0xBE3690 //
#define PosCall 0x83F710 //
#define LDCall  0x83F200 //
#define LClose  0x83FF40 //
#define ReSzArr 0x840E00 //
#define VHSetNum 0x840EE0 //
#define FindUpv 0x840040 //
#define VGetNameCall 0x842B80
#define LSNilObject 0x1E22DA8
