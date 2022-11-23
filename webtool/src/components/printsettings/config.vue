<script setup>
import {
    NIcon,
    NButton,
    NSpace,
    NDataTable,
    useDialog,
    NInput

} from 'naive-ui'
import { Add } from "@vicons/ionicons5";
import { LogoRss } from "@vicons/ionicons5";
import ServerNet from "../websocket"

import { ref, onMounted, h } from 'vue'
import configModal from './configModal.vue'
const emit = defineEmits(['printerConfigUpdated'])
const dialog = useDialog();
const _configModal = ref(null)
const tableData = ref([])
const columns = [
    { title: "名称", key: "Name" },
    { title: "打印机", key: "PrinterName" },
    { title: "纸张", key: "PaperName" },
    { title: "边距👈mm", key: "LeftMargin" },
    { title: "边距👆mm", key: "TopMargin" },
    { title: "边距👉mm", key: "RightMargin" },
    { title: "边距👇mm", key: "BottomMargin" },
    { title: "方向", key: "Orientation" },
    {
        title: "操作", key: "op", width: "200", fixed: "right",
        render(row) {
            return [
                h(
                    NButton,
                    {
                        strong: true,
                        type: "info",
                        style: {
                            marginRight: '6px'
                        },
                        onclick: () => {
                            _configModal.value.showOrHide('update', row)
                        }

                    },
                    { default: () => "修改" }
                ),
                h(
                    NButton,
                    {
                        strong: true,
                        type: "warning",
                        onclick: () => {
                            dialog.warning({
                                title: "删除",
                                content: "不可恢复,确定删除吗？",
                                positiveText: '我确定',
                                negativeText: "我再想一下",
                                onPositiveClick: () => {
                                    delOnePrinterConfig(row.Id)
                                }

                            })

                        }

                    },
                    { default: () => "删除" }
                ),
            ]
        }
    }
]
function add() {
    _configModal.value.showOrHide('add', {})
}
async function getPrinterConfigInfo() {
    let res = await ServerNet.send({ MsgType: "GetPrintConfigs" })
    if (res.IsSuccess) {
        if (res.Result && res.Result.length > 0) {
            tableData.value = res.Result
            emit('printerConfigUpdated', res.Result)
            return
        }
    }
    tableData.value = []
}

async function delOnePrinterConfig(Id) {
    let res = await ServerNet.send({ MsgType: "DelOnePrintConfig", Data: Id })
    if (res.IsSuccess) {
        if (res.IsSuccess) {
            dialog.success({
                title: "删除",
                content: "删除成功了！",
                positiveText: '😒',

            })
            getPrinterConfigInfo()
            return
        } else {
            dialog.error({
                title: "删除失败了",
                content: res.Message,
                positiveText: '🤪',

            })

        }
    }
}

onMounted(async () => {


    let res = await ServerNet.send({ MsgType: "GetPrintInfo" })
    if (res.IsSuccess) {
        _configModal.value.setPrintInfoData(res.Result)
    }
    getPrinterConfigInfo();
})


</script>

<template>
    <configModal ref="_configModal" @reload="getPrinterConfigInfo" style="width: 1000px;height: 440px;">
    </configModal>

    <h2>
        1.3 打印配置
    </h2>
    <n-space style="margin-top: 2vh;">
        <n-button type="info" v-on:click="add" style="margin-bottom: 1vh;">
            <template #icon>
                <n-icon :size="20" :component="Add"></n-icon>
            </template>
            添加
        </n-button>
    </n-space>
    <n-data-table :columns="columns" :data="tableData" flex-height style="min-height: 600px;max-height: 600px;" />
    <n-space style="margin-top: 20px;width: 100%;">
        <n-button type="info">
            <template #icon>
                <n-icon :size="20" :component="LogoRss"></n-icon>
            </template>
            远端websocket
        </n-button>
        <n-input style="width: 450px;">

        </n-input>

    </n-space>

</template>

<style>

</style>