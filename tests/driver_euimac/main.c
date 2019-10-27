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
#include "net/eui64.h"
//#include "euimac_params.h"
#include "euimac.h"


extern eui64_t euimac;

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
      printf("EUIMAC-64: %x-%x-%x-%x-%x-%x-%x-%x\n",
	    euimac.uint8[0], euimac.uint8[1], euimac.uint8[2], euimac.uint8[3], euimac.uint8[4],
	     euimac.uint8[5], euimac.uint8[6], euimac.uint8[7]);
    }
}    
