#include "include/euimac_params.h"
#include "periph/i2c.h"

#define ENABLE_DEBUG        (0)
#include "debug.h"

#define _BUS                (dev->params.i2c_dev)
#define _ADDR               (dev->params.i2c_addr)

uint8_t eui64[8];

int euimac_init(euimac_t* dev, const euimac_params_t* params)
{
    int i2c_result;
    cmd_t command = read_eui64;

    dev->params = *params;

    i2c_acquire(_BUS);

    DEBUG("[EUIMAC] write command: addr=%02x cmd=%02x\n", _ADDR, (uint8_t)command);
    
    i2c_result = i2c_write_byte(_BUS, _ADDR, (uint8_t)command, 0);

    if (i2c_result == EUIMAC_OK) {
      i2c_result = i2c_read_bytes(_BUS, _ADDR, eui64, sizeof(eui64), 0);
    }
    i2c_release(_BUS);

    if (i2c_result != EUIMAC_OK) {
      return i2c_result;
    }

    DEBUG("EUI-64 MAC: %x-%x-%x-%x-%x-%x-%x-%x\n",
	 eui64[0], eui64[1], eui64[2], eui64[3], eui64[4], eui64[5], eui64[6], eui64[7]);
    
    return EUIMAC_OK;
}
