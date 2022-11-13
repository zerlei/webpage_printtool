<script setup>

import {
    NModal,
    NStep,
    NSpace,
    NSteps,
    NButton,
    NDataTable,
    useDialog,
    NInputNumber,
    NSwitch,
    NInput

} from 'naive-ui'

import { ref, watch } from 'vue'
import ServerNet from "../websocket"

const emit = defineEmits(['reload'])
const showModal = ref(false)
const currentStep = ref(1)
const acceptinfo = ref("下一步")
const currentStatus = ref('process')
const beforedisabled = ref(true)
const Printercolumns = ref([
    { type: 'selection', multiple: false, },
    { title: "打印机名称", key: "PrinterName" }
])
const PrinterData = ref([])
const PrinterChecked = ref([])
const Papercolumns = ref([
    { type: 'selection', multiple: false, },
    { title: "纸张名称", key: "PaperName" },
    { title: "纸张尺寸", key: "PaperSize" }
])
const PaperData = ref([])
const PaperChecked = ref([])
const dialog = useDialog()

const Margin = ref({
    Top: 0,
    Left: 0,
    Bottom: 0,
    Right: 0

})
const railStyle = ref(({
    focused,
    checked
}) => {
    const style = {};
    if (checked) {
        style.background = "#f0a020";
        if (focused) {
            style.boxShadow = "0 0 0 2px #f0a0200";
        }
    } else {
        style.background = "#2080f0";
        if (focused) {
            style.boxShadow = "0 0 0 2px #2080f040";
        }
    }
    return style;
})
const Orientation = ref(true)
const Name = ref("")
const NameInputStatus = ref("success")
const NameInputPh = ref("英文~")

let PrinterInfoData = []
let type_ = 'add'
let configId = null

watch(Name, (n, o) => {
    if (n == "" || n == null) {
        NameInputStatus.value = 'error'
    } else {
        NameInputStatus.value = 'success'
    }
})``

async function addOnePrintConfig(ob) {

    let res = await ServerNet.send({ MsgType: "AddOnePrintConfig", Data: ob })
    if (res.IsSuccess && res.Result.IsSuccess) {
        dialog.success({
            title: '成功',
            content: '添加成功！',
            positiveText: '好的',
            onPositiveClick: () => {
            }
        })
        showModal.value = false
        currentStep.value = 1
        acceptinfo.value = "下一步"
        beforedisabled.value = true
        emit('reload')
    } else {
        dialog.error({
            title: '失败',
            content: res.Result.Message,
            positiveText: '好的',
            onPositiveClick: () => {
            }
        })
    }


}
async function updateOnePrintConfig(ob) {
    // //TODO
    // fetch('http://127.0.0.1:8847/PrintController/updateoneprintconfig', {
    //     method: "POST",
    //     body: JSON.stringify(ob)
    // })
    //     .then(response => response.json())
    //     .then(data => {
    //         if (data.isSuccess) {
    //             dialog.success({
    //                 title: '成功',
    //                 content: '修改成功！',
    //                 positiveText: '好的',
    //                 onPositiveClick: () => {
    //                 }
    //             })
    //             showModal.value = false
    //             currentStep.value = 1
    //             acceptinfo.value = "下一步"
    //             beforedisabled.value = true
    //             emit('reload')

    //         } else {
    //             dialog.error({
    //                 title: '失败',
    //                 content: data.message,
    //                 positiveText: '好的',
    //                 onPositiveClick: () => {
    //                 }
    //             })
    //         }
    //     });
}
//#region 显示效果
function acceptNext() {
    if (currentStep.value == 1) {

        PrinterInfoData.forEach(e => {
            if (e.PrinterName == PrinterChecked.value[0]) {
                PaperData.value = e.Papers
                if (PaperData.value.length > 0 && PaperChecked.value.length == 0) {
                    PaperChecked.value = [PaperData.value[0].Id]
                }
            }
        })
        currentStep.value = currentStep.value + 1
        beforedisabled.value = false
        return

    } else if (currentStep.value == 2) {
        acceptinfo.value = "确定"
        currentStep.value = currentStep.value + 1
        return

    }
    if (currentStep.value == 3) {
        if (Name.value == "" || Name.value == null) {
            NameInputStatus.value = 'error'
            NameInputPh.value = "❌,输入名字！"
            return
        }
        let ob = {}
        ob.Name = Name.value
        ob.TopMargin = Margin.value.Top
        ob.BottomMargin = Margin.value.Bottom
        ob.LeftMargin = Margin.value.Left
        ob.RightMargin = Margin.value.Right
        ob.Orientation = Orientation ? "横向" : "纵向"
        ob.PrinterName = PrinterChecked.value[0]
        ob.PaperName = PaperData.value.find(e => {
            if (e.Id == PaperChecked.value[0]) {
                return true
            }
        }).PaperName
        if (type_ == 'add') {
            addOnePrintConfig(ob)

        } else {
            ob.Id = configId
            updateOnePrintConfig(ob)
        }
    }
}
function acceptBefore() {

    currentStep.value = currentStep.value - 1
    acceptinfo.value = "下一步"
    if (currentStep.value == 1) {
        beforedisabled.value = true
    }
}

function cancle() {
    showModal.value = false
}
function showOrHide(from, row) {
    showModal.value = true
    if (from == 'add') {

    } else {
        type_ = 'update'
        configId = row.Id

        if (row.Orientation == '横向') {
            Orientation.value = true
        }

        Margin.value = {
            Top: row.TopMargin,
            Left: row.LeftMargin,
            Right: row.RightMargin,
            Bottom: row.BottomMargin
        }
        Name.value = row.Name
        PrinterChecked.value = [`${row.PrinterName}`]
        PrinterInfoData.forEach(e => {
            if (e.PrinterName == row.PrinterName) {
                PaperData.value = e.Papers
                e.Papers.forEach(e2 => {
                    if (e2.PaperName == row.PaperName) {
                        PaperChecked.value = [e2.Id]
                    }
                })
            }
        })

    }

}


//#endregion

function setPrintInfoData(data) {
    PrinterInfoData = data
    PrinterInfoData.forEach(e => {
        // 打印机的纸张存在同名同Id的
        for (var i = 0; i < e.Papers.length; ++i) {
            e.Papers[i].Id = i
        }
        let ob = {}
        ob.PrinterName = e.PrinterName
        PrinterData.value.push(ob)

    });
    if (PrinterData.value.length > 0) {
        PrinterChecked.value.push(PrinterData.value[0].PrinterName)
    }

}
defineExpose({
    showOrHide,
    setPrintInfoData
})
</script>
<template>

    <n-modal v-model:show="showModal" id="configModal" :mask-closable="false">
        <n-space vertical>
            <n-steps size="small" :current="currentStep" :status="currentStatus">
                <n-step title="选择一个打印机" description="从当前的打印机中选择一个" />
                <n-step title="选择纸张" description="然后，这些是打印机支持的纸张" />
                <n-step title="配置打印机" description="配置。只是记忆一个打印机状态，并不是配置打印机驱动~" />
            </n-steps>

            <n-data-table v-show="currentStep == 1" :row-key="row => row.PrinterName" :columns="Printercolumns"
                :data="PrinterData" v-model:checked-row-keys="PrinterChecked" :style="{ height: `300px` }"
                flex-height />
            <n-data-table v-show="currentStep == 2" :row-key="row => row.Id" :columns="Papercolumns" :data="PaperData"
                v-model:checked-row-keys="PaperChecked" :style="{ height: `300px` }" flex-height />

            <div v-show="currentStep == 3" style="height: 300px;">
                <n-space justify="end" vertical>
                    <n-space style="margin-bottom: 40px;">
                        边距：
                        <n-input-number v-model:value="Margin.Left" :min="0"></n-input-number>
                        <n-input-number v-model:value="Margin.Top" :min="0"></n-input-number>
                        <n-input-number v-model:value="Margin.Right" :min="0"></n-input-number>
                        <n-input-number v-model:value="Margin.Bottom" :min="0"></n-input-number>
                    </n-space>

                    <n-space style="margin-bottom: 40px;">
                        方向：
                        <n-switch :rail-style="railStyle" v-model:value="Orientation">
                            <template #checked>
                                横向
                            </template>
                            <template #unchecked>
                                纵向
                            </template>
                        </n-switch>
                    </n-space>
                    <n-space>
                        名字：
                        <n-input v-model:value="Name" type="text" v-model:placeholder="NameInputPh"
                            v-model:status="NameInputStatus" />
                    </n-space>

                </n-space>
            </div>
            <n-space justify="end">
                <n-button type="warning" v-on:click="cancle">
                    取消
                </n-button>
                <n-button color="#8a2be2" :disabled="beforedisabled" v-on:click="acceptBefore">
                    上一步
                </n-button>
                <n-button type="success" v-on:click="acceptNext">
                    {{ acceptinfo }}
                </n-button>
            </n-space>
        </n-space>
    </n-modal>
</template>
<style>
#configModal {
    background-color: #2c2c32;
    border-radius: 10px;
    padding: 20px;
}
</style>