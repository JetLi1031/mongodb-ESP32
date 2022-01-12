#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Vector.h>
#include "time.h"
#include "DHT.h"

typedef struct pinStatus {
  int8_t channel;
  int8_t pin;
  float frequency;
  uint8_t resolution;
  uint32_t value;
  uint32_t phase;
} pinStatus_t;

pinStatus_t pinstatus[16] = {     //checked unused chanel
  { 0, -1}, { 1, -1},{ 2, -1}, { 3, -1 },
  { 4, -1}, { 5, -1 },{ 6, -1}, { 7, -1},
  { 8, -1}, {9, -1 },{ 10, -1}, { 11, -1},
  {12, -1}, {13, -1 },{ 14, -1}, { 15, -1 }
};

const char* ntpServer = "pool.ntp.org";  const long  gmtOffset_sec = 3600; const int   daylightOffset_sec = 3600; // for time
const char* ssid = "Wong_2.4GHz@unifi"; const char* password = "60127886883@60127882883"; //for Wifi

//Your IP address or domain name with URL path
const char* serverName = "http://192.168.0.105:3000/api/interface"; //server connection

// Update interval time set to 5 seconds
const long interval = 5000; unsigned long previousMillis = 0; String outputsState; //for output schedule
String inputsState;
//vector declareation
const int ELEMENT_COUNT_MAX = 20; 
int storage_array[ELEMENT_COUNT_MAX]; Vector<int> inputlist(storage_array); 
String storage_array2[ELEMENT_COUNT_MAX]; Vector<String> Typeoftypes(storage_array2);
// function declare
void AnalogWrite(int pin ,int PWM);
//void Tone(int pin ,int freq);
String outputLocalTime();

//json declare
StaticJsonDocument<200> doc;  // declared for obhect
char output[1000]; //declare output
char output2[1000]; //declare output

void setup() {

  Serial.begin(115200);  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  // configure time 
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  outputLocalTime();
}

void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
      // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED ){ 
      
//      checkingoutput(serverName);  //debugged

      checkinginput(); 

      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
}

void checkinginput(){
      inputsState = httpPOSTRequest(serverName,"Input");
      Serial.println(inputsState);
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, inputsState);

  // Test if parsing succeeds.
       if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
         return;
  }
    
      Serial.print("JSON object = ");
      serializeJson(doc,Serial);
//      {vid:12,State:{TypeofType:"Analog"},{vid:14}

     for(int i=0;i< doc.size();i++){
      int pin = (doc[i]["Vid"]);
      JsonObject State = doc[i]["State"];
      String Typeoftype = State["Typeoftype"];
      Typeoftypes.push_back(Typeoftype);
      inputlist.push_back(pin);
//      int value = State["Value"][0];
      String msg = "GPIO - " + String(pin) + " read input from "  + Typeoftype ;
      Serial.println(msg) ;
      }
Serial.print("Reading for next round completed");
      
}

void checkingoutput(){
      outputsState = httpPOSTRequest(serverName,"Output");
//      Serial.println(outputsState);
      StaticJsonDocument<1000> doc;
      DeserializationError error = deserializeJson(doc, outputsState);

  // Test if parsing succeeds.
       if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
         return;
  }
    
      Serial.print("JSON object = ");
      serializeJsonPretty(doc,Serial);
    
       //myObject.keys(); //can be used to get an array of all the keys in the object
     for(int i=0;i< doc.size();i++){
      int pin = (doc[i]["Vid"]);
      JsonObject State = doc[i]["State"];
      String Typeoftype = State["Typeoftype"];
      int value = State["Value"][0];
      String msg = "GPIO - " + String(pin) + " set to - " + String(value) + " is a "  + Typeoftype ;
      Serial.println(msg) ;  
      
      
      if(Typeoftype == "Digital"){
        if(value == -1 ){REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);} 
        else {pinMode(pin, OUTPUT); digitalWrite(pin,value); }} 
      else if (Typeoftype == "PWM") {AnalogWrite(pin,value); }
//      else if (Typeoftype == "Tune") {   Tone(pin, value);delay(1000);Tone(pin,0);delay(1000);} 
      else {Serial.println("no result matched output type");}
      }
}

String httpPOSTRequest(const char* serverName, String Type) {
      WiFiClient client;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
   
     //If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode;

      if(Type == "Input"){
         httpResponseCode = http.POST(JsonGenerator());
      } else if (Type == "Output"){
         httpResponseCode = http.POST("{\"BoardName\":\"ESP32_01Home\",\"Type\":\"Output\"}");
        }
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      
      String payload = http.getString();
      Serial.println(payload);    
      // Free resources
      http.end();

  return payload;
}


String JsonGenerator(){
//  Serial.println(inputlist.size());
   StaticJsonDocument<1024> doc;
   doc["Boardname"]="ESP32_01Home";
   doc["Type"]="Input";
   JsonArray arr = doc.createNestedArray("Setting");
   int inputsize = inputlist.size();
  for (int i = 0;i< inputsize;i++){ 
    StaticJsonDocument<200> innerdoc;     
     String condition = Typeoftypes.back(); Typeoftypes.pop_back();
     int pin = inputlist.back(); inputlist.pop_back();
     Serial.println(condition);
     Serial.println(pin);
     
     float reading ;
//     innerdoc["State"]="{\"Date\":\"10/Jan/2022 15:54:23\",\"Value\":15}"
     innerdoc["Vid"]=pin;
     innerdoc["Typeoftype"]=condition;    
     JsonObject Value = innerdoc.createNestedObject("Value");
     Value["Date"]=   outputLocalTime();
     
     
      if(condition == "Digital"){pinMode(pin,INPUT); Value["reading"]= digitalRead(pin);}//digitalRead(pin); } 
      else if (condition == "Analog") {pinMode(pin,INPUT);  Value["reading"]= analogRead(pin);}
      else if (condition == "Temperature") { if(getDHTreading(pin)!="null"){Value["reading"]= getDHTreading(pin);} ;}
      else {Serial.println("no result matched input type");}
     REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE); // disable to prepare to switch 

      Serial.print("loop "); Serial.println(i);
      serializeJson(innerdoc,output);
      Serial.println(output);
      arr.add(innerdoc);
//      arr.add((String)output);
      }

      serializeJson(doc,output2);
    Serial.println(output2);
   return (String)output2;
  }


// Simulate AnalogWrite with 980Hz
void AnalogWrite(int pin ,int PWM){
 
  if (PWM == -1){
  delchannel( pinstatus, pin);
  REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);
  ledcDetachPin(pin); 
  return;
  }
  int channel =  getchannel(pinstatus,pin); //assign free chanel
  if(REG_GET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL) == 0){ // become 2 after first initialize 
  ledcSetup(channel, 980, 8); // 16 channels,freq,resolution in bits
  ledcAttachPin(pin, channel); }
  ledcWrite(channel, PWM);
  return;
}

//void Tone(int pin ,int freq){
//  ledcSetup(1, freq, 8); // 16 channels,freq,resolution in bits (2000-5000)
//  ledcAttachPin(pin, 0);
//  ledcWrite(0, 127);
//}




//Time 
String outputLocalTime()
{
  time_t now;
  char strftime_buf[64];
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "failed to read";
  }
  time(&now);
// Set timezone to China Standard Time
  setenv("TZ", "CST-8", 1);
  tzset();

  localtime_r(&now, &timeinfo); //to convert to selectable format
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  strftime(strftime_buf, sizeof(strftime_buf), "%d/%b/%Y %H:%M:%S", &timeinfo);
  return (String)strftime_buf;
}





// for getting chanel which is free to a pin
int getchannel(pinStatus_t  pinstatus[16], int pinforchecked){
  for(int8_t i=0;i<16;i++){  //when pin was registered, 
    if(pinstatus[i].pin == pinforchecked){  
      return   pinstatus[i].channel;
    }
  }
  for(int8_t i=0;i<16;i++){ //when pin was not register, resgister
    if(pinstatus[i].pin == -1){  
      pinstatus[i].pin = pinforchecked;
      return   pinstatus[i].channel;
    }
  }
}
// for deleting channel when pin set to -1
void delchannel(pinStatus_t  pinstatus[16], int pinfordelete){
  for(int8_t i=0;i<16;i++){
    if(pinstatus[i].pin == pinfordelete){
      pinstatus[i].pin = -1;
    }
  }
}

String getDHTreading(int pin){
   DHT dht(pin, DHT22); 
   float hum = dht.readHumidity();
   float tem =  dht.readTemperature();
   if(isnan(hum) || isnan(tem) ){return "null";}
   
   return( "Humidity: " + String(hum)+ "%  Temperature(C): " + String(tem)
        + "°C  Heat index: " + String(dht.computeHeatIndex(tem, hum, false))+ "°C");
 
}
