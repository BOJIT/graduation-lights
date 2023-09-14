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

    import { SearchableList } from "@bojit/svelte-components/form";
    import { Content } from "@bojit/svelte-components/layout";
    import { Button } from "@bojit/svelte-components/smelte";

    import { ColorPalette, Moon } from "@svicons/ionicons-outline";

    import ColorPicker from "svelte-awesome-color-picker";

    /*--------------------------------- Types --------------------------------*/

    // type Mode = "Off" | "Pattern 1" | "Pattern 2" | "Pattern 3" | "Pattern 4";

    /*--------------------------------- Props --------------------------------*/

    let options = ["Off", "Pattern 1", "Pattern 2", "Pattern 3", "Pattern 4"];
    let mode: string = "Off";

    // let options = {
    //     Off: {
    //         icon: Moon,
    //     },
    //     "Pattern 1": {
    //         icon: ColorPalette,
    //     },
    //     "Pattern 2": {
    //         icon: ColorPalette,
    //     },
    //     "Pattern 3": {
    //         icon: ColorPalette,
    //     },
    //     "Pattern 4": {
    //         icon: ColorPalette,
    //     },
    // };

    let hex: string = "#0000FF"; // or hsv or hex

    /*-------------------------------- Methods -------------------------------*/

    /*------------------------------- Lifecycle ------------------------------*/
    onMount(() => {
        // Subscribe to MQTT Broker
    });
</script>

<svelte:head>
    <title>Graduation Lights!</title>
</svelte:head>

<Content>
    <h2>Graduation Lights</h2>
    <hr />

    <h4>Change Pattern</h4>
    <hr />

    <!-- <SearchableList items={options} /> -->

    <div class="buttons">
        {#each options as o}
            <!-- <TextIconButton
                icon={ColorPalette}
                label={o}
                shape="rounded"
                color={mode === o ? "primary" : "secondary"}
            /> -->
            <Button
                outlined
                add={mode === o ? "bg-primary-trans" : ""}
                on:click={() => {
                    mode = o;
                }}><h2>{o}</h2></Button
            >
        {/each}
    </div>
    <hr />

    <ColorPicker
        bind:hex
        isAlpha={false}
        isPopup={false}
        label="Theme Colour"
    />
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
</style>
