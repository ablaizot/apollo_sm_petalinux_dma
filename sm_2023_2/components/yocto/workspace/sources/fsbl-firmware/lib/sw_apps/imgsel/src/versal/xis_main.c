/******************************************************************************
* Copyright (c) 2022 Xilinx, Inc. All rights reserved.
* Copyright (c) 2022-2023, Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
*
* @file xis_main.c
*
* This is the main file which contains code for Versal Image Selector.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00  skd  01/13/23 Initial release
*
* </pre>
*
* @note
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "xis_config.h"
#include "xis_i2c.h"
#include "xis_pm.h"
#include "xis_proc.h"
#include "xis_loader.h"
#include "xis_module.h"
#include "xplmi.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
static int XPlm_Init(void);

/************************** Variable Definitions *****************************/

/*****************************************************************************/
/**
 * @brief This is PLM main function
 *
 * @return	Ideally should not return, in case if it reaches end,
 *		error is returned
 *
 *****************************************************************************/
int main(void)
{
	int Status = XST_FAILURE;

	Status = XPlm_Init();
	if (Status != XST_SUCCESS)
	{
		goto END;
	}

	Status = XPlm_ModuleInit();
	if (Status != XST_SUCCESS) {
		goto END;
	}

	Status = XPlm_ProcessPmcCdo();
	if (Status != XST_SUCCESS)
	{
		goto END;
	}

	Status = XPlm_LoadBootPdi();
	if (Status != XST_SUCCESS) {
		goto END;
	}

	Status = XIs_ImageSelBoardParam();

END:
	/* Control should never reach here */
	return Status;
}

/*****************************************************************************/
/**
 * @brief This function initializes DMA, Run Time Config area, the processor
 * 		and task list structures.
 *
 * @return	Status as defined in xplmi_status.h
 *
 *****************************************************************************/
static int XPlm_Init(void)
{
	int Status = XST_FAILURE;

	/**
	 * Reset the wakeup signal set by ROM
	 * Otherwise MB will always wakeup, irrespective of the sleep state
	 */
	XPlmi_PpuWakeUpDis();

	XPlmi_InitDebugLogBuffer();

	Status = XPlm_InitProc();
	if (Status != XST_SUCCESS) {
		goto END;
	}

	/* Initializes the DMA pointers */
	Status = XPlmi_DmaInit();
	if (Status != XST_SUCCESS) {
		goto END;
	}

	Status = XPlmi_RunTimeConfigInit();

END:
	return Status;
}
