/* DHT22 driver by Ben Oxley <https://github.com/ben-oxley/>
 * sourced from: <https://github.com/ben-oxley/UKHASnet_Firmware/tree/baremetalavr/avr_repeater/sensors>
 */

#ifndef _DHT22_H_
#define _DHT22_H_

#include <inttypes.h>

typedef enum
{
  DHT_ERROR_NONE = 0,
  DHT_BUS_HUNG,
  DHT_ERROR_NOT_PRESENT,
  DHT_ERROR_ACK_TOO_LONG,
  DHT_ERROR_SYNC_TIMEOUT,
  DHT_ERROR_DATA_TIMEOUT,
  DHT_ERROR_CHECKSUM,
  DHT_ERROR_TOOQUICK
} DHT22_ERROR_t;

DHT22_ERROR_t dht22_read(float *temperature, float *humidity);

#endif /*_DHT22_H_*/