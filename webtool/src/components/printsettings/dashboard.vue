<script setup>
import {
    NIcon,
    NSpace,
    NTooltip,
    NGradientText,
    NDataTable

} from 'naive-ui'
import { ref, onMounted, h } from 'vue'
import { PlugConnected24Filled, PlugDisconnected24Filled } from "@vicons/fluent"
const _server_websoc_connected_ = ref(true)

const tableData = ref([])
const columns = [
    { title: "页面地址", key: "PageName" },
    { title: "打印时间", key: "PrintTime" },
    { title: "请求IP", key: "FromIp" },
    { title: "请求类型", key: "FromType" },
    { title: "打印机配置名称", key: "ConfigName" },
    { title: "打印模式", key: "PrintMode" },
    { title: "是否成功", key: "IsSuccess" }
]
const loading = ref(true)
const pagination = ref({
    page: 1,
    pageSize: 20,
    pageCount: 1,
    prefix({ itemCount }) {
        return `共计: ${itemCount}`
    }
})

function pageChanged(currentPage) {
    if(!loading.value) {
        loading.value = true
        getPrintedPage(currentPage)
    }
}
async function getPrintedPage(currentPage) {
//TODO
}
onMounted( async ()=>{
    getPrintedPage(1)
})

</script>

<template>
    <h2>
        1.1 连接服务端websocet
    </h2>
    <!-- <n-tooltip trigger="hover">
            <template #trigger> -->
    <n-space style="margin-left: 50px;">
        <div>
            <n-tooltip trigger="hover" v-if="_server_websoc_connected_">
                <template #trigger>
                    <n-icon size="80" color="#2ADD9C">
                        <PlugConnected24Filled />
                    </n-icon>
                </template>
                已连接！👌
            </n-tooltip>
            <n-tooltip trigger="hover" v-else>
                <template #trigger>
                    <n-icon size="80" color="#BE002F">
                        <PlugDisconnected24Filled />
                    </n-icon>
                </template>
                还没有连接...🦉
            </n-tooltip>
        </div>
        <n-gradient-text type="info" style=" font-size: 40px;">
            websockerURL
        </n-gradient-text>

    </n-space>
    <h2>
        1.2 打印页面信息
    </h2>
    <n-data-table :columns="columns" remote :data="tableData" :loading="loading" striped
        style="min-height: 600px;max-height: 600px;" :pagination="pagination" v-on:update:page="pageChanged" />
</template>