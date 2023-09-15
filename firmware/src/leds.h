/**
 * @file leds.h
 * @author James Bennion-Pedley
 * @brief LED Strip Animations
 * @date 15/09/2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __FIRMWARE_SRC_LEDS_H__
#define __FIRMWARE_SRC_LEDS_H__

/*--------------------------------- Includes ---------------------------------*/

#include <stdint.h>

/*--------------------------------- Datatypes --------------------------------*/

/*--------------------------------- Functions --------------------------------*/

void leds_pattern_off(void);
void leds_pattern_solid(uint8_t *cols);
void leds_pattern_fire(void);
void leds_pattern_sparkle(uint8_t *cols);
void leds_pattern_calming(void);
void leds_pattern_rainbow(void);

void leds_initialise(void);
void leds_render(void);

/*----------------------------------------------------------------------------*/

#endif /* __FIRMWARE_SRC_LEDS_H__ */
