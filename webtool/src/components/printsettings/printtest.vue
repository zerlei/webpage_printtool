<script setup>

import {
    ref
} from 'vue'

import {
    NButton,
    NInput,
    NSelect,
    NSwitch,
    NForm,
    NFormItem,
    useDialog
} from 'naive-ui'
import ServerNet from '../websocket'

const dialog = useDialog()
const options = ref([
])
const printBtnDisable = ref(false)
const _printRequest = ref({
    PageUrl: "https://www.bing.com",
    PrintMode: "LoadAchieve",
    ConfigName: null
})
const rules = ref({
    PageUrl: {
        required: true,
        message: "请输入网页！",
        trigger: "blur"
    },
    PrintMode: {
        required: true,
        message: "选择正确的值！",
        trigger: "blur"
    },
    ConfigName: {
        required: true,
        validator: (rule, value) => {
            if (!value) {
                return new Error("需要打印机");
            }
            return true;
        },
        trigger: ['blur', "change"]
    }
})
const formRef = ref(null)
function toPrint(e) {
    e.preventDefault();
    formRef.value?.validate(async (e) => {
        if (!e) {
            let ar = []
            ar.push(_printRequest.value)
            printBtnDisable.value = true
            let res = await ServerNet.send({ MsgType: "ToPrint", Data: ar })
            printBtnDisable.value = false
            if (res.IsSuccess && res.Result[0].IsSuccess) {
                dialog.success({
                    title: "打印结果",
                    content: "打印成功！",
                    positiveText: '😀',
                })
            } else {
                dialog.error({
                    title: "打印结果",
                    content: res.Result?res.Result[0].Message:res.Message,
                    positiveText: '😀',
                })
            }
        }
    })

}
const railStyle = ({
    focused,
    checked
}) => {
    const style = {};
    if (checked) {
        style.background = "#d03050";
        if (focused) {
            style.boxShadow = "0 0 0 2px #d0305040";
        }
    } else {
        style.background = "#2080f0";
        if (focused) {
            style.boxShadow = "0 0 0 2px #2080f040";
        }
    }
    return style;
}

function setPrinterConfigOptions(options_) {

    options.value = []

    options_.forEach(e => {

        options.value.push({
            label: e.Name,
            value: e.Name
        })

    });
}
defineExpose({
    setPrinterConfigOptions
})

</script>
<template>
    <h2>1.4 测试</h2>
    <n-form ref="formRef" inline label-width="120" label-placement="left" :rules="rules" :model="_printRequest">
        <n-form-item label="网页" path="PageUrl">
            <n-input v-model:value="_printRequest.PageUrl"></n-input>
        </n-form-item>
        <n-form-item label="打印方式：" path="PrintMode">
            <n-switch v-model:value="_printRequest.PrintMode" checked-value="LoadAchieve" :rail-style="railStyle"
                unchecked-value="JsPrintRequest">
                <template #checked>
                    页面加载完成时
                </template>
                <template #unchecked>
                    Js要求打印时
                </template>
            </n-switch>

        </n-form-item>
        <n-form-item label="打印机:" path="ConfigName">
            <n-select style="min-width: 120px;" clearable v-model:value="_printRequest.ConfigName" :options="options">
            </n-select>
        </n-form-item>
        <n-form-item>
            <n-button :disabled="printBtnDisable" attr-type="button" @click="toPrint">
                测试打印
            </n-button>
        </n-form-item>
    </n-form>
</template>