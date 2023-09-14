<!--
 * @file +page.svelte
 * @author James Bennion-Pedley
 * @brief Home Page
 * @date 26/01/2023
 *
 * @copyright Copyright (c) 2023
 *
-->

<script lang="ts">
    /*-------------------------------- Imports -------------------------------*/

    import { onMount } from "svelte";

    import { Content } from "@bojit/svelte-components/layout";
    import { Button } from "@bojit/svelte-components/smelte";

    import ColorPicker from "svelte-awesome-color-picker";

    import mqttClient from "$lib/mqttClient";

    /*--------------------------------- Types --------------------------------*/

    // type Mode = "Off" | "Pattern 1" | "Pattern 2" | "Pattern 3" | "Pattern 4";

    /*--------------------------------- Props --------------------------------*/

    let options = ["Off", "Pattern 1", "Pattern 2", "Pattern 3", "Pattern 4"];
    let mode: string = "Off";

    let hex: string = "#0000ff"; // or hsv or hex

    let messageTimeout: number | null = null;

    /*-------------------------------- Methods -------------------------------*/

    async function setState() {
        const commandString = `m:${mode}\nh:${hex}`;

        if (messageTimeout !== null) window.clearTimeout(messageTimeout);

        // Buffer sends to avoid sluggish reponse
        messageTimeout = window.setTimeout(async () => {
            await mqttClient.publish("command", commandString);
            messageTimeout = null;
        }, 300);
    }

    /*------------------------------- Lifecycle ------------------------------*/

    onMount(async () => {
        // Connect to MQTT Broker
        await mqttClient.connect();
        await mqttClient.subscribe("command");
    });
</script>

<svelte:head>
    <title>Graduation Lights!</title>
</svelte:head>

<Content>
    <h2>Graduation Lights</h2>
    <hr />

    <h4>Change Colour</h4>
    <hr />

    <div class="picker">
        <ColorPicker
            bind:hex
            isAlpha={false}
            label="Theme Colour"
            on:input={() => {
                setState();
            }}
        />
    </div>

    <h4>Change Pattern</h4>
    <hr />

    <div class="buttons">
        {#each options as o}
            <Button
                outlined
                add={mode === o ? "bg-primary-trans" : ""}
                on:click={() => {
                    mode = o;
                    setState();
                }}><h2>{o}</h2></Button
            >
        {/each}
    </div>
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

    .buttons {
        display: flex;
        flex-direction: column;
        gap: 0.5rem;
    }

    .picker {
        display: flex;
        align-items: center;
        /* justify-content: center; */
        padding: 1rem;
    }

    .picker :global(.color-picker > .wrapper) {
        margin-top: 0.5rem;
    }
</style>