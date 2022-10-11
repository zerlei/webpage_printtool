<template>
    <div style="height: 100vh;width: 100%;">
        <!-- <img
            :src="menuSrc"
            id="blog_menu"
            :class="{ blog_menu_open: isShowMenu, blog_menu_close: !isShowMenu }"
            @click="LeftMenuShowOrHide"
        />
        <img
            id="blog_anchor_img_show"
            src="../assets/images/DownToBottom.svg"
            @click="onAnchorClose"
            v-show="isAnchorHide"
        />-->
        <div id="container">
            <div id="regionmid">
                <MarkdownBody
                    ref="mdbody"
                    @currentAnchor="onCurrentAnchor"
                    @mdRenderAnchor="onMdRenderAnchor"
                />
            </div>
            <div id="regionright" class="regionside">
                <div id="regionright_bar" class="bar">
                    <div class="sign"></div>
                </div>
                <div id="blog_anchor" v-show="isShowAnchor">
                    <MarkdownAnchor
                        ref="mdAnchor"
                        @anchorDo="onAnchorDo"
                        @anchorClose="onAnchorClose"
                    />
                </div>
            </div>
        </div>
    </div>
</template>

<script setup>
import { onMounted, ref } from 'vue'
import MarkdownBody from './MarkdownBody.vue';
import MarkdownAnchor from './MarkdownAnchor.vue';

import readme from './readme.js'
//#region  交互信息

const leftMenu = ref(null)
const mdbody = ref(null)
const mdAnchor = ref(null)
const isShowMenu = ref(true)
const isShowAnchor = ref(true)


onMounted(async () => {
    const rigth_bar = document.getElementById("regionright_bar")
    const right_region = document.getElementById("regionright")

    rigth_bar.addEventListener("mousedown", e => {
        document.documentElement.style.userSelect = "none"
        const startX_r = e.clientX
        const startWidth_r = parseInt(getComputedStyle(right_region, null).width)

        document.onmousemove = e => {
            right_region.style.width = `${startWidth_r + startX_r - e.clientX}px`
        }

        document.onmouseup = e => {
            document.onmousemove = null
            document.onmouseup = null
            document.documentElement.style.userSelect = "text"
            if (parseInt(getComputedStyle(right_region, null).width) < 30) {
                right_region.style.width = "17px"
                isShowAnchor.value = false
            } else {
                isShowAnchor.value = true
            }
        }
    })


    mdbody.value.updateContent(readme)


    //#endregion
})


function onAnchorDo(currentAnchor) {
    mdbody.value.AnchorIt(currentAnchor)
}
function onAnchorClose() {
    isAnchorHide.value = !isAnchorHide.value
}

function onCurrentAnchor(currentAchor, isDown) {
    mdAnchor.value.setCurrentPos(currentAchor, isDown)
}

function onMdRenderAnchor(anchorArr) {

    mdAnchor.value.updateAnchor(anchorArr)

}
//#endregion
</script>
<style>
#container {
    display: flex;
    height: 100%;
}
.regionside {
    height: 100%;
    width: 15%;
    position: relative;
    min-width: 17px;
    background-color: rgba(20, 20, 20, 1);
}
.bar {
    width: 17px;
    height: 100%;
    position: absolute;
    cursor: col-resize;
    box-shadow: 0 0 2px rgba(0, 0, 0, 0.6);
    display: flex;
    justify-content: center;
    align-items: center;
    z-index: 999;
}
#regionright_bar {
    left: 0;
    top: 0;
}
.sign {
    width: 3px;
    height: 20px;
    border-left: 2px solid #00d780;
    border-right: 2px solid #00d780;
}
#regionmid {
    flex: 1;
    height: 100%;
    background-color: #0d1117;
    padding-left: 0%;
    width: auto;
}
/* .blog_menu_open {
    position: absolute;
    top: 50%;
    left: 19%;
    z-index: 10;
    width: 20px;
    height: 20px;
    color: #00d780;
    background-color: rgba(80, 80, 85, 0.6);
    text-align: center;
    line-height: 20px;
    border-radius: 20px;
}
.blog_menu_close {
    position: absolute;
    top: 50%;
    left: 0%;
    z-index: 10;
    width: 20px;
    height: 20px;
    color: #00d780;
    background-color: rgba(80, 80, 85, 0.6);
    text-align: center;
    line-height: 20px;
    border-radius: 20px;
}
#blog_menu:hover {
    cursor: Pointer;
}

#blog_leftmenu {
    height: 100%;
    background-color: #18181c;
} */

/* #blog_anchor {
    position: absolute;
    top: 5px;
    right: 5px;
    background-color: rgba(35, 35, 35, 0.6);
    z-index: 78;
    width: 250px;
    height: 400px;
    border: 2px solid #494949;
    border-radius: 20px;
} */
/* .blog_body_menu_open {
    height: 100vh;
    background-color: #0d1117;
    padding-left: 20%;
}
.blog_body_menu_close {
    height: 100vh;
    background-color: #0d1117;
    padding-left: 0%;
}
#blog_anchor_img_show {
    width: 25px;
    height: 25px;
    border-radius: 10px;
    position: absolute;
    top: 30px;
    right: 20px;
    z-index: 99;
    background-color: #494949;
}
#blog_anchor_img_show:hover {
    cursor: pointer;
} */
</style>