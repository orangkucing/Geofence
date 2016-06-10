// Interface to simple mechanical switches with software debounce.
//
// This code is used for the miniSD card detect.
//
// Simple mechanical switches require debounce. For software debounce, original code can be found at everywhere, for example,
//   http://www.arduino.cc/en/Tutorial/Debounce
//

#ifdef USE_MICRO_SD

SdFat sd;

int microSDState = 0; // 0: no card, 1: card inserted, 2: config updated

void switchClosedCommand()
{
  sd.begin(SPI_CS, SPI_FULL_SPEED);
  SdFile::dateTimeCallback(&SDtimeStamp); // set callback for file timestamps
  microSDState = 1;
}

void switchOpenedCommand()
{
  microSDState = 0; // card ejected
}

void setupSwitch()
{
  pinMode(CARD_DETECT, INPUT_PULLUP);
  delay(10); // 10ms delay is necessary for voltage to stabilize
}

void checkSwitch()
{
  static unsigned long lastDebounceTime = 0;
  // default to no card
  static char lastButtonState = HIGH;
  static char buttonState = HIGH;

  // read switch with debounce
  char reading = digitalRead(CARD_DETECT);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime > 100) {
    if (reading != buttonState) {
      if (reading == LOW) {
        switchClosedCommand();
      } else {
        switchOpenedCommand();
      }
      buttonState = reading;
    }
  }
  lastButtonState = reading;
}

#else

void setupSwitch()
{
  
}

void checkSwitch()
{
  
}
#endif
