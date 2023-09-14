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
    // Connect to broker
    if (import.meta.env.PROD) {
        client = await mqtt.connectAsync("wss://broker.emqx.io:8084/mqtt");
    } else {
        client = await mqtt.connectAsync("ws://broker.emqx.io:8083/mqtt");
    }

    // Set up message handler
    client.on('message', function (topic, payload, packet) {
        // Payload is Buffer
        console.log(`Topic: ${topic}, Message: ${payload.toString()}`)
    })

    return client;
}

async function publish(subtopic: string, message: string) {
    if (client === null) return;

    await client.publishAsync(`${TOPIC_PREFIX}/${subtopic}`, message);
}

async function subscribe(subtopic: string) {
    if (client === null) return;

    return await client.subscribeAsync(`${TOPIC_PREFIX}/${subtopic}`);
}

/*-------------------------------- Exports -----------------------------------*/

export default { connect, publish, subscribe };
