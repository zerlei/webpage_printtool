<template>
    <div style="height: 100vh;width: 100%;">
        <div id="container">
            <div id="regionmid">
                <MarkdownBody ref="mdbody" @currentAnchor="onCurrentAnchor" @mdRenderAnchor="onMdRenderAnchor" />
            </div>
            <div id="regionright" class="regionside">
                <div id="regionright_bar" class="bar">
                    <div class="sign"></div>
                </div>
                <div id="blog_anchor" v-show="isShowAnchor">
                    <MarkdownAnchor ref="mdAnchor" @anchorDo="onAnchorDo" @anchorClose="onAnchorClose" />
                </div>
            </div>
        </div>
    </div>
</template>

<script setup>
import { onMounted, ref } from 'vue'
import MarkdownBody from './MarkdownBody.vue';
import MarkdownAnchor from './MarkDownAnchor.vue';

import readme from './readme.js'
//#region  交互信息

const mdbody = ref(null)
const mdAnchor = ref(null)
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
</style>