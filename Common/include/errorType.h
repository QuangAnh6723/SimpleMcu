/*
 * errorType.h
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#ifndef INCLUDE_ERRORTYPE_H_
#define INCLUDE_ERRORTYPE_H_

#include "stdint.h"

typedef enum __attribute__((packed)) {
	MY_OK  		=  0,
	MY_ERR		= -1,
	MY_ERR_CRC	= -2,
} myError_t;



#endif /* INCLUDE_ERRORTYPE_H_ */
