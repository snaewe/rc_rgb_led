//
// Code von: http://meineweltinmeinemkopf.blogspot.de/2015/04/beleuchtung-mit-dem-attiny-45.html
//
// NeoPixel Bibliothek hinzufügen für die Ansteuerung der WS2812B LED Streifen
#include <Adafruit_NeoPixel.h>

// Instanz initialisieren
// Parameter Angaben:
// 1. Anzahl für verwendeter RGB LEDs
// 2. Auf welchen Pin soll verwendet werden.
// 3. Auswahl zwischen RGB, GRB oder BRG und verwendeter Takt 400KHz oder 800KHz
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(4, 2, NEO_GRB + NEO_KHZ800);

// Helligkeit einstellen. Kann von 0 bis Maximal 255 festgelegt werden.
// Bei Maximaler Ausleuchtung ist zu prüfen, ob die Maximale Stromabnahme erreicht wird.
const int brightness = 100;

// Helligkeit bei verwendung der WS2812b, wenn sie weis auf blitzen sollen.
// Bei 210 wird die Maximale Stromabnahme überschritten.
// Darf nur kurz verwendet werden, ansonsten brennt der Spannungsregler durch.
const int brightnessWhite = 210;

// Wird zum festlegen des Licht Einstellung verwendet.
// Damit die Einstellung funktioniert,
// muss an der Fernsteuerung einmal hin und her Geschaltet oder gedreht werden,
// um die längste Pulslänge zu ermitteln.
static unsigned long maxPwmIn = 2;

void setup() {
  // Signal von der Fernsteuerung aufnehmen.
  pinMode(3, INPUT);
  // Positionslichter schalten
  pinMode(0, OUTPUT);

  // RGB LEDs Stribes starten
  pixels.begin();
  pixels.show();
}

void loop() {

  // Signal von der Fernsteuerung einlesen.
  // Für die Verwendung sollte ein Schalter genutzt werden.
  unsigned long pwmIn =  pulseIn(3, HIGH);
  // Wird zum Anlernen verwendet. Durch festlegen der maximalen Pulslänge
  // können zwei Zustände verwendet werden.
  if (pwmIn > maxPwmIn) {
    maxPwmIn = pwmIn;
  }

  // Wenn das Signal kürzer ist als der Maximalen Pulslänge.
  if (pwmIn < maxPwmIn - 100) {
    // Positionslichter einschalten
    // (Bei verwendung von NPN Transisitor, kann die '255 -' weg)
    analogWrite(0, 255 - brightness); delay(100);
    // Ausschalten (Bei NPN, den Wert auf 0 setzen.)
    analogWrite(0, 255); delay(100);

    // RGB LED Streifen ansteuern.
    // Rot und Grün
    allRGB_LED_Off();
    pixels.setPixelColor(0, pixels.Color(brightness, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, brightness, 0));
    pixels.show();
    delay(100);
    allRGB_LED_Off();
    pixels.setPixelColor(2, pixels.Color(brightness, 0, 0));
    pixels.setPixelColor(3, pixels.Color(0, brightness, 0));
    pixels.show();
    delay(100);
  }
  else
  {
    // Positionslichter ausschalten
    // (Bei verwendung von NPN Transisitor, ist zum Ausschalten der Wert 0)
    analogWrite(0, 255);

    // RGB LED Streifen ansteuern.
    for (int i = 0; i < 4; i++)
    {
      // Alle aus.
      allRGB_LED_Off();

      // Mit der Aktuellen Iteration als Index verwenden,
      // um den entsprechende RGB LED zu überschreiben.
      pixels.setPixelColor(i, pixels.Color(brightnessWhite, brightnessWhite, brightnessWhite));
      pixels.show();
      delay(100);
    }
  }
}

// Stellt alle Werte zurück.
void allRGB_LED_Off()
{
  for (int i = 0; i < 4; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
}
