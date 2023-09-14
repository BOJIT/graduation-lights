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

import { get, writable, type Writable } from 'svelte/store';

/*--------------------------------- Types ------------------------------------*/

type SystemState = {
    mode: string,
    colour: string,
    syncState: boolean;
};

/*--------------------------------- State ------------------------------------*/

const TOPIC_PREFIX = "DIET-4073c85645649a02734";

let client: mqtt.MqttClient | null = null;

let devices: Writable<Set<string>> = writable(new Set([]));

let state: Writable<SystemState> = writable({
    mode: "Off",
    colour: "#0000FF",
    syncState: false,
});

/*------------------------------- Functions ----------------------------------*/

async function connect() {
    // Connect to broker
    if (import.meta.env.PROD) {
        client = await mqtt.connectAsync("wss://broker.emqx.io:8084/mqtt");
    } else {
        client = await mqtt.connectAsync("ws://broker.emqx.io:8083/mqtt");
    }

    // Core subtopics
    await subscribe("discover");    // Gets light addresses + status
    await subscribe("command");    // Gets commands sent by other browsers

    // Set up message handler
    client.on('message', function (topic, payload, packet) {

        // Add discovery entries to set
        if (topic === `${TOPIC_PREFIX}/discover`) {
            const msg = JSON.parse(payload.toString());

            if (get(state).syncState === false) {
                // If this is the first light discovery, update the app state
                state.update((s) => {
                    s.mode = msg.mode;
                    s.colour = msg.colour;
                    s.syncState = true;
                    return s;
                });
            }

            devices.update((d) => {
                d.add(msg.mac);
                return d;
            });
            // TODO add callback after 20 seconds to remove from list if not heard from

        } else if (topic === `${TOPIC_PREFIX}/command`) {
            const msg = JSON.parse(payload.toString());

            // TODO check if it came from me!
            state.update((s) => {
                s.mode = msg.mode;
                s.colour = msg.colour;
                return s;
            });
        } else {
            // Payload is Buffer
            console.log(`Topic: ${topic}, Message: ${payload.toString()}`)
        }

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

export { devices, state };

export default { connect, publish, subscribe };
