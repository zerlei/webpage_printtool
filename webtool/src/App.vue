<script setup>

import { darkTheme, NScrollbar, NConfigProvider, NMessageProvider, NDialogProvider } from 'naive-ui'

import Settings from './components/printsettings/settings.vue'

</script>

<template>
  <NConfigProvider :theme="darkTheme">
    <NMessageProvider>
      <NDialogProvider>
        <div id="container">
          <n-scrollbar id="mainregion">
            <Settings />
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
  padding-left: 1%;
  padding-right: 1%;
}

.markdown-body p {
  font-family: cursive;
  font-size: 19px;
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
