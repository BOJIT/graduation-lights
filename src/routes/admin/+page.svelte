<!--
 * @file +page.svelte
 * @author James Bennion-Pedley
 * @brief Admin Dashboard
 * @date 14/09/2023
 *
 * @copyright Copyright (c) 2023
 *
-->

<script lang="ts">
    /*-------------------------------- Imports -------------------------------*/

    import { onMount } from "svelte";

    import { derived } from "svelte/store";

    import { SearchableList } from "@bojit/svelte-components/form";
    import { Content } from "@bojit/svelte-components/layout";

    import mqttClient, { devices, state } from "$lib/mqttClient";
    import { Switch } from "@bojit/svelte-components/smelte";

    /*--------------------------------- Props --------------------------------*/

    let deviceObject = derived(devices, (d: Set<string>) => {
        const o: any = {};

        d.forEach((e) => {
            o[e] = {};
        });

        return o;
    });

    let messageTimeout: number | null = null;

    /*-------------------------------- Methods -------------------------------*/

    async function setState() {
        const command = {
            mode: $state.mode,
            colour: $state.colour,
            enable: $state.enable,
            lock: $state.lock,
        };

        if (messageTimeout !== null) window.clearTimeout(messageTimeout);

        // Buffer sends to avoid sluggish reponse
        messageTimeout = window.setTimeout(async () => {
            await mqttClient.publish("command", JSON.stringify(command));
            messageTimeout = null;
        }, 300);
    }

    /*------------------------------- Lifecycle ------------------------------*/

    onMount(async () => {
        // Connect to MQTT Broker
        await mqttClient.connect();
    });
</script>

<svelte:head>
    <title>Admin Page</title>
</svelte:head>

<Content>
    <h2>Admin Dashboard</h2>
    <hr />

    <h4>Overrides</h4>
    <hr />
    <Switch
        label="Enable Lights"
        bind:value={$state.enable}
        on:change={setState}
    />
    <Switch
        label="Lock Settings"
        bind:value={$state.lock}
        on:change={setState}
    />
    <br /><br />

    <h4>Connected Devices ({$devices.size})</h4>
    <hr />
    <SearchableList items={$deviceObject} />
</Content>

<style>
    h2 {
        text-align: center;
        margin-top: 0.2rem !important;
        margin-bottom: 0.2rem !important;
    }

    h4 {
        color: var(--color-gray-500);
        text-align: center;
        margin-top: 0.2rem !important;
        margin-bottom: 0.2rem !important;
    }
</style>
