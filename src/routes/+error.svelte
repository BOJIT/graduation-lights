<!--
 * @file +error.svelte
 * @author James Bennion-Pedley
 * @brief Generic Error Page
 * @date 21/01/2023
 *
 * @copyright Copyright (c) 2023
 *
-->

<script lang='ts'>
    /*-------------------------------- Imports -------------------------------*/

    import { page } from '$app/stores';

    // Assets
    import logo from "$lib/img/logo_x512.png";
</script>


<div class="error">
    <img src={logo} alt="logo"/>
    <h1>{$page.status}</h1>
    <h3>{$page.error.message}</h3>
</div>


<style>
    .error {
        height: 100vh;

        display: flex;
        text-align: center;
        flex-direction: column;
        align-items: center;
        justify-content: center;
    }

    h1 {
        margin-bottom: -2rem !important;
    }

    h3 {
        color: var(--color-gray-700);
    }

    img {
        max-height: 13rem;
    }
</style>
