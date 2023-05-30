import { useState, useEffect, useRef } from "react";

interface Message {
  message: string;
}

export default function Test() {
  const [message, setMessage] = useState<Message>();
  const wsRef = useRef<WebSocket>();

  useEffect(() => {
    wsRef.current = new WebSocket("ws://localhost:8080/ws");

    wsRef.current.addEventListener("open", () => {
      wsRef.current!.send(JSON.stringify({ message: "hahaa" }));
    });
    wsRef.current.addEventListener("message", (event) => {
      setMessage(JSON.parse(event.data));
    });
  }, []);

  return <div>{message ? message.message : "nothing here"}</div>;
}
