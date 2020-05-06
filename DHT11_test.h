/* --------------- DHT11 Test library ------------
 *  
 *  Used for reading DHT11 devices around board
 *  
 *  
 */

#define DHTTYPE DHT11 
#define DHT11n 8


short dht_read(short i);
void dht_test();
void dht_begin();

short dht11_found[] = {0,0,0,0,0,0,0,0};

DHT dht1(2, DHTTYPE);
DHT dht2(3, DHTTYPE);
DHT dht3(4, DHTTYPE);
DHT dht4(5, DHTTYPE);
DHT dht5(6, DHTTYPE);
DHT dht6(7, DHTTYPE);
DHT dht7(8, DHTTYPE);
DHT dht8(9, DHTTYPE);

DHT dht[] = {dht1, dht2,dht3,dht4,dht5,dht6,dht7,dht8};


void dht_begin(){
   for (int i = 0; i<DHT11n; i++)
   {
     dht[i].begin();
   }
}

void dht_test(){
 for (int i = 0; i<DHT11n; i++)
   {
     delay(100);
     dht11_found[i] = dht_read(i);
   }
   Serial.println();
}

short dht_read(short i) {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht[i].readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht[i].readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht[i].readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 0;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht[i].computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht[i].computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  
  return 1;
}
