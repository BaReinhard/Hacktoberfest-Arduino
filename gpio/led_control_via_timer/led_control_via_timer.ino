void setup()
{
  // LED is connected to pin 13
  pinMode(13, OUTPUT);
  
  // disable interrupts
  cli(); 
  //set TCCR1A register to 0
  TCCR1A = 0;
  //set TCCR1A register to 0
  TCCR1B = 0;
  
  //(1<<CS12) creates a temporary operand that contains 1 in the CS12 position.
  TCCR1B |= 1 << CS12;
  
  //timer counter interrupt register 
  TIMSK1 |= 1 << TOIE1;

  //Timer 1 is a 16-bit timer -> 2^16 = 65535
  TCNT1 = 65535 - 31267;
  
  //enable interrupts
  sei();  
}


ISR(TIMER1_OVF_vect){
  
  // to make LED blink, it is necessary to get the delay
  TCNT1 = 65535 - 31267;
  
  //change state of LED 
  digitalWrite(13, !digitalRead(13));
  
}
void loop()
{
  
}
