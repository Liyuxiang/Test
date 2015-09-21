
#ifndef _BSP_H_
#define _BSP_H_

#include "locadef.h"

#ifdef __cplusplus
extern "C" {
#endif


	
void BSP_Init(void);
	
void BSP_DWTSpiSet(int16u speed);
	
#ifdef __cplusplus
}
#endif
	
#endif
