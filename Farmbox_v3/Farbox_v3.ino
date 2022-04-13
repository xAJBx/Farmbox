#include <Ethernet.h>
EthernetClient client;
#include <ArduinoJson.h>
byte mac[] = { 0x00, 0xAB, 0xBC, 0xCC, 0xDE, 0x01 }; // RESERVED MAC ADDRESS


#include <EEPROM.h>

//Eporom calibration addresses
int AirValueEPROMaddr62 = 0;
int WaterValueEPROMaddr62 = 1;

int AirValueEPROMaddr63 = 2;
int WaterValueEPROMaddr63 = 3;

int AirValueEPROMaddr64 = 4;
int WaterValueEPROMaddr64 = 5;

int AirValueEPROMaddr65 = 6;
int WaterValueEPROMaddr65 = 7;


//Read cal numbers
int AirValue62 = EEPROM.read(AirValueEPROMaddr62) * 4; 
int WaterValue62 = EEPROM.read(WaterValueEPROMaddr62) * 4; 

int AirValue63 = EEPROM.read(AirValueEPROMaddr63) * 4; 
int WaterValue63 = EEPROM.read(WaterValueEPROMaddr63) * 4; 

int AirValue64 = EEPROM.read(AirValueEPROMaddr64) * 4; 
int WaterValue64 = EEPROM.read(WaterValueEPROMaddr64) * 4; 

int AirValue65 = EEPROM.read(AirValueEPROMaddr65) * 4; 
int WaterValue65 = EEPROM.read(WaterValueEPROMaddr65) * 4; 



int soilMoistureValue = 0;
int soilmoisturepercent=0;

int callibrate62dry = 22;
int callibrate62wet = 24;

int callibrate63dry = 26;
int callibrate63wet = 28;

int callibrate64dry = 30;
int callibrate64wet = 32;

int callibrate65dry = 34;
int callibrate65wet = 36;
   
void setup() {
  Serial.begin(9600);
  
  // initialize the Ethernet shield using DHCP:
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to obtaining an IP address using DHCP");
      while(true);
    }
    
  delay(1000);

  Serial.println("connecting...");

  if (client.connect("10.20.30.192", 5000)) {
    Serial.println("connected");
  } else {
    Serial.println("connection failed");
  }

      
  pinMode(callibrate62dry, INPUT_PULLUP);
  pinMode(callibrate62wet, INPUT_PULLUP);

  pinMode(callibrate63dry, INPUT_PULLUP);
  pinMode(callibrate63wet, INPUT_PULLUP);

  pinMode(callibrate64dry, INPUT_PULLUP);
  pinMode(callibrate64wet, INPUT_PULLUP);

  pinMode(callibrate65dry, INPUT_PULLUP);
  pinMode(callibrate65wet, INPUT_PULLUP);
}

void loop() {


  // callibrate62dry = 0
  while(!digitalRead(callibrate62dry)){
    AirValue62 = analogRead(62);
    Serial.print("62 dry raw: ");
    Serial.println(AirValue62);
    delay(1000);
    EEPROM.write(AirValueEPROMaddr62, AirValue62/4);
  }
  while(!digitalRead(callibrate62wet)){
    WaterValue62 = analogRead(62);
    Serial.print("62 wet raw: ");
    Serial.println(WaterValue62);
    delay(1000);
    EEPROM.write(WaterValueEPROMaddr62, WaterValue62/4);
  }

  while(!digitalRead(callibrate63dry)){
    AirValue63 = analogRead(63);
    Serial.print("63 dry raw: ");
    Serial.println(AirValue63);
    delay(1000);
    EEPROM.write(AirValueEPROMaddr63, AirValue63/4);
  }
  while(!digitalRead(callibrate63wet)){
    WaterValue63 = analogRead(63);
    Serial.print("63 wet raw: ");
    Serial.println(WaterValue63);
    delay(1000);
    EEPROM.write(WaterValueEPROMaddr63, WaterValue63/4);
  }

  while(!digitalRead(callibrate64dry)){
    AirValue64 = analogRead(64);
    Serial.print("64 dry raw: ");
    Serial.println(AirValue64);
    delay(1000);
    EEPROM.write(AirValueEPROMaddr64, AirValue64/4);
  }
  while(!digitalRead(callibrate64wet)){
    WaterValue64 = analogRead(64);
    Serial.print("64 wet raw: ");
    Serial.println(WaterValue64);
    delay(1000);
    EEPROM.write(WaterValueEPROMaddr64, WaterValue64/4);
  }
  
  while(!digitalRead(callibrate65dry)){
    AirValue65 = analogRead(65);
    Serial.print("65 dry raw: ");
    Serial.println(AirValue65);
    delay(1000);
    EEPROM.write(AirValueEPROMaddr65, AirValue65/4);
  }
  while(!digitalRead(callibrate65wet)){
    WaterValue65 = analogRead(65);
    Serial.print("65 wet raw: ");
    Serial.println(WaterValue65);
    delay(1000);
    EEPROM.write(WaterValueEPROMaddr65, WaterValue65/4);
  }

  

  int post_delay = 2500;

 
  Serial.print("moister 62: ");
  Serial.println(get_moister(62, AirValue62, WaterValue62));
  post_data("1", get_moister(62, AirValue62, WaterValue62));
  delay(1000);

  Serial.print("moister 63: ");
  Serial.println(get_moister(63, AirValue63, WaterValue63));
  post_data("2", get_moister(63, AirValue63, WaterValue63));
  delay(1000);

  Serial.print("moister 64: ");
  Serial.println(get_moister(64, AirValue64, WaterValue64));
  post_data("3", get_moister(64, AirValue64, WaterValue64));
  delay(1000);

  Serial.print("moister 65: ");
  Serial.println(get_moister(65, AirValue65, WaterValue65));
  post_data("4", get_moister(65, AirValue65, WaterValue65));
  delay(1000);


   delay(900000); // 15 min
}


int post_data(String instrument_Id, int instrument_value){
  if (client.connect("10.20.30.192",5000)) {
    //POST request
    Serial.println("Begin POST Request");
  
    client.println("POST /api/iot/sensordata HTTP/1.1");
      Serial.println("POST /api/iot/sensordata HTTP/1.1");
  
    client.println("Host: 10.20.30.192");
      Serial.println("Host: 10.20.30.192");
    //client.println("User-Agent: Arduino/1.0");
    //  Serial.println("User-Agent: Arduino/1.0");
  
    client.println("Content-Type: application/json");
      Serial.println("Content-Type: application/json");
  
    client.println("Connection: keep-alive");
      Serial.println("Connection: keep-alive");
    
    String jsonpacket = "{\"sensorId\":" + instrument_Id + ",\"data\":" + instrument_value + "}";
    client.print("Content-Length: ");
      Serial.print("Content-Length: ");
    client.println(jsonpacket.length());
      Serial.println(jsonpacket.length());
      
    client.println();
    client.println(jsonpacket);
    Serial.println(jsonpacket);
    Serial.println();
    //To let me know that request has been completed
    Serial.println("Sent POST Request");
  
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
}

int get_moister(int pinNum, int AirValue, int WaterValue){
  soilMoistureValue = analogRead(pinNum);  //put Sensor insert into soil
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  return soilmoisturepercent;
}
  
