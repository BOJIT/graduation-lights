<!--
 * @file +layout.svelte
 * @author James Bennion-Pedley
 * @brief Main Application Page
 * @date 11/01/2023
 *
 * @copyright Copyright (c) 2023
 *
-->

<script lang="ts">
    /*-------------------------------- Imports -------------------------------*/
    import { App, Notification } from "@bojit/svelte-components/core";
    import Theme from "@bojit/svelte-components/theme";

    import palette from "$lib/palette";

    const mode = Theme.Mode;
    $mode = "light"; // Default Mode if no stored state is present
</script>

<App theme={palette}>
    <slot />
</App>

<Notification />
