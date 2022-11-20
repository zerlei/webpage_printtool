<script setup>
import config from "./config.vue"
import dashboard from "./dashboard.vue";
import printtest from "./printtest.vue"
import {ref,nextTick} from 'vue'

const printTest = ref(null)

function unPrinterConfigUpdated(data) {

    nextTick(()=>{
        printTest.value.setPrinterConfigOptions(data)
    })
}
</script>
<template>
    <dashboard>
    </dashboard>
    <config @printerConfigUpdated="unPrinterConfigUpdated">
    </config>
    <printtest ref="printTest">
    </printtest> 
</template>