/**************************************************************************
 * @file        dataTypeDefintion.c
 * @author      MDBU Software Team
 * @brief       define data type for application.
 * @note        Copyright (c) 2016 Texas Instruments Incorporated.
 *              All rights reserved.
 ******************************************************************************/

#ifndef TYPE_DEF_H_
#define TYPE_DEF_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef CHAR
#define CHAR   char
#endif

#ifndef SINT
#define SINT   int
#endif

#ifndef UINT
#define UINT   unsigned int
#endif

#ifndef SINT8
#define SINT8   signed char
#endif

#ifndef float_t
#define float_t   float
#endif

#ifndef UINT8
#define UINT8   unsigned char
#endif

#ifndef SINT16
#define SINT16  int
#endif

#ifndef INT16
#define INT16 int
#endif

#ifndef UINT16
#define UINT16  unsigned int
#endif

#ifndef SINT32
#define SINT32  long
#endif

#ifndef UINT32
#define UINT32  unsigned long
#endif

#ifndef UINT64
#define UINT64  unsigned double long
#endif

#ifndef BOOL
#define BOOL	bool
#endif

typedef unsigned char BYTE;
typedef unsigned long ULONG;
typedef unsigned int WORD;

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#define PRIVATE         static
#define VOLATILE        volatile
#define CONST           const

#define SIZE(array)      (sizeof(array) / sizeof(array[0]))

#define MAX(a,b)        ((a) > (b) ? (a) : (b))
#define MIN(a,b)        ((a) < (b) ? (a) : (b))

#define LEFTSHIFT(a, b)  ((a) << (b))
#define RIGHTSHIFT(a, b) ((a) >> (b))

#endif /*TYPE_DEF_H_*/
