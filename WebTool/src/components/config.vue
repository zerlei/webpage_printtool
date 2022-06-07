<script setup>
import {
    NIcon,
    NButton,
    NSpace,
    NDataTable,
    useDialog

} from 'naive-ui'
import { Add } from "@vicons/ionicons5";

import { ref, onMounted, h} from 'vue'
import configModal from './configModal.vue'
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
function add() {
    _configModal.value.showOrHide('add', {})
}




function GetPrinterConfigInfo() {
    // 配置的打印机信息
    fetch('http://127.0.0.1:8847/PrintController/GetPrintConfigs')
        .then(response => response.json())
        .then(data => {
            if(data == null) {
                tableData.value = []
                return
            }
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




})


</script>

<template>
    <configModal ref="_configModal" @reload="GetPrinterConfigInfo" style="width: 1000px;height: 440px;">
    </configModal>
    <n-space style="margin-top: 2vh;">
        <n-button type="info" v-on:click="add">
            <template #icon>
                <n-icon :size="20" :component="Add"></n-icon>
            </template>
            添加
        </n-button>

    </n-space>
    <div style="height: 20px;"></div>
    <n-data-table :columns="columns" :data="tableData" :bordered="false" :min-height="600" />
    

</template>

<style>
</style>