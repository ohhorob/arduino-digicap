#include <Arduino.h>

#include <Bounce2.h>
#define TPS_DIV A15
#define LED 13u

Bounce debouncer = Bounce();
bool wasPressed = false;

bool hasSerial = false;


void setup() {
    pinMode(LED, OUTPUT);
    Serial.begin(19200);  // USB, communication to PC or Mac
    while(!Serial && millis() < 2000) {
        digitalWriteFast(LED, HIGH);
        delay(20);
    }
    hasSerial = !(!Serial);
    digitalWriteFast(LED, LOW);
    if (hasSerial) {
        Serial.println("Connected.");
    }

    // Pullup requires the button connect the pin to ground
    debouncer.attach(TPS_DIV, INPUT_PULLUP);
    debouncer.interval(10);

    wasPressed = debouncer.read();
}

void maintainButton() {

    // Update the Bounce instance :
    debouncer.update();

    // Get the updated value :
    bool pressed = !debouncer.read();
    if (wasPressed != pressed) {
        wasPressed = pressed;
        if (hasSerial) {
            Serial.print(millis());
            Serial.print(pressed ? ": Closed" : ": Open");
            Serial.println(" now");
        }
        digitalWriteFast(LED, wasPressed ? HIGH : LOW);
    }
}

void loop() {
    maintainButton();
}

