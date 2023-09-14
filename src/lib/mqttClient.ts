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

const TOPIC_PREFIX = "DIET-4073c85645649a02734";

let client: mqtt.MqttClient | null = null;

/*------------------------------- Functions ----------------------------------*/

async function connect() {
    if (import.meta.env.PROD) {
        client = await mqtt.connectAsync("wss://broker.hivemq.com:8884");
    } else {
        client = await mqtt.connectAsync("ws://broker.hivemq.com:8000");
    }

    console.log(client);
}

async function publish(subtopic: string, message: string) {
    if (client === null) return;

    await client.publishAsync(`${TOPIC_PREFIX}/${subtopic}`, message);
}

async function subscribe() { }

/*-------------------------------- Exports -----------------------------------*/

export default { connect, publish };
