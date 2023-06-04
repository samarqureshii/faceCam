// This is a basic snapshot sketch using the VC0706 library.
// On start, the Arduino will find the camera and SD card and
// then snap a photo, sending it over the serial connection.
// Public domain.

#include <Adafruit_VC0706.h>
#include <SPI.h>

#if defined(__AVR__) || defined(ESP8266)
// On Uno: camera TX connected to pin 2, camera RX to pin 3:
#include <SoftwareSerial.h>         
SoftwareSerial cameraconnection(2, 3);
#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// Using hardware serial on Mega: camera TX conn. to RX1,
// camera RX to TX1, no SoftwareSerial object is required:
#define cameraconnection Serial1
#endif

Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

void setup() {
  Serial.begin(9600);
  Serial.println("VC0706 Camera snapshot test");
  
  // Try to locate the camera
  if (cam.begin()) {
    Serial.println("Camera Found:");
  } else {
    Serial.println("No camera found?");
    return;
  }

  // Print out the camera version information (optional)
  char *reply = cam.getVersion();
  if (reply == 0) {
    Serial.print("Failed to get version");
  } else {
    Serial.println("-----------------");
    Serial.print(reply);
    Serial.println("-----------------");
  }

  // Set the picture size - you can choose one of 640x480, 320x240 or 160x120 
  // Remember that bigger pictures take longer to transmit!
  cam.setImageSize(VC0706_640x480);        // biggest

  // You can read the size back from the camera (optional, but maybe useful?)
  uint8_t imgsize = cam.getImageSize();
  Serial.print("Image size: ");
  if (imgsize == VC0706_640x480) Serial.println("640x480");
  if (imgsize == VC0706_320x240) Serial.println("320x240");
  if (imgsize == VC0706_160x120) Serial.println("160x120");

  Serial.println("Snap in 3 secs...");
  delay(3000);

  if (! cam.takePicture()) 
    Serial.println("Failed to snap!");
  else 
    Serial.println("Picture taken!");
  
  // Get the size of the image (frame) taken  
  uint32_t jpglen = cam.frameLength();
  Serial.print("Storing ");
  Serial.print(jpglen, DEC);
  Serial.print(" byte image.");

  int32_t time = millis();
  pinMode(8, OUTPUT);

  // Read all the data up to # bytes!
  byte wCount = 0; // For counting # of writes
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min((uint32_t)32, jpglen);
    buffer = cam.readPicture(bytesToRead);
    Serial.write(buffer, bytesToRead);
    if(++wCount >= 64) { 
      Serial.print('.');
      wCount = 0;
    }
    jpglen -= bytesToRead;
  }

  time = millis() - time;
  Serial.println("done!");
  Serial.print(time); Serial.println(" ms elapsed");

}

  void loop() {
} 