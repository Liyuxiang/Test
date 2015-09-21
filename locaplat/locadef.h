/*
* Copyright (c) 2014,֣������ӿƼ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�locadef.h
* �ļ���ʶ�����峣����
* ժ    Ҫ�����峣����
* 
* ��ǰ�汾��0.1
* ��    �ߣ���ѩ��
* ������ڣ�2014.03.18
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
	LOCA_FAULT,		//ִ�й����з��ʹ���
	LOCA_ERROR		//�����������
}loca_status;

#define TURE	1
#define FALSE	0


#ifdef __cplusplus
}
#endif

#endif		//#ifndef _LOCADEF_H_
