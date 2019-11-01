/*
 * Copyright (C) 2018 Kaspar Schleiser <kaspar@schleiser.de>
 *               2016 RWTH Aachen, Josua Arndt
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_rss2-mega256rfr2
 * @{
 *
 * @file
 * @brief       Board specific LED initialization
 *
 * @author      Josua Arndt <jarndt@ias.rwth-aachen.de>
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Robert Olsson <roolss@kth.se>
 *
 * This is based on  boards_jiminy-mega256rfr2
 *
 * @}
 */

#include "board.h"
#include "cpu.h"

#include "net/eui64.h"
#include <string.h>
#include <inttypes.h>
#include "../../drivers/euimac/include/euimac_params.h"
#include "euimac.h"

extern eui64_t euimac;

void led_init(void)
{
    /* Ports Pins as Output */
    LED_PORT_DDR |= LED1_MASK | LED0_MASK;
    /* All LEDs OFF */
    LED_PORT |= (LED1_MASK | LED0_MASK);
}

void _euimac_init(void)
{
  euimac_t dev;
  int result;

  result = euimac_init(&dev, &euimac_params[0]);
  if (result != EUIMAC_OK) {
    puts("[Error] The given i2c is not enabled");
  }
}
