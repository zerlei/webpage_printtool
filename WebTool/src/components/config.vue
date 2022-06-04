<script setup>
import {
    NIcon,
    NButton,
    NSpace,
    NDataTable,
    useDialog,
    NRadio,
    NRadioGroup,
    NInputNumber

} from 'naive-ui'
import { Refresh, Add } from "@vicons/ionicons5";

import { ref, onMounted, h, watch } from 'vue'
import configModal from './configModal.vue'
import { isBoolean } from 'lodash';
const dialog = useDialog();
const _configModal = ref(null)
const columns = ref([

    {
        title: "名称",
        key: "Name"
    },
    {
        title: "打印机",
        key: "PrinterName"
    },
    {
        title: "纸张",
        key: "PaperName"
    },
    {
        title: "边距👈mm",
        key: "LeftMargin"
    },
    {
        title: "边距👆mm",
        key: "TopMargin"
    },
    {
        title: "边距👉mm",
        key: "RightMargin"
    },
    {
        title: "边距👇mm",
        key: "BottomMargin"
    },
    {
        title: "方向",
        key: "Orientation"
    },
    {
        title: "操作",
        key: "op",
        width: "200",
        fixed: "right",
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
                                    fetch(`http://127.0.0.1:8847/PrintController/deloneprintconfig?Id=${row.Id}`)
                                        .then(response => response.json())
                                        .then(data => {
                                            if (data.isSuccess) {
                                                dialog.success({
                                                    title: "成功",
                                                    content: "成功删除！",
                                                    positiveText: '好的',
                                                    onPositiveClick: () => {
                                                    }
                                                })
                                                GetPrinterConfigInfo()

                                            } else {
                                                dialog.error({
                                                    title: "失败",
                                                    content: data.message,
                                                    positiveText: '好的',
                                                    onPositiveClick: () => {
                                                    }
                                                })

                                            }

                                        });

                                }

                            })

                        }

                    },
                    { default: () => "删除" }
                ),



            ]
        }
    }


])
const tableData = ref([])
const screens = ref([])
const screenIndex = ref(0)
function add() {
    console.log(screenIndex.value)
    _configModal.value.showOrHide('add', {})
}

const currentScreen = ref({})

watch(screenIndex, (n, o) => {
    currentScreen.value = screens.value[n]
})

const paperDivStyle = ref({
    width: '0px',
    height: '0px',
    borderBottomWidth: '148.5px',
    borderTopWidth: '148.5px',
    borderLeftWidth: '105px',
    borderRightWidth: '105px',
    borderStyle: 'solid',
    borderTopColor: 'white',
    borderBottomColor: 'white',
    borderLeftColor: 'black',
    borderRightColor: 'black'


})
const paperDivWidth = ref(210)
const paperDivHeight = ref(297)

watch(paperDivWidth, (n, o) => {
    paperDivStyle.value.borderLeftWidth = `${n / 2 / screens.value[screenIndex.value].resolution_cm_X / 10}px`
    paperDivStyle.value.borderRightWidth = `${n / 2 / screens.value[screenIndex.value].resolution_cm_X / 10}px`

})
watch(paperDivHeight, (n, o) => {
    paperDivStyle.value.borderBottomWidth = `${n / 2 / screens.value[screenIndex.value].resolution_cm_Y / 10}px`
    paperDivStyle.value.borderTopWidth = `${n / 2 / screens.value[screenIndex.value].resolution_cm_Y / 10}px`

})

function GetPrinterConfigInfo() {
    // 配置的打印机信息
    fetch('http://127.0.0.1:8847/PrintController/GetPrintConfigs')
        .then(response => response.json())
        .then(data => {
            tableData.value = data
        });

}
onMounted(() => {
    // 本地打印机信息
    fetch('http://127.0.0.1:8847/PrintController/getprintinfo?isUpdate=true')
        .then(response => response.json())
        .then(data => {
            _configModal.value.setPrintInfoData(data)
        }
        );

    GetPrinterConfigInfo();
    fetch('http://127.0.0.1:8847/PrintController/getscreeninfo')
        .then(response => response.json())
        .then(data => {

            //Qt计算屏幕分辨率BUG
            data.forEach(e => {
                let ob = {}
                ob["resolutionX"] = e.width == ~~e.width ? e.width : e.width.toFixed(2)//分辨率 X
                ob["resolutionY"] = e.height == ~~e.height ? e.height : e.height.toFixed(2)//分辨率 Y
                ob["inchX"] = e.width / e.physicalDotsPerInchX//英寸 X
                ob["inchY"] = e.height / e.physicalDotsPerInchY//英寸 Y
                ob["inch"] = Math.sqrt(ob["inchX"] * ob["inchX"] + ob["inchY"] * ob["inchY"]).toFixed(2)//英寸

                ob["cmX"] = (2.54 * e.width / e.physicalDotsPerInchX).toFixed(2)//cm X
                ob["cmY"] = (2.54 * e.height / e.physicalDotsPerInchY).toFixed(2)//cm Y

                ob["resolution_cm_X"] = 2.54 / e.physicalDotsPerInchX //cm/分辨率 X
                ob["resolution_cm_Y"] = 2.54 / e.physicalDotsPerInchY //cm/分辨率 Y

                screens.value.push(ob)

            });
            if (screens.value.length > 0) {
                currentScreen.value = screens.value[0]
                paperDivStyle.value.borderLeftWidth = `${210/ 2 / screens.value[screenIndex.value].resolution_cm_X / 10}px`
                paperDivStyle.value.borderRightWidth = `${210/ 2 / screens.value[screenIndex.value].resolution_cm_X / 10}px`
                paperDivStyle.value.borderBottomWidth = `${297/ 2 / screens.value[screenIndex.value].resolution_cm_Y / 10}px`
                paperDivStyle.value.borderTopWidth = `${297/ 2 / screens.value[screenIndex.value].resolution_cm_Y / 10}px`


            }

        }
        );




})


</script>

<template>
    <configModal ref="_configModal" @reload="GetPrinterConfigInfo" style="width: 1000px;height: 440px;">
    </configModal>
    <h1>1. 打印机配置</h1>
    <n-space>
        <n-button type="info" v-on:click="add">
            <template #icon>
                <n-icon :size="20" :component="Add"></n-icon>
            </template>
            添加
        </n-button>

    </n-space>
    <div style="height: 20px;"></div>
    <n-data-table :columns="columns" :data="tableData" :bordered="true" :min-height="500" />
    <h1>2. 测试</h1>
    <n-radio-group v-model:value="screenIndex" name="screenGroup">
        <n-space>
            <n-radio v-for="(screen, index) in screens" :key="index" :value="index">屏幕{{ index }}</n-radio>
        </n-space>
    </n-radio-group>
    <n-space style="font-size: 20px;margin-top: 20px;">
        <div>屏幕分辨率: {{ currentScreen.resolutionX }}X{{ currentScreen.resolutionY }}</div>
        <div>屏幕大小(英寸): {{ currentScreen.inch }}</div>
        <div>屏幕大小(cm):{{ currentScreen.cmX }}X{{ currentScreen.cmY }}</div>
    </n-space>
    <n-space>
        <n-input-number :min="1" v-model:value="paperDivWidth">
            <template #prefix>
                宽(mm)
            </template>
        </n-input-number>
        <n-input-number :min="1" v-model:value="paperDivHeight">
            <template #prefix>
                高(mm)
            </template>
        </n-input-number>

    </n-space>


    <n-space justify="space-around">
        <div :style="paperDivStyle">
        </div>
    </n-space>



</template>

<style>
</style>