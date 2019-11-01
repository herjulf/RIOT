/*
 * Copyright (C) 2016 RWTH Aachen, Josua Arndt
 *               2019 Otto-von-Guericke-Universität Magdeburg
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
 * @brief       Peripheral MCU configuration for the Jiminy Mega 256rfr2 board
 *
 * @author      Josua Arndt <jarndt@ias.rwth-aachen.de>
 * @author      Steffen Robertz <steffen.robertz@rwth-aachen.de>
 * @author      Marian Buschsieweke <marian.buschsieweke@ovgu.de>
 * @author      Robert Olsson <roolss@kth.se>
 *
 * This is based on  boards_jiminy-mega256rfr2
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name   Clock configuration
 * @{
 */
#define CLOCK_CORECLOCK     (16000000UL)
/** @} */

#ifdef __cplusplus
}
#endif

#include "periph_conf_atmega_common.h"

#endif /* PERIPH_CONF_H */
/** @} */
