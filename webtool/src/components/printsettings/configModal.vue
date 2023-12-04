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

const PaperWidthInmm = ref(210)
const PaperHeightInmm = ref(297)
const IsSavePdf = ref(true)
const IsSavePng = ref(false)
const IsUsePrinter = ref(true)
const IsUseThisConfig = ref(false)
const railStyle = ref(({
    focused,
    checked
}) => {
    const style = {};
    style.color = 'black'
    if (checked) {
        style.background = "#36ad6a";
        if (focused) {
            style.boxShadow = "0 0 0 2px #36ad6a";
        }
    } else {
        style.background = "#c98c33";
        if (focused) {
            style.boxShadow = "0 0 0 2px #c98c33";
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
})

async function addOnePrintConfig(ob) {

    let res = await ServerNet.send({ MsgType: "AddOnePrintConfig", Data: ob })
    if (res.IsSuccess) {
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
            content: res.Message,
            positiveText: '好的',
            onPositiveClick: () => {
            }
        })
    }


}
async function updateOnePrintConfig(ob) {
    let res = await ServerNet.send({ MsgType: "UpdateOnePrintConfig", Data: ob })

    if (res.IsSuccess) {
        dialog.success({
            title: '成功',
            content: '修改成功！',
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
            content: res.Message,
            positiveText: '好的',
            onPositiveClick: () => {
            }
        })
    }
}
//#region 显示效果
function acceptNext() {
    // debugger
    // if (currentStep.value == 0) {

    //     // PrinterInfoData.forEach(e => {
    //     //     if (e.PrinterName == PrinterChecked.value[0]) {
    //     //         PaperData.value = e.Papers
    //     //         if (PaperData.value.length > 0 && PaperChecked.value.length == 0) {
    //     //             PaperChecked.value = [PaperData.value[0].Id]
    //     //         }
    //     //     }
    //     // })

    //     currentStep.value = currentStep.value + 1
    //     return

    if (currentStep.value == 1) {
        beforedisabled.value = false
        acceptinfo.value = "确定"
        currentStep.value = currentStep.value + 1
        return

    }
    if (currentStep.value == 2) {
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
        ob.Orientation = Orientation.value ? "横向" : "纵向"
        ob.PrinterName = PrinterChecked.value[0]
        ob.PaperName = PaperData.value.find(e => {
            if (e.Id == PaperChecked.value[0]) {
                return true
            }
        }).PaperName
        ob.PaperWidthInmm = PaperWidthInmm.value
        ob.PaperHeightInmm = PaperHeightInmm.value

        const f = (is)=>{
            if(is) {
                return '1'
            }
            return '0'
        }

        ob.SaveType = `${f(IsSavePdf.value)}${f(IsSavePng.value)}${f(IsUsePrinter.value)}${f(IsUseThisConfig.value)}`
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
    if (!PrinterInfoData)
        return
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
        PaperData.value = PrinterInfoData[0].Papers
        PaperChecked.value.push(PaperData.value[0].Id)
    }

}
const PrinterCheckedChange = (keys) => {
    PrinterInfoData.forEach(e => {
        if (e.PrinterName == keys[0]) {
            PaperData.value = e.Papers
            if (PaperData.value.length > 0 ) {
                PaperChecked.value = [PaperData.value[0].Id]
            }
        }
    })

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
                <n-step title="配置" description="做一个打印配置，选择输出格式，起个名字等。" />
                <n-step title="打印机选择" description="选择（物理）打印机，选择纸张，纸张等要和之前的匹配~" />
            </n-steps>

            <div v-show="currentStep == 1" style="height: 350px;">
                <n-space style="margin-bottom: 40px;">
                    名字：
                    <n-input v-model:value="Name" type="text" v-model:placeholder="NameInputPh"
                        v-model:status="NameInputStatus" />
                </n-space>

                <n-space style="margin-bottom: 40px;">
                    尺寸：
                    <n-input-number v-model:value="PaperWidthInmm" :min="0" placeholder="宽度(mm)">
                        <template #prefix>
                            宽度(mm):
                        </template>
                    </n-input-number>
                    <n-input-number v-model:value="PaperHeightInmm" :min="0" placeholder="高度(mm)">
                        <template #prefix>
                            高度(mm):
                        </template>
                    </n-input-number>
                </n-space>


                <n-space justify="end" vertical>
                    <n-space style="margin-bottom: 40px;">
                        边距：
                        <n-input-number style="width: 200px;" v-model:value="Margin.Left" :min="0" placeholder="左边距(mm)">

                            <template #prefix>
                                左(mm):
                            </template>
                        </n-input-number>
                        <n-input-number style="width: 200px;" v-model:value="Margin.Top" :min="0" placeholder="上边距(mm)">
                            <template #prefix>
                                上(mm):
                            </template>
                        </n-input-number>
                        <n-input-number style="width: 200px;" v-model:value="Margin.Right" :min="0" placeholder="右边距(mm)">

                            <template #prefix>
                                右(mm):
                            </template>
                        </n-input-number>
                        <n-input-number style="width: 200px;" v-model:value="Margin.Bottom" :min="0" placeholder="下边距(mm)">
                            <template #prefix>
                                下(mm):
                            </template>
                        </n-input-number>
                    </n-space>

                    <n-space style="margin-bottom: 40px;">
                        输出：
                        <n-switch style="color: black;" :rail-style="railStyle" v-model:value="IsSavePdf">
                            <template #checked>
                                保存pdf
                            </template>
                            <template #unchecked>
                                不保存pdf
                            </template>
                        </n-switch>
                        <n-switch :rail-style="railStyle" v-model:value="IsSavePng">
                            <template #checked>
                                保存png图片
                            </template>
                            <template #unchecked>
                                不保存png图片
                            </template>
                        </n-switch>
                    </n-space>


                </n-space>
            </div>



            <div v-show="currentStep == 2">
                <n-space style="margin-bottom: 10px;">
                    输出：
                    <n-switch style="color: black;" :rail-style="railStyle" v-model:value="IsUsePrinter">
                        <template #checked>
                            使用打印机
                        </template>
                        <template #unchecked>
                            不使用打印机
                        </template>
                    </n-switch>
                    <n-switch :rail-style="railStyle" v-model:value="IsUseThisConfig">
                        <template #checked>
                            使用打印机自定义配置
                        </template>
                        <template #unchecked>
                            使用打印机默认配置
                        </template>
                    </n-switch>

                    <n-switch :rail-style="railStyle" v-model:value="Orientation">
                        <template #checked>
                            横向(正常)
                        </template>
                        <template #unchecked>
                            纵向(90°旋转)
                        </template>
                    </n-switch>
                </n-space>
                <n-space justify="space-between" :wrap="false">
                    <div>
                        <n-data-table :row-key="row => row.PrinterName" :columns="Printercolumns" :data="PrinterData"
                            :onUpdateCheckedRowKeys="PrinterCheckedChange" v-model:checked-row-keys="PrinterChecked"
                            :style="{ height: `350px` }" flex-height />
                    </div>
                    <div>
                        <n-data-table :row-key="row => row.Id" :columns="Papercolumns" :data="PaperData"
                            v-model:checked-row-keys="PaperChecked" :style="{ height: `350px` }" flex-height />
                    </div>
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