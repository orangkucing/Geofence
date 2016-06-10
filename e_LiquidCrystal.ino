#ifdef USE_LIQUID_CRYSTAL

// initialize OLED
//                   addr, contrast(0-255)
I2CLiquidCrystal lcd(0x3C, (uint8_t)127);

void setupLiquidCrystal()
{
#if defined(__SAM3X8E__)
  // Due has 1k5 pull up at the default TWI I/F, Wire. The value is too small so we'd better use Wire1.
  lcd.setWire(&Wire1);
#endif
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(F("No Fix"));
}

void LCDprint(char *p)
{
  // print hour:minute:second on LCD
  lcd.setCursor(0, 0);
  for (int i = 0; ; ) {
    lcd.print(p[i++]); lcd.print(p[i++]);
    if (i == 6)
      break;
    lcd.print(F(":"));
  }
}

#else

void setupLiquidCrystal()
{
  
}

void LCDprint(char *s)
{
  
}
#endif
