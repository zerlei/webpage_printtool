import { createRouter, createWebHashHistory,createWebHistory} from 'vue-router'



const routes = [
    {
        path:'/',
        name:'setting',
        title:'配置',
        component:()=>import('../printsettings/settings.vue')
    },
    {
        path:'/about',
        name:'about',
        title:'关于',
        component:()=>import('../about/about.vue')
    }
]


const router = createRouter({
    history: createWebHashHistory(),
    routes
})

export default router


