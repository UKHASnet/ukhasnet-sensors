/**
 * This file is part of the UKHASnet maintained sensor library.
 *
 * @file max31855.h
 * @addtogroup ukhasnet-sensor
 * @{
 */
#ifndef __MAX31855_CONF_H__
#define __MAX31855_CONF_H__

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/* Public prototypes here */
void thermocouple_init(void);
bool thermocouple_read_temp(int32_t* temperature);

#endif /* __MAX31855_CONF_H__ */