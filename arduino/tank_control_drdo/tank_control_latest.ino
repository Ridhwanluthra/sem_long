volatile int left_encoder = 0;
volatile int right_encoder = 0;


int encoder_left = 23, encoder_right = 22;
int pin_left1 = 29, pin_left2 = 33, pwm_left3 = 31;
int pin_right1 = 35, pin_right2 = 39, pwm_right3 = 37;
int r_pi_pin1 = 10, r_pi_pin2 = 11, r_pi_pin3 = 12, r_pi_pin4 = 13;
int counter = 0;
void setup(){
  Serial.begin(9600);

  pinMode(pin_left1, OUTPUT);
  pinMode(pin_left2, OUTPUT);
  pinMode(pwm_left3, OUTPUT);
  pinMode(pin_right1, OUTPUT);
  pinMode(pin_right2, OUTPUT);
  pinMode(pwm_right3, OUTPUT);
  pinMode(r_pi_pin1, INPUT);
  pinMode(r_pi_pin2, INPUT);
  pinMode(r_pi_pin3, INPUT);
  analogWrite(pwm_left3, 130);
  analogWrite(pwm_right3, 130);
  pinMode(encoder_left, INPUT);
  pinMode(encoder_right, INPUT);
}

void loop(){
/*  int one = digitalRead(r_pi_pin1);
  int two = digitalRead(r_pi_pin2);
  int three = digitalRead(r_pi_pin3);
  int four = digitalRead(r_pi_pin4);
//  */
//  int one = LOW;
//  int two = LOW;
//  int three = LOW;
//  int four = LOW;
//  if (counter == 0){
//  one = HIGH;
//  two = LOW;
//  three = LOW;
//  four = HIGH;
move_to_cell(1);
delay(2000);
move_to_cell(3);
delay(2000);
move_to_cell(6);
delay(2000);
move_to_cell(9);
delay(2000);
move_to_cell(8);
delay(100000);
//  counter = 1;
//
//  if (one == LOW && two == LOW && three == LOW && four == LOW){
//    stop_all();
//  } else if (one == LOW && two == LOW && three == LOW && four == HIGH){
//    move_to_cell(1);
//  } else if (one == LOW && two == LOW && three == HIGH && four == LOW){
//    move_to_cell(2);
//  } else if (one == LOW && two == LOW && three == HIGH && four == HIGH){
//    move_to_cell(3);
//  } else if (one == LOW && two == HIGH && three == LOW && four == LOW){
//    move_to_cell(4);
//  } else if (one == LOW && two == HIGH && three == LOW && four == HIGH){
//    move_to_cell(5);
//  } else if (one == LOW && two == HIGH && three == HIGH && four == LOW){
//    move_to_cell(6);
//  }else if (one == LOW && two == HIGH && three == HIGH && four == HIGH){
//    move_to_cell(7);
//  }else if (one == HIGH && two == LOW && three == LOW && four == LOW){
//    move_to_cell(8);
//  }else if (one == HIGH && two == LOW && three == LOW && four == HIGH){
//    move_to_cell(9);
//  }
}

void move_to_cell(int x){
  int forty_five = 20;
  int distance = 30;
  if (x == 1){
    left(forty_five);
    delay(1000);
    forward(distance);
  }else if (x == 2){
    forward(distance);
  }else if (x == 3){
    right(forty_five);
    delay(1000);
    forward(distance);
  }else if (x == 4){
    left(forty_five*2);
    forward(distance);
  }else if(x == 5){
    stop_all();
  }else if (x == 6){
    right(forty_five*2);
    forward(distance);
  }else if(x == 7){
    left(forty_five*3);
    delay(1000);
    forward(distance);
  }else if(x == 8){
    left(forty_five*4);
    forward(distance);
  }else if(x == 9){
    right(forty_five*3);
    forward(distance);
  }
}

void forward(int encoder_count){
  left_encoder = 0;
  volatile int prev = 0;
  volatile int curr = 0;
  
  while(left_encoder < encoder_count) {
    curr = digitalRead(encoder_left);
    if (prev != curr){
       prev = curr;
       left_encoder++;
    }
    
    digitalWrite(pin_left1, LOW);
    digitalWrite(pin_left2, HIGH);
    digitalWrite(pin_right1, LOW);
    digitalWrite(pin_right2, HIGH);
  }
  stop_all();
}

void left(int encoder_count){
  left_encoder = 0;
  volatile int prev = 0;
  volatile int curr = 0;
  
  while(left_encoder < encoder_count) {
    curr = digitalRead(encoder_left);
    if (prev != curr){
       prev = curr;
       left_encoder++;
    }
    digitalWrite(pin_left1, LOW);
    digitalWrite(pin_left2, HIGH);
    digitalWrite(pin_right1, HIGH);
    digitalWrite(pin_right2, LOW);
  }
  stop_all();
}

void right(int encoder_count){
  left_encoder = 0;
  volatile int prev = 0;
  volatile int curr = 0;
  
  while(left_encoder < encoder_count) {
    curr = digitalRead(encoder_left);
    if (prev != curr){
       prev = curr;
       left_encoder++;
    }
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
