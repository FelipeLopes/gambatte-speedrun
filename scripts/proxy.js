const fs = require('fs');
const ws = require('ws');

if (process.argv.length < 3) {
    console.log('Usage: '+process.argv[0]+' '+process.argv[1]+' <connectionUrl>')
    process.exit(1)
}

socket = new ws.WebSocket(process.argv[2]);

socket.on('error', error => {
    console.log(error)
});

socket.on('open', () => {

    new Promise((resolve, reject) => {
        process.stdin.on('data', (data) => {
            socket.send(data);
        });
        process.stdin.on('error', (err) => {
            socket.close();
            process.exit(1);
        });
        process.stdin.on('end', () => {
            socket.close();
            process.exit(0);
        });
    });

    socket.addEventListener('message', event => {
        process.stdout.write(event.data)
    });

    socket.addEventListener('error', event => {
        process.stderr.write(error);
        process.exit(1);
    });

    socket.addEventListener('close', event => {
        process.exit(0);
    });

});
