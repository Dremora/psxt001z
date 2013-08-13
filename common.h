#ifndef __DEFS_H__
#define __DEFS_H__

typedef __int8  s8;
typedef __int16 s16;
typedef __int32 s32;
typedef __int64 s64;
typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "main.h"
#include "functions.h"
#include "libcrypt.h"
#include "derus.h"
#include "sha1.h"
#include "md5.h"

#define WIN32_LEAN_AND_MEAN
#define ZERO 0
#define VERSION "v0.20 beta 13 derus fix"
#define btoi(b)		((b)/16*10 + (b)%16)		/* BCD to u_char */
#define itob(i)		((i)/10*16 + (i)%10)		/* u_char to BCD */

#endif