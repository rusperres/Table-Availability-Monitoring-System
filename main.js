const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

const portOne = new SerialPort({
    path: 'COM5',
    baudRate: 9600,
    dataBits: 8,
    parity: 'none',
    stopBits: 1,
    flowControl: false
});

const portTwo = new SerialPort({
    path: 'COM7',
    baudRate: 9600,
    dataBits: 8,
    parity: 'none',
    stopBits: 1,
    flowControl: false
});


const parserOne = portOne.pipe(new ReadlineParser({ delimiter: '\r\n' }));
const parserTwo = portTwo.pipe(new ReadlineParser({ delimiter: '\r\n' }));

parserOne.on('open', function (open) {
    console.log("Open Connection");
})

parserTwo.on('open', function (open) {
    console.log("Open Connection");
})

parserOne.on('data', function (data) {
    setTimeout(function () {
        dataNum = parseInt(data);
        if (dataNum < 2000) {
            portTwo.write("1");
        } else {
            portTwo.write("2");
        }
    }, 1);
});









//the server part

const express = require('express');
const app = express();
const server = require('http').createServer(app);
const WebSocket = require('ws');
const fs = require('fs');

const wss = new WebSocket.Server({ server: server });

const html = fs.readFileSync('files/index.html', 'utf-8');

wss.on('connection', function connection(ws) {
    console.log('New client connected')
    ws.send(html)
});

app.get('/', (req, res) => {
    res.send(html)
})

server.listen(8000, () => {
    console.log('Listening on port: 8000')
})
