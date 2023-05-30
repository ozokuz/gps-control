import { useConnectionStatus } from "../lib/connectionStatus";

export default function StatusIndicator() {
  const isConnected = useConnectionStatus();

  return (
    <p className={isConnected ? "text-green-600" : "text-red-600"}>
      ⬤ {isConnected ? "Connected" : "Not Connected"}
    </p>
  );
}
