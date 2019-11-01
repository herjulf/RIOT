/*
 * Copyright (C) 2016 RWTH Aachen, Josua Arndt
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
 * @brief       Board specific definitions for the rss2 256rfr2 board.
 *
 * @author      Josua Arndt <jarndt@ias.rwth-aachen.de>
 * @author      Steffen Robertz <steffen.robertz@rwth-aachen.de>
 * @author      Robert Olsson <roolss@kth.se>
 *
 * This is based on  boards_jiminy-mega256rfr2
 */

#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "rss2.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name   Baudrate for STDIO terminal
 *
 * The standard configuration for STDIO in spu/atmega_comman/periph/uart.c
 * is to use double speed.
 *
 *
 * Matches this with BAUD in Board/Makefile.include
 *
 * @{
 */
#ifndef STDIO_UART_BAUDRATE
#define STDIO_UART_BAUDRATE (115200)       /**< Sets Baudrate for e.g. Shell */
#endif
/** @} */


/**
 * @name   LED pin definitions and handlers
 * @{
 */
#define LED_PORT            PORTE
#define LED_PORT_DDR        DDRE

#define LED0_PIN             (1 << LED_RED )
#define LED1_PIN             (1 << LED_YELLOW)

#define LED0_MASK           (1 << DDE4)
#define LED1_MASK           (1 << DDE3)

#define LED0_ON             (LED_PORT |=  LED0_MASK)
#define LED0_OFF            (LED_PORT &= ~LED0_MASK)
#define LED0_TOGGLE         (LED_PORT ^=  LED0_MASK)

#define LED1_ON             (LED_PORT |=  LED1_MASK)
#define LED1_OFF            (LED_PORT &= ~LED1_MASK)
#define LED1_TOGGLE         (LED_PORT ^=  LED1_MASK)


/**
 * @name    Usage of LED to turn on when a kernel panic occurs.
 * @{
 */
#define LED_PANIC            LED0_ON //LED_RED_ON
/** @} */

  
/**
 * @name DS18 pins  OW_BUS_0
 * @{
 */
#define DS18_PARAM_PIN  0xD7
#define DS18_PARAM_PULL   (GPIO_IN_PU)
  //#define DS18_PARAM_PULL   (GPIO_IN)  
  
  
/**
 * @name xtimer configuration values
 * @{
 */
#define XTIMER_DEV          TIMER_DEV(0)
#define XTIMER_CHAN         (0)
#define XTIMER_WIDTH        (16)
//#define XTIMER_HZ           (125000UL)
#define XTIMER_HZ                   (62500UL)
//#define XTIMER_BACKOFF              (40)

/**
 * @name Indicate Watchdog cleared in bootloader an
 *
 * AVR CPUs need to reset the Watchdog as fast as possible.
 * This flag indicates that the watchdog is reseted in the bootloader
 * and that the MCUSR value is stored in register 0 (r0)
 * @{
 */
#define BOOTLOADER_CLEARS_WATCHDOG_AND_PASSES_MCUSR 0
/** @} */

/**
 * @name CPU clock scale for jiminy-megarfr256rfr2
 *
 * The CPU can not be used with the external xtal oscillator if the core
 * should be put in sleep while the transceiver is in rx mode.
 *
 * It seems the as teh peripheral clock divider is set to 1 and this all
 * clocks of the timer, etc run with 16MHz increasing power consumption.
 */
#define CPU_ATMEGA_CLK_SCALE_INIT    CPU_ATMEGA_CLK_SCALE_DIV1
/** @} */

/**
 * @name    User button configuration
 * @{
 */
#define BTN0_PIN            GPIO_PIN(1, 0)
#define BTN0_MASK           (0x00)
#define BTN0_MODE           GPIO_IN

#define BTN0_PRESSED        ((BTN0_PIN & BTN0_MASK) == 0)
#define BTN0_RELEASED       ((BTN0_PIN & BTN0_MASK) != 0)
  
/**
 * @brief Initialize board specific hardware, including clock, LEDs and std-IO
 */
void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
