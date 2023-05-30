import process from "node:process";
import fastify from "fastify";
import websocket from "@fastify/websocket";
import { SerialPort } from "serialport";

/* const port = new SerialPort({
  path: "/dev/tty-usbserial1",
  baudRate: 9600,
}); */

const app = fastify({ logger: true });
await app.register(websocket);

app.get("/ws", { websocket: true }, (connection) => {
  connection.socket.on("message", (message: Buffer) => {
    console.log(JSON.parse(message.toString()));
    connection.socket.send(JSON.stringify({ message: "hi from server" }));
  });
});

await app.listen({ port: 8080 });

process.on("SIGTERM", () => {
  app.close().then(() => {
    process.exit(0);
  });
});
