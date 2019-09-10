/*
 * Copyright (C) 2019 KTH
 *               2017 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_euimac
 *
 * @{
 * @file
 * @brief       Default configuration for EUIMAC
 *
 * @author      Robert Olsson <roolss@kth.se>
 */

#ifndef EUIMAC_PARAMS_H
#define EUIMAC_PARAMS_H

#include "board.h"
#include "euimac.h"
#include "saul_reg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters for the EUIMAC
 * @{
 */
#ifndef EUIMAC_PARAM_I2C_DEV
#define EUIMAC_PARAM_I2C_DEV         I2C_DEV(0)
#endif
#ifndef EUIMAC_PARAM_I2C_ADDR
#define EUIMAC_PARAM_I2C_ADDR        EUIMAC_ADDR
#endif

#ifndef EUIMAC_PARAMS
#define EUIMAC_PARAMS                { .i2c_dev      = EUIMAC_PARAM_I2C_DEV,  \
                                       .i2c_addr     = EUIMAC_PARAM_I2C_ADDR}
#endif
#ifndef EUIMAC_SAUL_INFO
#define EUIMAC_SAUL_INFO             { .name = "euimac" }
#endif
/**@}*/

/**
 * @brief   Configure EUIMAC
 */
static const euimac_params_t euimac_params[] =
{
    EUIMAC_PARAMS
};

/**
 * @brief   Configure SAUL registry entries
 */
static const saul_reg_info_t euimac_saul_info[] =
{
    EUIMAC_SAUL_INFO
};

#ifdef __cplusplus
}
#endif

#endif /* EUIMAC_PARAMS_H */
/** @} */
