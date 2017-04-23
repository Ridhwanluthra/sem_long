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

int pin_left1 = 4, pin_left2 = 5, pwm_left3 = 6;
int pin_right1 = 8, pin_right2 = 7, pwm_right3 = 9;
int r_pi_pin1 = 10, r_pi_pin2 = 11, r_pi_pin3 = 12;

void setup(){
  // Set INT4 as falling edge triggered
  EICRB |= (1 << ISC40);
  EICRB |= (1 << ISC50);
   
  // Enable INT4 interrupt
  EIMSK |= (1 << INT4);
  EIMSK |= (1 << INT5);

  pinMode(pin_left1, OUTPUT);
  pinMode(pin_left2, OUTPUT);
  pinMode(pwm_left3, OUTPUT);
  pinMode(pin_right1, OUTPUT);
  pinMode(pin_right2, OUTPUT);
  pinMode(pwm_right3, OUTPUT);
  pinMode(r_pi_pin1, INPUT);
  pinMode(r_pi_pin2, INPUT);
  pinMode(r_pi_pin3, INPUT);
  analogWrite(pwm_left3, 150);
  analogWrite(pwm_right3, 150);
}

void loop(){
  int one = digitalRead(r_pi_pin1);
  int two = digitalRead(r_pi_pin2);
  int three = digitalRead(r_pi_pin3);
  
  if (one == LOW && two == LOW && three == LOW){
    stop_all();
  } else if (one == HIGH && two == LOW && three == LOW){
    move_to_cell(1);
  } else if (one == LOW && two == HIGH && three == LOW){
    move_to_cell(2);
  } else if (one == HIGH && two == HIGH && three == LOW){
    move_to_cell(3);
  } else if (one == LOW && two == LOW && three == HIGH){
    move_to_cell(4);
  } else if (one == HIGH && two == LOW && three == HIGH){
    move_to_cell(6);
  }
}

void move_to_cell(int x){
  int forty_five = 30;
  if (x == 1){
    left(forty_five);
    forward(70);
  }else if (x == 2){
    forward(60);
  }else if (x == 3){
    right(forty_five);
    forward(70);
  }else if (x == 4){
    left(forty_five*2);
  }else if (x == 6){
    right(forty_five*2);
  }
}

void forward(int encoder_count){
  left_encoder = 0;
  while(left_encoder < encoder_count) {
    digitalWrite(pin_left1, LOW);
    digitalWrite(pin_left2, HIGH);
    digitalWrite(pin_right1, LOW);
    digitalWrite(pin_right2, HIGH);
  }
  stop_all();
}

void left(int encoder_count){
  left_encoder = 0;
  while(left_encoder < encoder_count) {
    digitalWrite(pin_left1, LOW);
    digitalWrite(pin_left2, HIGH);
    digitalWrite(pin_right1, HIGH);
    digitalWrite(pin_right2, LOW);
  }
  stop_all();
}

void right(int encoder_count){
  left_encoder = 0;
  while(left_encoder < encoder_count) {
    digitalWrite(pin_left1, HIGH);
    digitalWrite(pin_left2, LOW);
    digitalWrite(pin_right1, LOW);
    digitalWrite(pin_right2, HIGH);
  }
  stop_all();
}

void stop_all(){
  digitalWrite(pin_left1, LOW);
  digitalWrite(pin_left2, LOW);
  digitalWrite(pin_right1, LOW);
  digitalWrite(pin_right2, LOW);  
}