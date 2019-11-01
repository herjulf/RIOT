#include <stdint.h>
#include "byteorder.h"
#include "net/eui64.h"
#include "include/euimac_params.h"
#include "periph/i2c.h"

#define ENABLE_DEBUG        (0)
#include "debug.h"

#define _BUS                (dev->params.i2c_dev)
#define _ADDR               (dev->params.i2c_addr)

eui64_t euimac;

int euimac_init(euimac_t *dev, const euimac_params_t *params)
{
    int i2c_result, i;
    cmd_t command = read_eui64;
    uint8_t buf[8];

    dev->params = *params;

    i2c_acquire(_BUS);

    DEBUG("[EUIMAC] write command: addr=%02x cmd=%02x\n", _ADDR,
          (uint8_t)command);

    i2c_result = i2c_write_byte(_BUS, _ADDR, (uint8_t)command, 0);

    if (i2c_result == EUIMAC_OK) {
        i2c_result = i2c_read_bytes(_BUS, _ADDR, buf, sizeof(eui64_t), 0);
    }
    i2c_release(_BUS);

    if (i2c_result != EUIMAC_OK) {
        return i2c_result;
    }

    for (i = 0; i < 8; i++) {
        euimac.uint8[i] = buf[7 - i];
    }

    DEBUG("EUIMAC-64 MAC: %x-%x-%x-%x-%x-%x-%x-%x\n",
          euimac.uint8[0], euimac.uint8[1], euimac.uint8[2], euimac.uint8[3],
          euimac.uint8[4],
          euimac.uint8[5], euimac.uint8[6],
          euimac.uint8[7]);

    return EUIMAC_OK;
}
