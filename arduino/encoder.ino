// Definition of interrupt names
#include <avr/io.h>
// ISR interrupt service routine
#include <avr/interrupt.h>

volatile int left_encoder = 0;
volatile int right_encoder = 0;

int sensePin = 2;
// Install the interrupt routine.
ISR(INT0_vect) {
  // check the value again - since it takes some time to
  // activate the interrupt routine, we get a clear signal.
  left_encoder++;
}


void setup() {
  Serial.begin(9600);
  pinMode(sensePin, INPUT);
  EIMSK |= ( 1 << INT0);
  // Signal change triggers interrupt
  MCUCR |= ( 1 << ISC00);
  MCUCR |= ( 0 << ISC01);
}

void loop() {
  Serial.println(left_encoder);
}
