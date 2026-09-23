// TextAnimation works only when ArduinoGraphics is installed and used.
// ArduinoGraphics is an external library and needs to be installed using
// Library Manager.
// To use ArduinoGraphics APIs, please include BEFORE Arduino_LED_Matrix
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include <floatToString.h>
#include "Adafruit_SHT4x.h"

const unsigned long TEMP_CHECK_INTERVAL = 5000;
unsigned long lastCheck = millis();
bool firstRun = true;

Adafruit_SHT4x sht4 = Adafruit_SHT4x();
ArduinoLEDMatrix matrix;
char sDisp[10];

void setup() {
  matrix.begin();

 

  Wire1.begin();
  Serial.begin(115200);

  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit SHT4x test");
  if (! sht4.begin(&Wire1)) {
    Serial.println("Couldn't find SHT4x");
    while (1) delay(1);
  }
  Serial.println("Found SHT4x sensor");
  Serial.print("Serial number 0x");
  Serial.println(sht4.readSerial(), HEX);
  
}


String getReading() {
  Serial.println("Getting reading...");
  sensors_event_t humidity, temp;
  String reading = "T: ";
  Serial.println("  Calling getEvent()...");
  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  Serial.println("  First call to floatToString()");
  floatToString(temp.temperature, sDisp, sizeof(sDisp), 0);
  reading += String(sDisp);
  Serial.println("  Second call to floatToString()");
  floatToString(humidity.relative_humidity, sDisp, sizeof(sDisp), 0);
  reading += ", H: " + String(sDisp);

  return reading;
}


void loop() {
  unsigned long curMillis = millis();

  if(firstRun || curMillis < lastCheck || (curMillis - lastCheck) > TEMP_CHECK_INTERVAL) {
    lastCheck = curMillis;
    String text = getReading();
    firstRun = false;

    Serial.println(String("Reading is ") + text);
    // Make it scroll!
    matrix.beginDraw();

    matrix.stroke(0xFFFFFFFF);
    matrix.textScrollSpeed(150);

    // add the text
    matrix.textFont(Font_5x7);
    matrix.beginText(0, 1, 0xFFFFFF);
    matrix.println(text.c_str());
    matrix.endText(SCROLL_LEFT);

    matrix.endDraw();
  }
}