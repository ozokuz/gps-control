import { useState, useEffect } from "react";

export function useConnectionStatus() {
  const [isConnected, setIsConnected] = useState(false);

  const toggleConnected = () => {
    setIsConnected((v) => !v);
    setTimeout(toggleConnected, (Math.floor(Math.random() * 5) + 1) * 500);
  };

  useEffect(() => {
    toggleConnected();
  }, []);

  return isConnected;
}
