import { useRef, useEffect, type ReactNode } from "react";
import { MapContainer, TileLayer, Marker, useMapEvent } from "react-leaflet";
import type { LatLngExpression, LeafletMouseEvent } from "leaflet";
import type { Waypoint } from "../lib/types";

interface MapProps {
  waypoints: Waypoint[];
  addWaypoint: (waypoint: Waypoint) => void;
}

const tileLayerUrl =
  "https://dev.{s}.tile.openstreetmap.fr/cyclosm/{z}/{x}/{y}.png";
const attribution =
  '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors';

const defaultCenter: Waypoint = {
  lat: 60.18303504726024,
  lon: 24.927493265440646,
};

function waypointToCenter(waypoint: Waypoint): LatLngExpression {
  return [waypoint.lat, waypoint.lon];
}

function MapWrap({
  addWaypoint,
}: {
  addWaypoint: (Waypoint: Waypoint) => void;
}) {
  useMapEvent("dblclick", (event) => {
    const { lat, lng: lon } = event.latlng;
    addWaypoint({ lat, lon });
  });

  return null;
}

export default function Map({ waypoints, addWaypoint }: MapProps) {
  const mapRef = useRef<any>(null);
  const hasWaypoints = waypoints.length > 0;
  const center = waypointToCenter(hasWaypoints ? waypoints[0] : defaultCenter);

  // loadWaypoints();

  return (
    <MapContainer
      ref={mapRef}
      center={center}
      zoom={13}
      doubleClickZoom={false}
      className="h-screen"
    >
      <MapWrap addWaypoint={addWaypoint} />
      <TileLayer attribution={attribution} url={tileLayerUrl} maxZoom={18} />
      {waypoints.map((waypoint, i) => (
        <Marker position={[waypoint.lat, waypoint.lon]} key={i} draggable />
      ))}
    </MapContainer>
  );
}
