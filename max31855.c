/**
 * \mainpage
 * 
 * \section About
 * This file is part of the UKHASNet (ukhas.net) maintained sensor library for
 * use with all UKHASnet nodes, including Arduino, AVR and ARM.
 *
 * \section Authorship
 * Ported to AVR by Ben Oxley, 2017
 *
 * Some code based on the Adafruit MAX31855 arduino library
 * (https://github.com/adafruit/Adafruit-MAX31855-library)
 *
 * @file max31855.c
 * @addtogroup ukhasnet-sensor
 * @{
 */
#include "max31855.h"

/* SPI pins and ports - to be set by the user as required*/
#define SW_SPI_PIN     PINC
#define SW_SPI_DDR     DDRC
#define SW_SPI_PORT    PORTC
#define SW_SPI_CS      _BV(1)
#define SW_SPI_MISO    _BV(2)
#define SW_SPI_SCK     _BV(3)

/* Private functions */
int32_t _readCelsius(int32_t* v);

/**
* Method to set up the pins of the thermocouple reading board
*/
void thermocouple_init(void)
{
    /* Set up the SPI IO as appropriate */
    SW_SPI_DDR |= SW_SPI_CS | SW_SPI_SCK;
    SW_SPI_DDR &= ~(SW_SPI_MISO);

    /* Set SS high */
    SW_SPI_PORT |= SW_SPI_CS;
}

/**
* Read the temperature of the thermocouple. This reads from the MAX31855 over software 
* bit-banged SPI. 
* Takes an int32_t pointer which will be set, showing the temperature in degrees.
* @param temperature pointer to the value to be set during this method, representing
* the temperature in degrees.
*/
bool thermocouple_read_temp(int32_t* temperature)
{
    /* Ensure clock is low */
    SW_SPI_PORT &= ~(SW_SPI_SCK);
    /* Set SS low */
    SW_SPI_PORT &= ~(SW_SPI_CS);

    *temperature = 0;

    for (int i = 0; i < 32; i++){
        /* Pull clock high */
        SW_SPI_PORT |= (SW_SPI_SCK);

        //shift to the left
        *temperature = *temperature  << 1; 

        //Check the data
        if (PINC & SW_SPI_MISO){
            *temperature |= 1;
        }
        
        //pull clock low
        SW_SPI_PORT &= ~(SW_SPI_SCK);
    }
    /* Set SS high */
    SW_SPI_PORT |= (SW_SPI_CS);

    *temperature = _readCelsius(temperature);

    //Check if temp returns error
    return *temperature != -1000;
}

/**
* Helper method to convert the 32-bit data value from the MAX31855 
* to a int32_t representing the temperature in degrees.
* If the thermocouple is detected to not be attached this will return 0.
* If the thermocouple is detected to be shorted, this will return -1.
* @param v A pointer to the data in binary, read from the MAX31855.
* @returns A signed long int32_t representing the temperature of the thermocouple in celcius.
*/
int32_t _readCelsius(int32_t* v) {

  if (*v & 0x7) {
    // uh oh, a serious problem!
    return -1000; 
  }

  if (*v & 0x80000000) {
    // Negative value, drop the lower 18 bits and explicitly extend sign bits.
    *v = 0xFFFFC000 | ((*v >> 18) & 0x00003FFFF);
  }
  else {
    // Positive value, just drop the lower 18 bits.
    *v >>= 18;
  }
  
  double centigrade = *v;

  // LSB = 0.25 degrees C
  centigrade *= 0.25;
  //convert to int
  return (int32_t)centigrade;
}