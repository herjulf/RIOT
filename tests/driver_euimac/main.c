/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief       Test application for the i2c eui64 EEPROM
 *
 * @author      Robert Olsson <roolss@kth.se>
 *
 * @}
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "../../drivers/euimac/include/euimac_params.h"
//#include "euimac_params.h"
#include "euimac.h"

extern uint8_t eui64[8];

int main(void)
{
    euimac_t dev;
    int result;

    puts("EUIMAC test application\n");

    /* Init and read the EUI MAC addre */

    result = euimac_init(&dev, &euimac_params[0]);
    if (result != EUIMAC_OK) {
        puts("[Error] The given i2c is not enabled");
        return 1;
    }
    else {
      printf("EUI-64 MAC: %x-%x-%x-%x-%x-%x-%x-%x\n",
	    eui64[0], eui64[1], eui64[2], eui64[3], eui64[4], eui64[5], eui64[6], eui64[7]);
    }
}    
