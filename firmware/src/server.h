/**
 * @file server.h
 * @author James Bennion-Pedley
 * @brief Server for handling ESP8266 onboarding
 * @date 24/09/2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __FIRMWARE_SRC_SERVER_H__
#define __FIRMWARE_SRC_SERVER_H__

/*--------------------------------- Includes ---------------------------------*/

/*--------------------------------- Datatypes --------------------------------*/

/*--------------------------------- Functions --------------------------------*/

const char *server_get_ssid(void);
const char *server_get_psk(void);

void server_initialise(void);
void server_loop(void);
void server_launch_ap(const char *ap_prefix);

/*----------------------------------------------------------------------------*/

#endif /* __FIRMWARE_SRC_SERVER_H__ */
