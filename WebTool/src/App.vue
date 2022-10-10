<script setup>

import { defineExpose, defineEmits, ref, h } from 'vue'
import { darkTheme, NMenu, NScrollbar, NConfigProvider, NMessageProvider, NDialogProvider } from 'naive-ui'
import { RouterLink } from 'vue-router'

// import {
//   BookOutline as BookIcon,
//   PersonOutline as PersonIcon,
//   WineOutline as WineIcon
// } from "@vicons/ionicons5";

const currentSelect = ref("config")
const menuOptions = ref([
  {
    label: () =>
      h(
        RouterLink,
        {
          to: {
            name: "config",
            params: {
              lang: 'zh-CN'
            }
          }
        }
        , { default: () => '配置' }
      ),
    key: "config",
  },
  {
    label: () =>

      h(
        RouterLink,
        {
          to: {
            name: "about"

          },
          params: {
            lang: 'zh-CN'
          }
        },
        { default: () => '关于' }
      ),
    key: "about"
  }
])

</script>

<template>
  <NConfigProvider>
    <NMessageProvider>
      <NDialogProvider>

        <div id="container">

          <div id="leftmenu">
            <!-- <div style="height: 10px;">

            </div> -->
            <NConfigProvider :theme="darkTheme">
              <n-menu :options="menuOptions" :default-expand-all="true" v-model:value="currentSelect"></n-menu>

            </NConfigProvider>
          </div>
          <n-scrollbar id="mainregion">
            <router-view v-slot="{ Component }">
              <keep-alive>
                <component :is="Component"></component>
              </keep-alive>
            </router-view>

          </n-scrollbar>

        </div>
      </NDialogProvider>
    </NMessageProvider>
  </NConfigProvider>

</template>

<style>
#container {
  display: flex;
  height: 100vh;

}

#leftmenu {
  width: 10%;
  height: 100%;
  background-color: rgb(40, 40, 40);

}

#mainregion {
  width: 90%;
  background-color: beige;
  padding-left: 30px;
  padding-right: 30px;
  max-height: 100vh;
}
</style>
