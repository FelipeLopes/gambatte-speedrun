const fs = require('fs');
const ws = require('ws');

socket = new ws.WebSocket('ws://localhost:8080/host/abc');

fs.open('/home/felipe/lua_to_ws_host', 'r', (err, fd) => {
    const stream = fs.createReadStream(null, {fd});

    function sendStreamToWs (stream) {
        return new Promise((resolve, reject) => {
            stream.on('data', (data) => {
                socket.send(data)
            });
            stream.on('error', (err) => reject(err));
            stream.on('end', () => resolve());
        })
    };

    sendStreamToWs(stream);
});

fs.open('/home/felipe/ws_to_lua_host', 'w', (err, fd) => {
    const stream = fs.createWriteStream(null, {fd});

    socket.addEventListener('message', event => {
        stream.write(event.data)
    });
});

