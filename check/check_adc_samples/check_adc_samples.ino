
/* Check how many samples can be read from ADC in 1 second */

/* Serial Speed */
#define SERIAL_BAUDS 9600 // 9600, 19200, 57600, 115200, 230400, 250000

/* ADC Pin to be used */
#define ANALOG_PIN A0

void setup()
{
    // Initialize Serial
    Serial.begin(SERIAL_BAUDS);
    delay(3000);
}

void loop()
{
    static uint8_t num_samples = 0;
    static unsigned long t0 = millis();
    unsigned long t1;
    uint16_t read;

    // Get current systick
    t1 = millis();

    if(t1 - t0 >= 1000)
    {
        // A second has passed, show num of read samples
        Serial.print("ADC samples read in 1 second: ");
        Serial.println(num_samples);
        num_samples = 0;
        t0 = millis();
    }
    else
    {
        // Still 1s not reached, read from ADC
        read = analogRead(ANALOG_PIN);
        num_samples = num_samples + 1;
    }
}

