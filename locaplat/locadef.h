/*
* Copyright (c) 2014,郑州联睿电子科技有限公司
* All rights reserved.
* 
* 文件名称：locadef.h
* 文件标识：定义常用项
* 摘    要：定义常用项
* 
* 当前版本：0.1
* 作    者：何雪锋
* 完成日期：2014.03.18
*/

#ifndef _LOCADEF_H_
#define _LOCADEF_H_

#ifdef __cplusplus
extern	"C"	{
#endif

#include "stdint.h"

#ifndef int8s
typedef	int8_t	int8s;
#endif

#ifndef int16s
typedef	int16_t	int16s;
#endif

#ifndef	int32s
typedef	int32_t	int32s;
#endif

#ifndef int64s
typedef	int64_t	int64s;
#endif

#ifndef int8u
typedef	uint8_t	int8u;
#endif

#ifndef int16u
typedef	uint16_t	int16u;
#endif

#ifndef int32u
typedef	uint32_t	int32u;
#endif

#ifndef int64u
typedef	uint64_t	int64u;
#endif

typedef	enum
{
	LOCA_OK = 0,
	LOCA_FAULT,		//执行过程中发送错误
	LOCA_ERROR		//输入参数有误
}loca_status;

#define TURE	1
#define FALSE	0


#ifdef __cplusplus
}
#endif

#endif		//#ifndef _LOCADEF_H_
