#include <EEPROM.h>
#include <QMC5883LCompass.h>
#include <math.h>

QMC5883LCompass compass;

float myLat0 = 60.133470206254394, myLon0 = 24.942398071289066;  //previous location
float myLat1 = 60.138256970188564, myLon1 = 24.946861267089847;  //location now

float headingNow = 0;
float targetBearingNow = 0;
float headingChange = 0;
float distanceToNextWaypoint = 0;

//target coordinates
float WaypointLat[] = { -1.0, 60.146119427630715, 60.139282614706985, 60.12859724911918, 60.121842308255474, 60.130563616914216, 60.14372670476474, 60.14825564022888 };
float WaypointLong[] = { -1.0, 24.9580192565918, 24.97346878051758, 24.97346878051758, 24.941368103027347, 24.911155700683597, 24.927120208740238, 24.93947982788086 };

int WaypointIndex = 1;
int WayPointCount = sizeof(WaypointLat) / sizeof(WaypointLat[0]);
void setup() {
  Serial.begin(9600);
  compass.setCalibration(-2800, 15, -3570, 0, -6892, 0);
}

void loop() {
  compass.read();
  myCoordinates();  //reads coordinates from gps and saves them to myLat1 and myLong1
  //headingNow = currentGPSHeading(myLat0, myLon0, myLat1, myLon1);
  headingNow = compass.getAzimuth();
  targetBearingNow = targetBearing(myLat1, myLon1, WaypointLat[WaypointIndex], WaypointLong[WaypointIndex]);
  headingChange = turnAngle(headingNow, targetBearingNow);
  distanceToNextWaypoint = haversineDistance(myLat1, myLon1, WaypointLat[WaypointIndex], WaypointLong[WaypointIndex]);

  Serial.print(WaypointIndex);
  Serial.print(" my cordinates:");
  Serial.print(myLat1, 6);
  Serial.print(", ");
  Serial.print(myLon1, 6);
  Serial.print(" headingNow ");
  Serial.print(headingNow);
  Serial.print(" targetBearingNow ");
  Serial.print(targetBearingNow);
  Serial.print(" headingChange ");
  Serial.print(headingChange);
  Serial.print(" distanceToNextWaypoint ");
  Serial.println(distanceToNextWaypoint);

  WaypointIndex = WaypointIndex + 1;
  if (WaypointIndex >= WayPointCount) {
    Serial.println(" ");
    WaypointIndex = 1;
    delay(5000);
  }
}

void myCoordinates() {
  //reads coordinates from gps and saves them to myLat1 and myLong1
  
  //manual input for testing purposes:
  if (Serial.available()) {                                           // Check if data is available to read
    char incomingData[30];                                            // Array to store incoming serial data
    Serial.readBytesUntil('\n', incomingData, sizeof(incomingData));  // Read the incoming data until newline character

    // Now parse the incomingData
    char *latitudeStr = strtok(incomingData, ",");  // Split the string using comma as delimiter
    char *longitudeStr = strtok(NULL, ",");

    if (latitudeStr != NULL && longitudeStr != NULL) {  // Check if both latitude and longitude are available
      myLat1 = atof(latitudeStr);                       // Convert string to float
      myLon1 = atof(longitudeStr);
    }
  }
}

float currentGPSHeading(float lat1, float lon1, float lat2, float lon2) {
  const float pi = 3.14159265358979323846;

  // Convert decimal degrees to radians
  lat1 = lat1 * pi / 180;
  lon1 = lon1 * pi / 180;
  lat2 = lat2 * pi / 180;
  lon2 = lon2 * pi / 180;

  // Calculate the difference in longitude
  float delta_lon = lon2 - lon1;

  // Calculate the heading
  float y = sin(delta_lon) * cos(lat2);
  float x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(delta_lon);
  float heading = atan2(y, x);

  // Convert radians to degrees
  heading = heading * 180 / pi;

  // Normalize heading to be between 0 and 360 degrees
  if (heading < 0) {
    heading += 360;
  }

  return heading;
}

float targetBearing(float lat1, float lon1, float lat2, float lon2) {
  const float pi = 3.14159265358979323846;

  // Convert decimal degrees to radians
  lat1 = lat1 * pi / 180;
  lon1 = lon1 * pi / 180;
  lat2 = lat2 * pi / 180;
  lon2 = lon2 * pi / 180;

  // Calculate the difference in longitude and latitude
  float delta_lon = lon2 - lon1;
  float delta_lat = lat2 - lat1;

  // Calculate the bearing
  float y = sin(delta_lon) * cos(lat2);
  float x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(delta_lon);
  float bearing = atan2(y, x);

  // Convert radians to degrees
  bearing = bearing * 180 / pi;

  // Normalize bearing to be between 0 and 360 degrees
  if (bearing < 0) {
    bearing += 360;
  }

  return bearing;
}

float turnAngle(float current_heading, float target_bearing) {
  float turn_angle = target_bearing - current_heading;

  // Normalize turn angle to be between -180 and 180 degrees
  if (turn_angle > 180) {
    turn_angle -= 360;
  } else if (turn_angle < -180) {
    turn_angle += 360;
  }

  return turn_angle;
}

float haversineDistance(float lat1, float lon1, float lat2, float lon2) {
  const float pi = 3.14159265358979323846;
  const float R = 6371e3;  // Earth's radius in meters

  // Convert decimal degrees to radians
  lat1 = lat1 * pi / 180;
  lon1 = lon1 * pi / 180;
  lat2 = lat2 * pi / 180;
  lon2 = lon2 * pi / 180;

  // Calculate the differences in coordinates
  float delta_lat = lat2 - lat1;
  float delta_lon = lon2 - lon1;

  // Calculate the haversine of the central angle
  float a = sin(delta_lat / 2) * sin(delta_lat / 2) + cos(lat1) * cos(lat2) * sin(delta_lon / 2) * sin(delta_lon / 2);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));

  // Calculate the distance
  float distance = R * c;

  return distance;
}
