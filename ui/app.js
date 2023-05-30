let map = null;
let waypoints = [];
let selectedWaypoints = [];

function initMap() {
  const defaultCenter = [60.18303504726024, 24.927493265440646];
  const defaultZoom = 13;

  const center =
    waypoints.length > 0 ? [waypoints[0].lat, waypoints[0].lon] : defaultCenter;

  map = L.map("map", { doubleClickZoom: false }).setView(center, defaultZoom);

  L.tileLayer("https://dev.{s}.tile.openstreetmap.fr/cyclosm/{z}/{x}/{y}.png", {
    attribution:
      '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors',
    maxZoom: 18,
  }).addTo(map);
  loadWaypoints();
}

function addCoordinates() {
  const input = document.getElementById("coordinates");
  const coords = input.value.split(",");

  if (coords.length !== 2) {
    alert("Invalid input format. Please use 'latitude,longitude' format.");
    return;
  }

  const lat = parseFloat(coords[0].trim());
  const lon = parseFloat(coords[1].trim());

  if (isNaN(lat) || isNaN(lon)) {
    alert("Invalid coordinates. Please input valid numbers.");
    return;
  }

  const numberIcon = L.divIcon({
    className: "number-icon",
    html: `<img src="https://cdn.rawgit.com/pointhi/leaflet-color-markers/master/img/marker-icon-2x-blue.png" width="25" height="41"><div class="number-label">${
      waypoints.length + 1
    }</div>`,
    iconSize: [25, 41],
    iconAnchor: [12, 41],
    popupAnchor: [1, -34],
  });

  const marker = L.marker([lat, lon], {
    icon: numberIcon,
  }).addTo(map);

  waypoints.push({ lat, lon, marker });

  updateCoordinatesList();
  input.value = "";
  saveWaypoints();
}

function addCoordinatesFromMap(event) {
  const lat = event.latlng.lat;
  const lon = event.latlng.lng;
  document.getElementById("coordinates").value = `${lat}, ${lon}`;
  addCoordinates();
}

function updateCoordinatesList() {
  const list = document.getElementById("coordinates-list");
  list.innerHTML = "";

  waypoints.forEach((waypoint, index) => {
    const listItem = document.createElement("div");
    listItem.className =
      "flex flex-col md:flex-row items-center justify-between bg-gray-200 p-2 rounded mb-2";

    const coordinatesText = document.createElement("span");
    coordinatesText.className = "flex-grow truncate";
    coordinatesText.textContent = `${index + 1}. ${waypoint.lat}, ${
      waypoint.lon
    }`;
    coordinatesText.onclick = () => {
      editCoordinates(index);
    };
    listItem.appendChild(coordinatesText);

    const buttonsDiv = document.createElement("div");
    buttonsDiv.className = "flex space-x-2 mt-2 md:mt-0";

    const moveUpButton = document.createElement("button");
    moveUpButton.textContent = "Up";
    moveUpButton.className =
      "bg-blue-500 text-white px-2 py-1 rounded hover:bg-blue-700";
    moveUpButton.onclick = () => {
      moveSelectedWaypoints(-1);
    };
    buttonsDiv.appendChild(moveUpButton);

    const moveDownButton = document.createElement("button");
    moveDownButton.textContent = "Down";
    moveDownButton.className =
      "bg-green-500 text-white px-2 py-1 rounded hover:bg-green-700";
    moveDownButton.onclick = () => {
      moveSelectedWaypoints(1);
    };
    buttonsDiv.appendChild(moveDownButton);

    listItem.appendChild(buttonsDiv);

    list.appendChild(listItem);
  });
}

function saveWaypoints() {
  localStorage.setItem(
    "waypoints",
    JSON.stringify(waypoints.map((wp) => ({ lat: wp.lat, lon: wp.lon })))
  );
}

function loadWaypoints() {
  const savedWaypoints = JSON.parse(localStorage.getItem("waypoints"));
  if (savedWaypoints && Array.isArray(savedWaypoints)) {
    savedWaypoints.forEach((wp) => {
      const input = document.getElementById("coordinates");
      input.value = `${wp.lat}, ${wp.lon}`;
      addCoordinates();
    });
  }

  const defaultCenter = [60.18303504726024, 24.927493265440646];
  const defaultZoom = 13;
  const center =
    waypoints.length > 0 ? [waypoints[0].lat, waypoints[0].lon] : defaultCenter;

  map.setView(center, defaultZoom);
}

let editingWaypointIndex = null;

function editCoordinates(index) {
  const waypoint = waypoints[index];
  document.getElementById(
    "coordinates"
  ).value = `${waypoint.lat}, ${waypoint.lon}`;
  editingWaypointIndex = index;

  document.getElementById("add-button").style.display = "none";
  document.getElementById("confirm-button").style.display = "inline";
  document.getElementById("cancel-button").style.display = "inline";
  document.getElementById("delete-button").style.display = "inline";
  document.getElementById("coordinates").focus();
}

function confirmCoordinates() {
  if (editingWaypointIndex === null) return;

  const input = document.getElementById("coordinates");

  if (!input.value) {
    const waypoint = waypoints[editingWaypointIndex];
    waypoint.marker.remove();
    waypoints.splice(editingWaypointIndex, 1);
  } else {
    const coords = input.value.split(",");

    if (coords.length !== 2) {
      alert("Invalid input format. Please use 'latitude,longitude' format.");
      return;
    }

    const lat = parseFloat(coords[0].trim());
    const lon = parseFloat(coords[1].trim());

    if (isNaN(lat) || isNaN(lon)) {
      alert("Invalid coordinates. Please input valid numbers.");
      return;
    }

    const waypoint = waypoints[editingWaypointIndex];
    waypoint.lat = lat;
    waypoint.lon = lon;
    waypoint.marker.setLatLng([lat, lon]);
  }

  input.value = "";

  document.getElementById("add-button").style.display = "inline";
  document.getElementById("confirm-button").style.display = "none";
  document.getElementById("cancel-button").style.display = "none";
  document.getElementById("delete-button").style.display = "none";

  editingWaypointIndex = null;
  updateCoordinatesList();
  updateWaypointPopupContent();
  saveWaypoints();
}

function deleteCoordinate() {
  if (editingWaypointIndex === null) {
    if (selectedWaypoints.length === 0) return;

    selectedWaypoints
      .sort((a, b) => b - a)
      .forEach((index) => {
        const waypoint = waypoints[index];
        waypoint.marker.remove();
        waypoints.splice(index, 1);
      });
    selectedWaypoints = [];

    // Only exit the selection mode if no waypoint is being edited
    if (editingWaypointIndex === null) {
      cancelEditing();
    }
  } else {
    // Keep the existing delete logic here
    const input = document.getElementById("coordinates");
    input.value = "";
  }
}

function cancelEditing() {
  if (editingWaypointIndex === null) return;

  document.getElementById("coordinates").value = "";
  editingWaypointIndex = null;

  const listItems = document.querySelectorAll(".coordinates-list-item");
  listItems.forEach((item) => {
    item.style.backgroundColor = "";
    item.onclick = null;
  });
  selectedWaypoints = [];

  // Only change the UI elements if no waypoint is being edited
  if (editingWaypointIndex === null) {
    document.getElementById("select-button").style.display = "inline";
    document.getElementById("add-button").style.display = "inline";
    document.getElementById("confirm-button").style.display = "none";
    document.getElementById("cancel-button").style.display = "none";
    document.getElementById("delete-button").style.display = "none";
  }
}

function startSelection() {
  const listItems = document.querySelectorAll(".coordinates-list-item");
  listItems.forEach((item, index) => {
    item.onclick = () => {
      toggleWaypointSelection(index);
    };
  });
  document.getElementById("select-button").style.display = "none";
  document.getElementById("add-button").style.display = "none";
  document.getElementById("confirm-button").style.display = "inline";
  document.getElementById("cancel-button").style.display = "inline";
  document.getElementById("delete-button").style.display = "inline";
}

function toggleWaypointSelection(index) {
  const listItem = document.querySelectorAll(".coordinates-list-item")[index];
  if (selectedWaypoints.includes(index)) {
    selectedWaypoints.splice(selectedWaypoints.indexOf(index), 1);
    listItem.style.backgroundColor = "";
  } else {
    selectedWaypoints.push(index);
    listItem.style.backgroundColor = "rgba(0, 0, 255, 0.2)";
  }
}

function submitForm(event) {
  event.preventDefault();

  const input = document.getElementById("coordinates");

  if (editingWaypointIndex === null) {
    if (!input.value.trim()) {
      return;
    }
    addCoordinates();
  } else {
    confirmCoordinates();
  }
}

function moveWaypoint(fromIndex, toIndex) {
  // Check if toIndex is valid
  if (toIndex < 0 || toIndex > waypoints.length - 1) {
    return;
  }

  function moveSelectedWaypoints(toIndexChange) {
    // Sort selectedWaypoints in ascending order
    selectedWaypoints.sort((a, b) => a - b);
    // Only move if the first or last waypoint (depending on direction) can be moved
    if (
      (toIndexChange > 0 &&
        selectedWaypoints[selectedWaypoints.length - 1] <
          waypoints.length - 1) ||
      (toIndexChange < 0 && selectedWaypoints[0] > 0)
    ) {
      // Move each waypoint
      for (let i = 0; i < selectedWaypoints.length; i++) {
        const fromIndex = selectedWaypoints[i];
        const toIndex = fromIndex + toIndexChange;
        // Swap waypoints in the array
        const waypointToMove = waypoints[fromIndex];
        waypoints[fromIndex] = waypoints[toIndex];
        waypoints[toIndex] = waypointToMove;
        // Update selected waypoint index
        selectedWaypoints[i] = toIndex;
      }

      // Update the coordinates list to reflect the new order
      updateCoordinatesList();

      // Save the new order of waypoints
      saveWaypoints();
    }
  }

  // Swap waypoints in the array
  const waypointToMove = waypoints[fromIndex];
  waypoints[fromIndex] = waypoints[toIndex];
  waypoints[toIndex] = waypointToMove;

  // Update the coordinates list to reflect the new order
  updateCoordinatesList();

  // You might also want to save the new order of waypoints
  saveWaypoints();
}

initMap();

map.on("dblclick", addCoordinatesFromMap);
document
  .getElementById("coordinates")
  .addEventListener("keydown", function (event) {
    if (event.key === "Enter") {
      event.preventDefault();
      if (editingWaypointIndex === null) {
        addCoordinates();
      } else {
        confirmCoordinates();
      }
    }
  });
