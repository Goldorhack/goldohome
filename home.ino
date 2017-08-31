/*
 * Goldo'Home
 * 
 * - Centralized lights
 * - Light level detection and action
 * - Temperature check and clim update
 */

#include "DHT.h"

boolean DEBUG = true;

int LED_SALON = 4;
int LED_WC = 5; 
int LED_SALLEBAIN = 6;
int LED_CHAMBRE1 = 7;
int LED_CHAMBRE2 = 8;
int LED_GARAGE = 9;
int FAN_CLIM = 3;
int DETECTOR_CLIM = 2;

int LEDS[] = {LED_SALON, LED_WC, LED_SALLEBAIN, LED_CHAMBRE1, LED_CHAMBRE2, LED_GARAGE};

DHT dht(DETECTOR_CLIM, DHT11); 

void setup() {

  // Debug ?
  if(DEBUG){
     Serial.begin(9600); 
  }
  
  // Declare all LED as OUTPUT
  for (int current = 0; current < sizeof(LEDS); current++) {
    pinMode(LEDS[current], OUTPUT);
  }

  dht.begin();
}

void loop() {
  /**
   * Check light level
   */
  int light_level = analogRead(A0);
  ledsUpdate(light_level);

  /**
   * Check temperature
   */
  float temperature = dht.readTemperature();
  if (isnan(temperature)) {
    Serial.println("Error when read temperature value");
  } else{
    climUpdate(temperature);  
  }

  
  delay(2000);
}



/**
 * ==========================+++=========================
 *                     Custom Function
 * =======================+++++++++======================
 */



/**
 * powerOnLed()
 * @param int id
 */
void powerOnLed(int id){
  char message[50];
  sprintf(message, "Power on Led %d was send", id);
  Serial.println(message);

  digitalWrite(id, HIGH);
}

/**
 * powerOffLed()
 * @param int id
 */
void powerOffLed(int id){
  char message[50];
  sprintf(message, "Power off Led %d was send", id);
  Serial.println(message);

  digitalWrite(id, LOW);
}

/**
 * powerAll()
 * @param char statut
 */
void powerAll(boolean statut){
  if (statut) {
    Serial.println("Power on all leds");
    for (int current = 0; current < sizeof(LEDS); current++) {
      digitalWrite(LEDS[current], HIGH);
    }
  } else if (statut == false) {
    Serial.println("Power off all leds");
    for (int current = 0; current < sizeof(LEDS); current++) {
      digitalWrite(LEDS[current], LOW);
    }
  }
}

/**
 * ledsUpdate()
 * @param int level
 */
void ledsUpdate(int level){
  char message[50];
  sprintf(message, "Leds update with light level %d", level);
  Serial.println(message);
  
  if (level < 150){
    powerAll(true);
  } else {
    powerAll(false);
  } 
}

/**
 * climUpdate()
 * @param int temperature
 */
void climUpdate(int temperature){
  char message[50];
  sprintf(message, "Clim update with temperature %d", temperature);
  Serial.println(message);  

  if (temperature > 27) {
    Serial.println("FAN ON");
    digitalWrite(FAN_CLIM, HIGH);
  } else {
    Serial.println("FAN OFF");
    digitalWrite(FAN_CLIM, LOW);
  }
  
}
