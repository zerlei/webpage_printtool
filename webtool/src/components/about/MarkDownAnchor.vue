<template>
    <!-- <div id="blog_anchor_header"></div> -->
    <n-scrollbar style="height: 88%;" ref="scrollBarAnchor">
        <div id="blog_anchor_menu"></div>
    </n-scrollbar>
    <!-- <img id="blog_anchor_img" src="../../assets/images/BackToTop.svg" @click="closeAnchor" /> -->
</template>

<script setup >
//you dont't love me 
import {ref, onMounted } from 'vue'
import { NScrollbar } from 'naive-ui'

const scrollBarAnchor = ref(null)
function changeDivClass(newDiv) {

    anchorNode = newDiv
    let preChoiced = document.getElementsByClassName("choicedDiv")

    if (preChoiced == null || preChoiced.length == 0) {
    } else {
        preChoiced[0].classList.remove('choicedDiv')
    }
    newDiv.classList.add('choicedDiv')
    scrollBarAnchor.value.scrollTo({ top: newDiv.offsetTop })
}
function closeAnchor() {
    emit('anchorClose')
}
onMounted(() => {
    menu_ob = document.getElementById("blog_anchor_menu")
    menu_ob.addEventListener('click', (e) => {
        emit('anchorDo', e.path[0].id.replace(/a/,'b'))
        changeDivClass(e.path[0])
    }, true)
})

const emit = defineEmits(['anchorClose', 'anchorDo'])
let menu_ob = null
let PosinfoStr = ''
let isDown = false
let anchorNode

function updateAnchor(anchor) {
    menu_ob.innerHTML = ''
    let currentArr = []
    currentArr.push({
        ob: menu_ob,
        rate: 0
    })
    anchor.forEach(e => {
        const rate = e.tagName
        const info = e.value

        let newOb = document.createElement('div')
        newOb.id= e.id.replace(/b/,'a')
        newOb.innerText = info;
        if (rate == currentArr[currentArr.length - 1].rate) {
            for (let i = currentArr.length - 1; i >= 0; i--) {
                if (currentArr[i].rate < rate) {
                    currentArr[i].ob.appendChild(newOb)
                    break
                }
            }

        } else if (rate > currentArr[currentArr.length - 1].rate) {
            currentArr[currentArr.length - 1].ob.appendChild(newOb)
           
            } else {
            for (let i = currentArr.length - 1; i >= 0; i--) {
                if (currentArr[i].rate < rate) {
                    currentArr[i].ob.appendChild(newOb)
                    break
                }
            }
        }
        currentArr.push({
            ob: newOb,
            rate: rate
        })
    });

    anchorNode = menu_ob.firstChild
}
function recureGetCurrentAnchor() {

    while (1) {
        if (anchorNode.id == PosinfoStr) {
            changeDivClass(anchorNode)
            return
        } else {
            if (anchorNode.nodeName == 'DIV' && anchorNode.children.length > 0) {

                if (anchorNode.firstChild == null && isDown) {
                    console.log(anchorNode)
                    console.log(PosinfoStr)
                    console.log("firstChild")
                    return
                }
                if (anchorNode.lastChild == null && isDown) {
                    console.log(anchorNode)
                    console.log(PosinfoStr)
                    console.log("lastChild")
                    return
                }
                if (isDown) {
                    anchorNode = anchorNode.firstChild
                } else {
                    anchorNode = anchorNode.lastChild
                }
                return recureGetCurrentAnchor()
            } else if (anchorNode.id == PosinfoStr) {
                changeDivClass(anchorNode.parentNode)
                return
            }
        }
        if (isDown) {
            //lastChild 可以是一个 #Text nextSibling 最后一个肯定是null
            if (anchorNode.nextSibling == null) {
                break
            } else {
                anchorNode = anchorNode.nextSibling
            }
        } else {
            if (anchorNode.previousSibling == null) {
                break
            } else {
                anchorNode = anchorNode.previousSibling
                
            }
        }
    }

    if (anchorNode.parentNode) {
        if (anchorNode.parentNode.nextSibling == null && isDown) {

            console.log(anchorNode)
            console.log(PosinfoStr)
            console.log("nextSibling")
            return
        }
        if (anchorNode.parentNode.previousSibling == null && !isDown) {
            console.log(anchorNode)
            console.log(PosinfoStr)
            console.log("previousSibling")
            return
        }
        if (isDown) {
            anchorNode = anchorNode.parentNode.nextSibling
        } else {
            anchorNode = anchorNode.parentNode.previousSibling
        }
        return recureGetCurrentAnchor()
    }
}
function setCurrentPos(PosInfo, isDown_) {
    PosinfoStr = PosInfo.replace(/b/,'a')
    isDown = isDown_
    if(anchorNode == null) {
        return
    }
    recureGetCurrentAnchor()
}

defineExpose({
    updateAnchor,
    setCurrentPos
})
</script>
<style>

#blog_anchor_menu {
    /* margin-left: 20px; */
    margin-right: 20px;
    margin-top: 30px;
}
#blog_anchor_menu div {
    padding-left: 20px;
    color: wheat;
    user-select: none;
    border-radius: 5px;
    padding-top: 10px;
    font-size: 15px;
}
.choicedDiv {
    background-color: #233633;
    color: #7fe7c4 !important;
}
#blog_anchor_menu div:hover {
    cursor: pointer;
}
</style>