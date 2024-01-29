#include "arduino.h"
#include "melty_config.h"

void init_led(void) {
  pinMode(HEADING_LED_PIN, OUTPUT);
}

void heading_led_on(int shimmer) {
    //check to see if we should "shimmer" the LED to indicate something to user
  if (shimmer == 1) {
    if (micros() & (1 << 10)) {
      digitalWrite(HEADING_LED_PIN, HIGH);
    } else {
      digitalWrite(HEADING_LED_PIN, LOW);
    }  
  } else {
    //just turn LED on
     digitalWrite(HEADING_LED_PIN, HIGH);
  }
}

void heading_led_off() {
  digitalWrite(HEADING_LED_PIN, LOW);
}
