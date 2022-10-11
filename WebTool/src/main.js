import { createApp } from 'vue'
import App from './App.vue'
import 'github-markdown-css/github-markdown-dark.css'
import router from "./components/router";
import "./assets/styles/font.css"
import "./assets/styles/code.css"
createApp(App).use(router).mount('#app')
