import { createRouter, createWebHashHistory,createWebHistory} from 'vue-router'



const routes = [
    {
        path:'/',
        name:'config',
        title:'配置',
        component:()=>import('../config.vue')
    },
    {
        path:'/about',
        name:'about',
        title:'关于',
        component:()=>import('../about.vue')
    }
]


const router = createRouter({
    history: createWebHashHistory(),
    routes
})

export default router


