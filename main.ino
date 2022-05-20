/**
 * LED based Timer application
 * 
 * @author Selcuk Kekec <khskekec@gmail.com>
 * @version 0.1
 */
 
// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "ENTER_TEMPLATE_ID"
#define BLYNK_DEVICE_NAME           "ENTER_DEVICE_NAME"
#define BLYNK_AUTH_TOKEN            "ENTER_AUTH_TOKEN"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define PIN 14


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "CountDown.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
                            NEO_MATRIX_TOP    + NEO_MATRIX_LEFT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "ENTER_WLAN_SSID";
char pass[] = "ENTER_WLAN_PASSPHRASE";

BlynkTimer timer;

CountDown CD(CountDown::SECONDS);
uint32_t start, stop;
uint32_t minutesToGo;
BLYNK_WRITE(V10)
{
  minutesToGo = param.asInt();
}

BLYNK_WRITE(V11)
{
  int shouldStartTimer = param.asInt();

  if (shouldStartTimer == 1 && minutesToGo)
  {
    CD.start(minutesToGo*60);
  }
}

BLYNK_WRITE(V13)
{
  int shouldStopTimer = param.asInt();

  if (shouldStopTimer == 1)
  {
    CD.start(0);
    CD.stop();
  }
}

void myTimerEvent()
{
  Blynk.virtualWrite(V2, millis() / 1000);
  uint32_t remaining = CD.remaining();
  String text;
  matrix.fillScreen(0);
  if (remaining > 0)
  {
    text += String(remaining / 60, DEC);
    text += String(":");
    text += String(remaining % 60, DEC);
    Blynk.virtualWrite(V12, text);
    matrix.setCursor(0, 0);
    matrix.setTextColor(matrix.Color(52, 143, 235));
    matrix.print(text);
  } else {
    Blynk.virtualWrite(V12, "FINISHED!");
  }

  matrix.show();
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  // Setup matrix
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(10);
  matrix.fillScreen(0);

  Blynk.begin(auth, ssid, pass, "ENTER_BLYNK_URL", "ENTER_BLYNK_PORT_AS_INTEGER");

  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}
