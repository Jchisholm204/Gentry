const TCP_PORT = 5566; 
// Reference: https://nodejs.org/dist/latest-v6.x/docs/api/net.html
var net = require('net');

net.createServer(client => {
	console.log('Connection established with ' + client.remoteAddress); // Works as a print on terminal

	client.on('close', () => { // when client close the connection execute the next line
		console.log('Connection closed');
	});

	client.on('error', err => { // when client got an error execute the next line
		console.log('Connection error ' + err);
	});

	client.on('data', data => { // when client send some data execute the next line
		//console.log('Received data: ' + data); // Works as a print on terminal
        console.log(data.buffer);
		//let dock = data[0];
		console.log("Dock: %s", String.fromCharCode(data[0].toString()));
		console.log("Slip Number: " + data.readUInt8(1));
		console.log("Voltage", data.readFloatLE(4));
		console.log("Current: ", data.readFloatLE(8));
		console.log("Time Elapsed: ", data.readUint32LE(12));
		console.log("Energy Consumed (W): ", data.readFloatLE(16));
		console.log("Sending OK reply..");
		try {
			client.write(Buffer.from("OK")); // send the same data as a replay
			console.log('Data replied');
		} catch (e) { 
			console.error('Send error: ' + e); // Print the actual error
		}
	});
}).listen(TCP_PORT); 