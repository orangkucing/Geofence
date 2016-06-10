void setupTrigger()
{
  pinMode(SHUTTER, INPUT);
}

void sendStart()
{
  outputLogFile("$PSTART\r\n");
  digitalWrite(SHUTTER, LOW);
  pinMode(SHUTTER, OUTPUT);
}

void sendStop()
{
  outputLogFile("$PEND\r\n");
  pinMode(SHUTTER, INPUT);
}

