class Websoc {
    #listener
    #sender
    #websocket
    #reopentimer
    #senderTem
    #ids
    //在这里打断点可能会导致debug错误，然后浏览器打不开页面， 这是为啥？
    constructor() {
        //Id,Msgtype,callback
        this.#listener = []
        //Id,resolve,time
        this.#sender = []
        this.#senderTem = []
        this.#ids = []
        this.#reopentimer = null
        this.#websocket = null
        setInterval(() => {
            let now = Date.now()
            for (let i = 0; i < this.#sender.length;) {
                if (now - this.#sender[i].time > 120000) {
                    let resp = {
                        IsSuccess: false,
                        Message: "超时！"
                    }
                    this.#backId(this.#sender[i].id)
                    this.#sender[i].resolve(resp)
                    this.#sender.splice(i, 1)

                    //TODO Should remove #senderTem?
                } else {
                    ++i;
                }
            }
        }, 120000)
        if (import.meta.env.DEV) {
            this.#websocket = new WebSocket(`ws://127.0.0.1:8847`)
        } else {
            this.#websocket = new WebSocket(`ws://${window.location.host}`)
        }
        this.#websocket.onopen = event => {
            console.warn("websocket connected!")
            while (this.#senderTem.length > 0) {
                this.#websocket.send(JSON.stringify(this.#senderTem.pop()))
            }

        }
        this.#websocket.onmessage = event => {
            this.#newMsg(event.data)
        }
        this.#websocket.onclose = event => {
            this.#webSocOnClose(event)
        }
        this.#websocket.onerror = e => {
            if (this.#websocket.readyState) {
                //bla bla
            }
        }
    }
    listen(id, msgtype, callback) {
        this.#listener.push({
            id: id,
            msgtype: msgtype,
            callback: callback
        })
    }
    unlisten(id) {
        for (let i = 0; i < this.#listener.length;) {
            if (this.#listener[i].id == id) {
                this.#listener.splice(i, 1)
            } else {
                i++
            }
        }
    }
    #getId() {
        let i = 0
        for (; i < this.#ids.length; ++i) {
            if (!this.#ids[i].isUsed) {
                this.#ids[i].isUsed = true
                return this.#ids[i].id
            }
        }
        this.#ids.push({
            id: `${i}`,//use str
            isUsed: true
        })
        return `${i}`
    }
    #backId(id) {
        for (let i = 0; i < this.#ids.length; ++i) {
            if (this.#ids[i].id == id) {
                this.#ids[i].isUsed = false
                return
            }
        }
    }
    send(msg) {
        msg.Id = this.#getId()
        return new Promise(resolve => {
            this.#sender.push({
                id: msg.Id,
                resolve: resolve,
                time: Date.now()
            })

            if (this.#websocket.readyState == 1) {
                this.#websocket.send(JSON.stringify(msg))
            } else {
                this.#senderTem.push(msg)
            }
        })


    }
    #newMsg(data) {
        try {

            let ob = JSON.parse(data)
            //msg
            for (let i = 0; i < this.#sender.length;) {
                if (this.#sender[i].id == ob.Id) {
                    this.#backId(ob.Id)
                    this.#sender[i].resolve(ob)
                    this.#sender.splice(i, 1)
                } else {
                    ++i;
                }
            }
            //listener
            for (let i = 0; i < this.#listener.length; ++i) {
                if (this.#listener[i].msgtype == ob.MsgType) {
                    this.#listener[i].callback(ob)
                }
            }

        }
        catch (e) {
            console.warn(e)

        }


    }
    #reOpen() {
        console.warn('reopen websocket!')
        if (this.#websocket.readyState == 1) {
            console.warn("had open!")
            return
        }
        this.#websocket.close()
        this.#websocket = new WebSocket(`ws://${window.location.host}`)
        this.#websocket.onopen = e1_ => {
            console.warn("websocket connected!")
            while (this.#senderTem.length > 0) {
                this.#websocket.send(JSON.stringify(this.#senderTem.pop()))
            }

        }
        this.#websocket.onmessage = e2_ => {
            this.#newMsg(e2_.data)
        }
        this.#websocket.onclose = e3_ => {
            //when connection failed the function will be call auto
            this.#webSocOnClose(e3_)
        }

    }
    #webSocOnClose(e_) {
        setTimeout(() => {
            this.#reOpen()
        }, 10000)

    }

}

let ServerNet = new Websoc()

export default ServerNet