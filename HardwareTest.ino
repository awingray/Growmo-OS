/*------------ GROWMO Hardware test ---------------
 * 
 * This software includes the following test:
 * 1) Connectivity verification (on startup):
 *    Check that all I2C addresses are present
 *    
 *    -0x40 and 0x41 for 2 PCA9685,118 chips and 
 *     0x70 (broadcast addrs) located on PCB1 board
 *    
 *    -0x7X for PCA9544 for I2C mux controllers located
 *     on each PCB2 board where X shows the user 
 *     defined address
 *     
 *     -0x5A for each Co2 sensor (this is going to be
 *      changed in the future)
 *      
 *     -8 DHC11 sensors on pins D2-D9
 *      
 * 2) Functionality test
 *    Test the function of the found devices
 *    
 *    -32 PWM outputs from the two PCA9685 chips 
 *      ON/OFF for 2 sec for each outputs
 *    
 *    -Read values from found Co2 sensors
 *    
 *    -Read values from found DHC11 sensors
 */
 
#include <Wire.h>
#include "DHT.h"
#include <Adafruit_PWMServoDriver.h>
#include "Adafruit_SGP30.h"


#include "DHT11_test.h"
#include "I2C_test.h"
#include "PWM_test.h"
#include "SGP30_test.h"
#include "hcsr04.h"
#include "tentacle.h"

#define TRIG_PIN 11
#define ECHO_PIN 12

HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, 20, 4000);

//Important from I2C test
//short pwm_found[] = {0,0,0};
//short mux_found[] = {0,0,0};
//short co2_found[] = {0,0,0,0,0,0};

//From DHT11_ test
//short dht11_found[] = {0,0,0,0,0,0,0,0};


bool wait_command = false;

void setup() {
  
  Wire.begin();

  Serial.begin(115200);
  while (!Serial);             // Leonardo: wait for Serial monitor

  Serial.println("\nDevice Scan");
  Serial.println("=========================================");

  //Initialize connected devices
  Serial.println("=============== I2C Test ===============");
  I2C_test();
  Serial.println("=========================================");
 
  Serial.println("============== DHT11 INIT ===============");
  dht_begin();
  dht_test();
  Serial.println("=========================================");
  
  Serial.println("=============== SPG INIT ================");
  sgp_begin_all();
  Serial.println("=========================================");

  Serial.println("=============== PWM INIT ================");
  pwm_setup(); 
  Serial.println("=========================================");
  Serial.println("");
  
}

void loop() {
  
  if (!wait_command){
    
    Serial.println("Serial command menu:");
    Serial.println("--------------------");
    Serial.println(" 1 - DHT11 test");
    Serial.println(" 2 - SGP test");
    Serial.println(" 3 - PWM on");
    Serial.println(" 4 - PWM off");
    Serial.println(" 5 - I2C test");
    Serial.println(" 6 - Distance sensor");
    Serial.println(" 7 - Tentacle read");
    Serial.println("--------------------");
    wait_command = true;
  }
  
  
}


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    Serial.println(inChar);
  if ((inChar != '\n') && (inChar != '\r')){
      serial_action(inChar);
    }
  }
  wait_command = false;
}

void serial_action(char c){
  switch(c){
    case '1': 
      Serial.println(" 1 - DHT11 test:");
      Serial.println("--------------------");
      dht_test();
      break;

    case '2': 
      Serial.println(" 2 - SGP30 test:");
      Serial.println("--------------------");
      sgp_read_all();
      break;

    case '3': 
      Serial.println(" 3 - DC ON / AC OFF:");
      Serial.println("--------------------");
      pwm_test_on();
      break;
    
    case '4':
      Serial.println(" 4 - DC OFF / AC ON:");
      Serial.println("--------------------");
      pwm_test_off();
      break;
    
    case '5': 
      Serial.println(" 5 - I2C test:");
      Serial.println("--------------------");
      I2C_test();
      break;

    case '6':
      Serial.println(" 6 - Distanse sensor:");
      Serial.println("--------------------");
      Serial.println(hcsr04.ToString());
      Serial.println("--------------------");
      break;

    case '7':
      Serial.println(" 7 - Tentacle read:");
      Serial.println("--------------------");
      tentacle_read();
      break;

    default:  
      Serial.println(" WRONG INPUT");
      Serial.println("--------------------");
      break;
  }
  
  Serial.println("=========================================");
  Serial.println("");
}
