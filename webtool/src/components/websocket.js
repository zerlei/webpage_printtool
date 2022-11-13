
class Websoc {
    #listener
    #sender
    #websocket
    #reopentimer
    #senderTem
    constructor() {
        //Id,Msgtype,callback
        this.#listener = []
        //Id,resolve,time
        this.#sender = []
        this.#senderTem = []
        setInterval(() => {
            let now = Date.now()
            for (let i = 0; i < this.#sender.length;) {
                if (now - this.#sender[i].time > 120000) {

                    let resp = {
                        IsSuccess: false,
                        Result: {
                            Message: "超时！"
                        }
                    }
                    this.#sender[i].resolve(resp)
                    this.#sender.splice(i, 1)
                } else {
                    ++i;
                }
            }
        }, 120000)

        this.#websocket = new WebSocket('ws://127.0.0.1:8847')
        this.#websocket.onopen = event => {
            console.warn("websocket connected!")
            clearInterval(this.#reopentimer)
            while (this.#senderTem.length > 0) {
                this.#websocket.send(JSON.stringify(this.#senderTem.pop()))
            }

        }
        this.#websocket.onmessage = event => {
            this.#newMsg(event.data)
        }
        this.#websocket.onclose = event => {
            this.#reopentimer = setInterval(() => {
                this.#websocket = new WebSocket('ws://127.0.0.1:8847')
                console.warn('reopen websocket!')
            }, 30000)
        }
        this.#websocket.onerror = e => {
            if (this.#websocket.readyState) {
                //blab;a
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
    send(msg) {
        if (!msg.Id) {
            msg.Id = `${Date.now()}`
        }
        if (this.#websocket.readyState == 1) {
            this.#websocket.send(JSON.stringify(msg))
        } else {
            this.#senderTem.push(msg)
        }

        return new Promise(resolve => {
            this.#sender.push({
                id: msg.Id,
                resolve: resolve,
                time: Date.now()

            })
        })
    }
    #newMsg(data) {
        try {

            let ob = JSON.parse(data)
            //msg
            for (let i = 0; i < this.#sender.length;) {
                if (this.#sender[i].id == ob.Id) {
                    this.#sender[i].resolve(ob)
                    this.#sender.splice(i, 1)
                } else {
                    ++i;
                }
            }
            //listener
            for (let i = 0; i < this.#listener.length; ++i) {
                if (this.#listener[i].msgtype == ob.msgtype) {
                    this.#listener[i].callback(ob)
                }
            }

        }
        catch (e) {
            console.warn(e)

        }


    }

}

let ServerNet = new Websoc()

export default ServerNet