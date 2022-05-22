<script setup>
import { NIcon, NButton, NSpace, NDataTable } from 'naive-ui'
import { Refresh, Add } from "@vicons/ionicons5";

import { ref, onMounted, h } from 'vue'
import configModal from './configModal.vue'

const _configModal = ref(null)
const columns = ref([

    {
        title: "名称",
        key: "name"
    },
    {
        title: "纸张",
        key: "paper"
    },
    {
        title: "边距←↑→↓mm",
        key: "s"
    },
    {
        title: "方向",
        key: "orientation"
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
                        onclick: () => row => {

                        }

                    },
                    { default: () => "修改" }
                ),
                h(
                    NButton,
                    {
                        strong: true,
                        type: "warning",
                        onclick: () => row => {

                        }

                    },
                    { default: () => "删除" }
                ),



            ]
        }
    }


])
const data = ref([


    {
        name: "测试",
        paper: "A4",
        s: "123"



    }
])

function add() {
    _configModal.value.showOrHide()
}


</script>

<template>

    <configModal ref="_configModal" style="width: 1000px;height: 700px;">


    </configModal>
    <h1>1. 打印机配置</h1>
    <n-space>

        <n-button type="primary">
            <template #icon>
                <n-icon :size="20" :component="Refresh"></n-icon>
            </template>
            刷新
        </n-button>
        <n-button type="info" v-on:click="add">
            <template #icon>
                <n-icon :size="20" :component="Add"></n-icon>
            </template>
            添加
        </n-button>

    </n-space>
    <div style="height: 20px;"></div>
    <n-data-table :columns="columns" :data="data" :bordered="true" :min-height="500"/>





    <h1>2. 测试</h1>
    <div style="background-color: red; height: 2000px;"></div>


</template>

<style>
</style>