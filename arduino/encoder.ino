// Definition of interrupt names
#include <avr/io.h>
// ISR interrupt service routine
#include <avr/interrupt.h>

volatile int left_encoder = 0;
volatile int right_encoder = 0;

//int sensePin = 3;
// Install the interrupt routine.
ISR(INT4_vect) {
  // check the value again - since it takes some time to
  // activate the interrupt routine, we get a clear signal.
  left_encoder++;
}

ISR(INT5_vect) {
  // check the value again - since it takes some time to
  // activate the interrupt routine, we get a clear signal.
  right_encoder++;
}


void setup() {
  Serial.begin(9600);
//  pinMode(sensePin, INPUT);
  // Set INT4 as falling edge triggered
  EICRB |= (1 << ISC40);
  EICRB |= (1 << ISC50);
   
  // Enable INT4 interrupt
  EIMSK |= (1 << INT4);
  EIMSK |= (1 << INT5);
}

void loop() {
  Serial.print(left_encoder);
  Serial.print("\t");
  Serial.println(right_encoder);
}
