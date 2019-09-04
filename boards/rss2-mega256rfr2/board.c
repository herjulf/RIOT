/*
 * Copyright (C) 2018 Kaspar Schleiser <kaspar@schleiser.de>
 *               2016 RWTH Aachen, Josua Arndt
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_jiminy-mega256rfr2
 * @{
 *
 * @file
 * @brief       Board specific LED initialization
 *
 * @author      Josua Arndt <jarndt@ias.rwth-aachen.de>
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 *
 * @}
 */

#include "board.h"
#include "cpu.h"

void led_init(void)
{
    /* Ports Pins as Output */
    LED_PORT_DDR |= LED1_MASK | LED0_MASK;
    /* All LEDs OFF */
    LED_PORT |= (LED1_MASK | LED0_MASK);
}
