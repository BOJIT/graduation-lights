/**
 * @file mqttClient.ts
 * @author James Bennion-Pedley
 * @brief MQTT Connection client code
 * @date 14/09/2023
 *
 * @copyright Copyright (c) 2023
 *
 */

/*-------------------------------- Imports -----------------------------------*/

import * as mqtt from 'mqtt/dist/mqtt.min';

/*--------------------------------- State ------------------------------------*/

let client;

/*------------------------------- Functions ----------------------------------*/

async function connect() {
    //
    client = await mqtt.connectAsync("wss://broker.hivemq.com:8884");

    console.log(client);
}

/*-------------------------------- Exports -----------------------------------*/

export default { connect };
