<template>
    <div style="height: 100%;overflow: auto;" id="blog_scroll">
        <div id="blog_MdBody" class="markdown-body" style="padding: 20px 20px 20px 20px;" v-html="Mdcontent"></div>
    </div>
</template>

<script setup>

import hljs from 'highlight.js'
import MarkdownIt from 'markdown-it'
import mermaid from 'mermaid/dist/mermaid'
import { ref, nextTick, onActivated, onDeactivated, onMounted } from 'vue'
let scrollBarEle
const md = new MarkdownIt(
    {
        highlight: function (str, lang) {
            if (lang == 'mermaid') {
                // mermaidCount++
                // mermaidCodeS.push({code:str,index:mermaidCount})

                return `<div class="mermaid">${str}</div>`
            }
            // if(lang== 'plantuml') {
            //     return `<div class="hi">${str}</div>`
            // }

            if (lang && hljs.getLanguage(lang)) {
                try {
                    return hljs.highlight(str, { language: lang }).value;
                } catch (__) { }
            }

            return ''; // use external default escaping
        }
    }
)
const Mdcontent = ref(null)
let anchorArr = []
let isDown = true
let scrollTimer = null
const scrollTimerFun = () => {
    let MdDiv = document.getElementById("blog_MdBody")
    let child_ = MdDiv.firstChild
    let currentAnchor = ''
    let minoffsetTop = 1000

    while (child_ != MdDiv.lastChild) {

        if (['H1', 'H2', 'H3', 'H4', 'H5'].includes(child_.tagName)) {
            if (Math.abs(child_.getBoundingClientRect().top) < minoffsetTop) {
                minoffsetTop = Math.abs(child_.getBoundingClientRect().top)
                currentAnchor = child_.id
            }
        }
        child_ = child_.nextSibling
    }
    emit('currentAnchor', currentAnchor, isDown)
}
onMounted(() => {
    scrollBarEle = document.getElementById("blog_scroll")
    scrollBarEle.addEventListener('scroll', ScrollAction, true)
})
async function updateContent(MDText) {

    Mdcontent.value = md.render(MDText)
    await nextTick()

    let MdDiv = document.getElementById("blog_MdBody")
    anchorArr = []
    let child_ = MdDiv.firstChild
    let anchorIndex = 0
    while (child_) {

        if (child_.nodeName === "H1") {
            let anchorOb = {}
            const idStr = child_.innerHTML
            child_.id = `b_${anchorIndex}`
            anchorOb.tagName = 1
            anchorOb.value = idStr
            anchorOb.id = `b_${anchorIndex}`
            anchorIndex++

            anchorArr.push(anchorOb)
        }
        else if (child_.nodeName === "H2") {
            let anchorOb = {}
            const idStr = child_.innerHTML
            child_.id = `b_${anchorIndex}`
            anchorOb.tagName = 2
            anchorOb.value = idStr
            anchorOb.id = `b_${anchorIndex}`
            anchorIndex++

            anchorArr.push(anchorOb)

        } else if (child_.nodeName === "H3") {
            let anchorOb = {}
            const idStr = child_.innerHTML
            child_.id = `b_${anchorIndex}`
            anchorOb.tagName = 3
            anchorOb.value = idStr
            anchorOb.id = `b_${anchorIndex}`
            anchorIndex++

            anchorArr.push(anchorOb)

        } else if (child_.nodeName === "H4") {
            let anchorOb = {}
            const idStr = child_.innerHTML
            child_.id = `b_${anchorIndex}`
            anchorOb.tagName = 4
            anchorOb.value = idStr
            anchorOb.id = `b_${anchorIndex}`
            anchorIndex++

            anchorArr.push(anchorOb)

        } else if (child_.nodeName === "H5") {
            let anchorOb = {}
            const idStr = child_.innerHTML
            child_.id = `b_${anchorIndex}`
            anchorOb.tagName = 5
            anchorOb.value = idStr
            anchorOb.id = `b_${anchorIndex}`
            anchorIndex++
            anchorArr.push(anchorOb)
        } else if (child_.nodeName === "P") {
            let _child_ = child_.firstChild
            while (_child_) {

                //console.log(_child_.nodeName)

                if (_child_.nodeName === "IMG") {
                    child_.classList.add('ContainImg')
                    break
                }
                _child_ = _child_.nextSibling

            }
            //
        }

        child_ = child_.nextSibling;
    }

    if (scrollBarEle.offsetHeight < MdDiv.offsetHeight) {
        scrollBarEle.onwheel = ScrollAction
    } else {
        scrollBarEle.onwheel = null
    }
    await nextTick()
    mermaid.init()
    emit('mdRenderAnchor', anchorArr)
}

let lastScrollTop = 0;
function ScrollAction(e) {
    var st = scrollBarEle.scrollTop
    if (st > lastScrollTop) {
        isDown = true
    } else if (st < lastScrollTop) {
        isDown = false
    } else {
        return
    }
    lastScrollTop = st <= 0 ? 0 : st
    clearTimeout(scrollTimer)
    scrollTimer = setTimeout(scrollTimerFun, 500)
}
function AnchorIt(AnchorStr) {
    let anchorOb = document.getElementById(AnchorStr)

    scrollBarEle.scrollTo({ top: anchorOb.offsetTop })
}
const emit = defineEmits(["mdRenderAnchor", 'currentAnchor'])

defineExpose({
    updateContent,
    AnchorIt
})
</script>

<style>

</style>