/* --------------- PWM Test library ------------
 *  
 *  Used for controlling PCA9866 devices around board
 *  
 *  
 */

void pwm_test_on();
void pwm_setup();
void pwm_test_off();

//PWM for base
Adafruit_PWMServoDriver pwm40 = Adafruit_PWMServoDriver(0x40);
//PWM for layers and Relays

Adafruit_PWMServoDriver pwm41 = Adafruit_PWMServoDriver(0x41);

void pwm_setup() {

  pwm40.begin();
  pwm40.setPWMFreq(1000);  // Set to whatever you like, we don't use it in this demo!

  pwm41.begin();
  pwm41.setPWMFreq(1000);

  Serial.println("All on");
  pwm_test_on();
  delay(1000);
  Serial.println("All off");
  pwm_test_off();
  delay(1000);
  
}

void pwm_test_on() {
  // Drive each pin in a 'wave'
  for (uint8_t pin=0; pin<16; pin++) {
    pwm40.setPWM(pin, 4096, 0);       // turns pin fully on
    pwm41.setPWM(pin, 4096, 0);       // turns pin fully on
  }
  delay(200);
}

void pwm_test_off(){
  for (uint8_t pin=0; pin<16; pin++) {
    pwm40.setPWM(pin, 0, 4096);       // turns pin fully off
    pwm41.setPWM(pin, 0, 4096);       // turns pin fully off
  }
  delay(200);

}
