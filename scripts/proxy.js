/**************************************************************************/
/* Copyright (c) 2025-present Felipe Lopes de Freitas.                    */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

// This file is a simple node.js script that connects to a websocket
// endpoint, echoing the data it receives from stdin to the socket,
// and similarly, echoing the data received from the socket to stdout.
// The process is terminated when stdin is closed from the writing
// side, or when the websocket connection is terminated from the server.
// Exit code is 0 in case of success and 1 in case of error.
// In case of error, it's written to stderr.
// All reads and writes are asynchronous and nonblocking, being
// multiplexed over a single thread in the node.js loop.

const fs = require('fs');
const ws = require('ws');

if (process.argv.length < 3) {
    console.log('Usage: '+process.argv[0]+' '+process.argv[1]+' <connectionUrl>')
    process.exit(1)
}

socket = new ws.WebSocket(process.argv[2]);

socket.on('error', error => {
    console.log(error)
    process.exit(1)
});

socket.on('open', () => {

    new Promise((resolve, reject) => {
        process.stdin.on('data', (data) => {
            socket.send(data);
        });
        process.stdin.on('error', (err) => {
            socket.close();
            process.stderr.write(err);
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
