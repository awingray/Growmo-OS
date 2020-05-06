/* --------------- I2C Test library ------------
 *  
 *  Used for reading I2C devices around board
 *  
 *  
 */


#define PWMn 3
#define MUXn 4


void I2C_test();
void PWM_scan();
void MUX_scan();
short Co2_scan();
void mux_set_channel(short i2c_addrs, short channel);
void extended_scan();


short pwm_found[] = {0,0,0};
short mux_found[] = {0,0,0};
short co2_found[] = {0,0,0,0,0,0};

short pwm_addrs[] = {0x40,0x41,0x70};
short mux_addrs[] = {0x71, 0x72, 0x73, 0x74};
short co2_addrs = 0x58; // CCS811-0x5A;

String pwm_name[] = {"PWM1", "PWM2", "PWM Broadcast"}; 
String mux_name[] = {"MUX1","MUX2","MUX3"};

void I2C_test(){
  PWM_scan();
  MUX_scan();
  Serial.println();
}

void PWM_scan(){
  //Scan all PWM available
  for(int i = 0; i < PWMn; i++ ) 
  {
    Wire.beginTransmission(pwm_addrs[i]);
    byte error = Wire.endTransmission();
    
    Serial.print(pwm_name[i]);
    Serial.print(" (0x" + String(pwm_addrs[i],HEX) + ")");
    if (error == 0)
    {
      Serial.println(" found");
      pwm_found[i] = 1;
    }
    else if (error==4) 
    {
      Serial.println(" ERROR");
    }    
    else
    {
      Serial.println(" NOT found");
    }
  }
}

void MUX_scan(){
  //Scan all I2C mux available
  for(int i = 0; i < MUXn; i++ ) 
  {
    Wire.beginTransmission(mux_addrs[i]);
    byte error = Wire.endTransmission();

    Serial.print(mux_name[i]);
    Serial.print(" (0x" + String(mux_addrs[i],HEX) + ")");
    
    if (error == 0)
    {      
      Serial.println(" found");
      mux_found[i] = 1;
      
      //look for co2 channels
      Serial.print("--SGP30 1:");
      mux_set_channel(mux_addrs[i],0);     
      co2_found[i*2] = Co2_scan();
      //extended_scan();

      Serial.print("--SGP30 2:");
      mux_set_channel(mux_addrs[i],1);
      co2_found[i*2+1] = Co2_scan();
      //extended_scan();
    }
    else if (error==4) 
    {
      Serial.println(" ERROR");
    }    
    else
    {
      Serial.println(" NOT found");
    }
      
    //Disable mux channels
    mux_set_channel(mux_addrs[i], 5);
  }
}

short Co2_scan(){
   Wire.beginTransmission(co2_addrs);
   byte error = Wire.endTransmission();
   if (error == 0)
    {
      Serial.println(" found");
      return 1;
    }
    else if (error==4) 
    {
      Serial.println(" ERROR");
      return 0;
    }    
    else
    {
      Serial.println(" NOT found"); 
      return 0;
    }
}

void mux_set_channel(short i2c_addrs, short channel){
  short mux_channel[] = {0x1, 0x2, 0x4, 0x8, 0x0}; //I2C mux channel commands 
  Wire.beginTransmission(i2c_addrs);
  Wire.write(mux_channel[channel]);
  Wire.endTransmission();
  delay(100);
}



void extended_scan()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    //Serial.println(address);

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}
