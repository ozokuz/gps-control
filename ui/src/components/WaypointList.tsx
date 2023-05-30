import type { Waypoint } from "../lib/types";

function WaypointItem({ waypoint }: { waypoint: Waypoint }) {
  return (
    <li className="flex flex-col md:flex-row items-center justify-between bg-gray-200 p-2 rounded mb-2">
      <span>
        {waypoint.lat}, {waypoint.lon}
      </span>
      <div>
        <button>Edit</button>
      </div>
    </li>
  );
}

interface WaypointListProps {
  waypoints: Waypoint[];
}

export default function WaypointList(props: WaypointListProps) {
  return (
    <ul className="mt-4">
      {props.waypoints.map((waypoint, i) => (
        <WaypointItem key={i} waypoint={waypoint} />
      ))}
    </ul>
  );
}
