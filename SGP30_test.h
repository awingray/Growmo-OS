

Adafruit_SGP30 sgp;

void sgp_read_all();
uint32_t getAbsoluteHumidity(float temperature, float humidity);
void sgp_begin_one(short I2C_mux, short sgp_ch);
void sgp_read_one(short I2C_mux, short sgp_ch);


/* return absolute humidity [mg/m^3] with approximation formula
* @param temperature [°C]
* @param humidity [%RH]
*/



uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}

void sgp_begin_one(short I2C_mux, short sgp_ch) {
 
  Serial.println("SGP begin");
  Serial.print("MUX: 0x");
  Serial.print(I2C_mux, HEX);
  Serial.println(" - Channel: " + String(sgp_ch));
  
  //!IMPORTANT: Set the mux to the device you want to read
  mux_set_channel(I2C_mux, sgp_ch);
  
  if (! sgp.begin()){
    Serial.println("Sensor not found :(");
    Serial.println("");
    //while (1);
  }
  else{
    Serial.print("Found SGP30 serial #");
    Serial.print(sgp.serialnumber[0], HEX);
    Serial.print(sgp.serialnumber[1], HEX);
    Serial.println(sgp.serialnumber[2], HEX);
    Serial.println("");
  }
  // If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  //sgp.setIAQBaseline(0x8E68, 0x8F41);  // Will vary for each sensor!

  //!IMPORTANT: set mux to a channel withouth an sgp sensor (so there is no conflict between two sensors)
  mux_set_channel(I2C_mux, 5);
}


void sgp_read_one(short I2C_mux, short sgp_ch) {

  Serial.println("SGP reading");
  Serial.print("MUX: 0x");
  Serial.print(I2C_mux, HEX);
  Serial.println(" - Channel: " + String(sgp_ch));
  
  //!IMPORTANT: Set the mux to the device you want to read
  mux_set_channel(I2C_mux, sgp_ch);


/* // If you have a temperature / humidity sensor, you can set the absolute humidity to enable the humditiy compensation for the air quality signals
  float temperature = 22.1; // [°C]
  float humidity = 45.2; // [%RH]
  sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));
*/

  if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    Serial.println("");
    return;
  }
  Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");

  if (! sgp.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    return;
  }
  Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
  Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");
  Serial.println("");
 
  delay(1000);
  
/*   // IF you want to get baseline readings use this but keep in mind that multiple sensors are used
    static int counter = 0;
    counter++;
    if (counter == 30) {
      counter = 0;
  
      uint16_t TVOC_base, eCO2_base;
      if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
        Serial.println("Failed to get baseline readings");
        return;
      }
      Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
      Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
    }
*/
  
  //!IMPORTANT: set mux to a channel withouth an sgp sensor (so there is no conflict between two sensors)
  mux_set_channel(I2C_mux, 5);
}


void sgp_read_all(){
  sgp_read_one(mux_addrs[0], 0);  
  delay(1000);
  sgp_read_one(mux_addrs[0], 1);
  delay(1000);
  sgp_read_one(mux_addrs[1], 0);
  delay(1000);
  sgp_read_one(mux_addrs[1], 1);
  delay(1000);
  sgp_read_one(mux_addrs[2], 0);
  delay(1000);
  sgp_read_one(mux_addrs[2], 1);
  delay(1000);
  sgp_read_one(mux_addrs[3], 0);
  delay(1000);
  sgp_read_one(mux_addrs[3], 1);
  delay(1000);
}


void sgp_begin_all(){
  sgp_begin_one(mux_addrs[0], 0);  
  delay(1000);
  sgp_begin_one(mux_addrs[0], 1);
  delay(1000);
  sgp_begin_one(mux_addrs[1], 0);
  delay(1000);
  sgp_begin_one(mux_addrs[1], 1);
  delay(1000);
  sgp_begin_one(mux_addrs[2], 0);
  delay(1000);
  sgp_begin_one(mux_addrs[2], 1);
  delay(1000);
  sgp_begin_one(mux_addrs[3], 0);
  delay(1000);
  sgp_begin_one(mux_addrs[3], 1);
  delay(1000);
}
