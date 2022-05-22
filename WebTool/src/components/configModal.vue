<script setup>

import { NModal, NStep, NSpace, NSteps, NButton, NDataTable } from 'naive-ui'

import { ref, defineExpose } from 'vue'
const showModal = ref(false)
const current = ref(1)
const acceptinfo = ref("下一步")
const currentStatus = ref('process')
const Printercolumns = ref([
    {
          type: 'selection',
          options: [
            'all',
            'none',
          ],
    },
    {
        title:"打印机名称",
        key:"name"
    }
])




function acceptNext() {
    current.value = current.value + 1
    if (current.value == 3) {
        acceptinfo.value = "确定"
    }
    if (current.value == 4) {

        showModal.value = false

        current.value = 1
        acceptinfo.value = "下一步"
    }

}

function cancle() {
    showModal.value = false
}
function showOrHide() {
    showModal.value = !showModal.value
}
defineExpose({
    showOrHide
})



</script>

<template>

    <n-modal v-model:show="showModal" id="configModal" :mask-closable="false">
        <n-space vertical>
            <n-steps size="small" :current="current" :status="currentStatus">
                <n-step title="选择一个打印机" description="从当前的打印机中选择一个" />
                <n-step title="选择纸张" description="这些是打印机支持的纸张" />
                <n-step title="配置打印机" description="上下左右边距，横向纵向，起一个名字（只是记忆一个打印机状态！并不是类似配置打印机驱动那种~）" />
            </n-steps>

            <n-data-table v-show="current == 1" :columns="Printercolumns" />
            <n-data-table v-show="current == 2" :columns="Papercolumns" />


            <n-space justify="end">

                <n-button type="warning" v-on:click="cancle">
                    取消
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
    background-color: aliceblue;
    border-radius: 10px;
    padding: 20px;
}
</style>