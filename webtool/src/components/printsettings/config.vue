<script setup>
import {
    NIcon,
    NButton,
    NSpace,
    NDataTable,
    useDialog,
    NInput,
NTag

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
    { title: "宽度mm", key: "PaperWidthInmm" },
    { title: "高度mm", key: "PaperHeightInmm" },
    { title: "边距👈mm", key: "LeftMargin" },
    { title: "边距👆mm", key: "TopMargin" },
    { title: "边距👉mm", key: "RightMargin" },
    { title: "边距👇mm", key: "BottomMargin" },
    { title: "打印机", key: "PrinterName" },
    { title: "打印机纸张", key: "PaperName" },
    { title: "打印方向", key: "Orientation" },
    {
        title: "保存类型", key: "SaveType",width:350, render(row) {

            if(['',null,undefined].includes(row.SaveType)) {
                return row.SaveType
            } else {
                let arr = [

                    h(
                        NTag,
                        {
                            type:"success",
                        },
                        {
                            default:()=>{
                                if(row.SaveType[0] == "1") {
                                    return "保存pdf"
                                } else {
                                    return "不保存pdf"
                                }
                            }
                        }

                    ),
                    h(
                        NTag,
                        {
                            type:"success",
                        },
                        {
                            default:()=>{
                                if(row.SaveType[1] == "1") {
                                    return "保存png图片"
                                } else {
                                    return "不保存png图片"
                                }
                            }
                        }

                    ),
                    h(
                        NTag,
                        {
                            type:"success",
                        },
                        {
                            default:()=>{
                                if(row.SaveType[2] == "1") {
                                    return "使用打印机"
                                } else {
                                    return "不使用打印机"
                                }
                            }
                        }

                    ),
                    h(
                        NTag,
                        {
                            type:"success",
                        },
                        {
                            default:()=>{
                                if(row.SaveType[3] == "1") {
                                    return "使用打印机自定义配置"
                                } else {
                                    return "使用打印机默认配置"
                                }
                            }
                        }

                    ),


                ]
     

                return h(
                    NSpace,
                    {
                        size:'small'
                    },
                    {
                        default:()=>arr
                    }
                    
                )

            }

        }
    },

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

const _webSocketUrl = ref(null)
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
async function InsertOrUpdateWebsocketUrl() {

    let res = await ServerNet.send({ MsgType: "InsertOrUpdateWebsocketUrl", Data: _webSocketUrl.value })
    if (res.IsSuccess) {
        dialog.success({
            title: "成功",
            content: "设置成功了！",
            positiveText: '😀',

        })
    }
    else {
        dialog.error({
            title: "失败了~",
            content: res.Message,
            positiveText: '😢',

        })
    }

}

onMounted(async () => {


    let res = await ServerNet.send({ MsgType: "GetPrintInfo" })
    if (res.IsSuccess) {
        _configModal.value.setPrintInfoData(res.Result)
    }
    getPrinterConfigInfo();

    let res2 = await ServerNet.send({ MsgType: "GetWebsocketUrl" })
    if (res2.IsSuccess) {
        _webSocketUrl.value = res2.Result.WebSocUrl
    }
})


</script>

<template>
    <configModal ref="_configModal" @reload="getPrinterConfigInfo" style="width: 1000px;height: 500px;">
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
        <n-button @click="InsertOrUpdateWebsocketUrl" type="info">
            <template #icon>
                <n-icon :size="20" :component="LogoRss"></n-icon>
            </template>
            远端websocket
        </n-button>
        <n-input v-model:value="_webSocketUrl" style="width: 450px;">

        </n-input>

    </n-space>
</template>

<style></style>