/*
 * Copyright (C) 2019 Robert Olsson / KTH
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_euimac EUIMAC
 * @ingroup     drivers_sensors
 * @brief       Device driver interface for the EUIMAC I2C chip
 * @{
 *
 * @file
 * @brief       Read eui64 MAC from i2c addr/eeprom chip
 *
 * @author      Robert Olsson <roolss@kth.se>
 */

#ifndef EUIMAC_H
#define EUIMAC_H

#include <stdint.h>
#include <stdbool.h>
#include "periph/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EUIMAC_PARAM_I2C_DEV
#define EUIMAC_PARAM_I2C_DEV         (I2C_DEV(0))
#endif
#ifndef EUIMAC_PARAM_I2C_ADDR
#define EUIMAC_PARAM_I2C_ADDR        (0x58)
#endif

typedef enum {
  read_eui64       = 0x98,     /* READ EUI64 8 bytes */
  read_id128       = 0x80,     /* READ 128 bit ID 16 bytes */
} cmd_t;

typedef struct {
    i2c_t i2c_dev;                      /**< I2C device               */
    uint8_t i2c_addr;                   /**< I2C address              */
} euimac_params_t;

typedef struct {
    euimac_params_t params;              /**< Device Parameters */
} euimac_t;

  /**
 * @brief       Status and error return codes
 */
enum {
    EUIMAC_OK            =  0,           /**< everything was fine */
    EUIMAC_ERR_I2C       = -1,           /**< error initializing the I2C bus */
    EUIMAC_ERR_NODEV     = -2,           /**< did not detect Euimac */
    EUIMAC_ERR_NOCAL     = -3,           /**< could not read calibration data */
    EUIMAC_ERR_I2C_READ  = -4,           /**< I2C read error */
    EUIMAC_ERR_TIMEDOUT  = -5,           /**< timed out */
    EUIMAC_ERR_CRC       = -6,           /**< CRC error */
    EUIMAC_ERR_USERREG   = -7,           /**< cannot write User Reg */
    EUIMAC_ERR_RES       = -8,           /**< invalid resolution */
    EUIMAC_ERR_OTHER     = -999,         /**< fatal error */
};


int euimac_init(euimac_t* dev, const euimac_params_t* params);

/**
 * @brief       Init and read out eui64 address the i2c EUIMAC chip
 *
 * @param[in]   dev         Device descriptor of EUIMAC device to read from
 *
 * @return                  EUIMAC_OK on success
 * @return                  EUIMAC_ERR_I2C if given I2C is not enabled in board config
 */

#ifdef __cplusplus
}
#endif

#endif /* EUIMAC_H */
/** @} */
