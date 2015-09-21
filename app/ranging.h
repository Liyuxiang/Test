
#ifndef _RANGING_H_
#define _RANGING_H_
#include "locadef.h"
#include "deca_device_api.h"

typedef enum tagdevmod {LOCA_TAG,LOCA_ANCHOR,LOCA_LISTENER}devmode;

typedef struct tagmsg
{
	int8u rxinfo[4];
	int8u *mesptr;
	int8u rxqinfo[8];
	int8u rxtrckinterval[4];
	int8u rxtrckoffset[5];
	int8u rxtimestamp[14];
	int32u buflength;
}RecMsg,*pRecMsg;

typedef struct tagdwtconf
{
	devmode mode;
	dwt_config_t config;
}dwtconf,*pdwtconf;

loca_status dw1000_init(void);
void dw1000_config(const pdwtconf config_t);

#endif
