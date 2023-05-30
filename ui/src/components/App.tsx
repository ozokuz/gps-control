import { useState } from "react";
import Map from "./Map";
import WaypointList from "./WaypointList";
import type { Waypoint } from "../lib/types";

function WaypointForm() {}

export default function App() {
  const [waypoints, setWaypoints] = useState<Waypoint[]>([]);

  function submitForm(event: React.FormEvent<HTMLFormElement>) {
    event.preventDefault();
  }
  function addCoordinates() {}
  function startSelection() {}
  function confirmCoordinates() {}
  function deleteCoordinate() {}
  function cancelEditing() {}

  function addWaypoint(waypoint: Waypoint) {
    setWaypoints((v) => [...v, waypoint]);
  }

  return (
    <div className="flex flex-col md:flex-row">
      <div className="flex-grow">
        <Map waypoints={waypoints} addWaypoint={addWaypoint} />
        <div className="relative">
          <button className="bg-blue-500 text-white px-2 py-1 rounded absolute top-4 right-4 z-10 md:hidden">
            Show/Hide Coordinates List
          </button>
        </div>
      </div>

      <div className="hidden md:block w-full md:w-96 bg-gray-200 p-4">
        <h2 className="font-bold text-xl">Add GPS Coordinates:</h2>
        <form className="mt-4" onSubmit={submitForm}>
          <label htmlFor="coordinates" className="block mb-2">
            Enter coordinates (lat, lon):
          </label>
          <input
            type="text"
            id="coordinates"
            name="coordinates"
            className="w-full p-2 border border-gray-300 rounded mb-2"
            required
          />
        </form>

        <div className="space-x-2">
          <button
            type="button"
            onClick={addCoordinates}
            className="bg-blue-500 text-white px-4 py-2 rounded hover:bg-blue-700"
          >
            Add
          </button>
          <button
            type="button"
            onClick={startSelection}
            className="bg-green-500 text-white px-4 py-2 rounded hover:bg-green-700"
          >
            Select
          </button>
          <button
            type="button"
            onClick={confirmCoordinates}
            className="bg-green-500 text-white px-4 py-2 rounded hover:bg-green-700 hidden"
          >
            Confirm
          </button>
          <button
            type="button"
            onClick={deleteCoordinate}
            className="bg-red-500 text-white px-4 py-2 rounded hover:bg-red-700 hidden"
          >
            Delete
          </button>
          <button
            type="button"
            onClick={cancelEditing}
            className="bg-gray-500 text-white px-4 py-2 rounded hover:bg-gray-700 hidden"
          >
            Cancel
          </button>
        </div>

        <h3 className="font-bold text-xl mt-4">Coordinates List:</h3>
        <WaypointList waypoints={waypoints} />
      </div>
    </div>
  );
}
