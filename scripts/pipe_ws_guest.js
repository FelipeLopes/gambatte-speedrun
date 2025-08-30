const fs = require('fs');
const ws = require('ws');

socket = new ws.WebSocket('ws://localhost:8080/guest/abc');

fs.open('/home/felipe/lua_to_ws_guest', 'r', (err, fd) => {
    const stream = fs.createReadStream(null, {fd});

    function sendStreamToWs (stream) {
        return new Promise((resolve, reject) => {
            stream.on('data', (data) => {
                console.log('from pipe: '+data.toString('hex'))
                socket.send(data)
            });
            stream.on('error', (err) => reject(err));
            stream.on('end', () => resolve());
        })
    };

    sendStreamToWs(stream);
});

fs.open('/home/felipe/ws_to_lua_guest', 'w', (err, fd) => {
    const stream = fs.createWriteStream(null, {fd});

    socket.addEventListener('message', event => {
        console.log('from sock: ' +event.data.toString('hex'))
        stream.write(event.data)
    });
});

