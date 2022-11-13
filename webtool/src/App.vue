<script setup>

import { ref, h } from 'vue'
import { darkTheme, NMenu, NScrollbar, NConfigProvider, NMessageProvider, NDialogProvider } from 'naive-ui'
import { RouterLink } from 'vue-router'


const currentSelect = ref("setting")
const menuOptions = ref([
  {
    label: () =>
      h(
        RouterLink,
        {
          to: {
            name: "setting",
            params: {
              lang: 'zh-CN'
            }
          }
        }
        , { default: () => '配置' }
      ),
    key: "setting",
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
  <NConfigProvider :theme="darkTheme">
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
  /* height: 100%; */
  background-color: #18181c;

}

#mainregion {
  width: 100%;
  background-color: #0d1117;
  max-height: 100vh;
}

.markdown-body p {
  font-family: "颜宋简体";
}

.markdown-body li {
  font-family: "楷体";
}

.markdown-body pre {
  /* max-height: 1000px; */
  scroll-behavior: auto;
  padding: 0;
}

.markdown-body table th,
.markdown-body table td {
  font-size: 1px;
  max-width: 150px;
}

.markdown-body code {
  color: #ed5736;
}

.markdown-body pre code {
  font-family: "Consolas";
  color: white !important;
}

.markdown-body blockquote p {
  font-family: "柳公权楷体";
  background-color: rgb(40, 40, 40);
  border-radius: 5px;
  padding-left: 1%;
}

h1 {
  background-color: brown;
  border-radius: 5px;
  padding-left: 2%;
  padding-top: 5px;
  padding-bottom: 5px;
  color: azure;
}

h2 {
  background-color: #405f86;
  border-radius: 5px;
  padding-left: 2%;
  padding-top: 5px;
  padding-bottom: 5px;
  color: azure;
}

h3 {
  background-color: #8a703c;
  border-radius: 5px;
  padding-left: 2%;
  padding-top: 5px;
  padding-bottom: 5px;
  margin-left: 1%;
  color: azure;
}

h4 {
  background-color: #647c68;
  border-radius: 5px;
  padding-left: 2%;
  padding-top: 5px;
  padding-bottom: 5px;
  margin-left: 1.5%;
  color: azure;

}

::-webkit-scrollbar {
  width: 10px;
  height: 10px;
  background-color: rgba(0, 0, 0, 0.25);
}

.markdown-body .mermaid {
  display: flex;
  justify-content: center;
  background-color: antiquewhite;
}

.markdown-body .ContainImg {
  display: flex;
  justify-content: center;
}

/*定义滚动条轨道
 内阴影+圆角*/
::-webkit-scrollbar-track {
  /* box-shadow:inset 0 0 6px rgba(0,0,0,0.3); */
  background-color: rgba(0, 0, 0, 0.25);
}

/*定义滑块
 内阴影+圆角*/
::-webkit-scrollbar-thumb {
  border-radius: 5px;
  box-shadow: inset 0 0 6px rgba(0, 0, 0, 0.3);
  background-color: #555;
}
</style>
