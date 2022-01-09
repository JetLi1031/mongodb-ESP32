#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Vector.h>
#include <ArduinoSTL.h>

Vector<int> values;
const char* ssid = "Wong_2.4GHz@unifi";
const char* password = "60127886883@60127882883";

//Your IP address or domain name with URL path
const char* serverName = "http://192.168.0.105:3000/api/interface";

// Update interval time set to 5 seconds
const long interval = 5000;
unsigned long previousMillis = 0;
String outputsState;
//vector declareation
const int ELEMENT_COUNT_MAX = 20;
int storage_array[ELEMENT_COUNT_MAX];
Vector<int> vector(storage_array);


void setup() {
  Serial.begin(115200);
  vector.push_back(15);
  Serial.print(find(vector.begin(), vector.end(), 15) != vector.end()))
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
      // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED ){ 
      
      checkingoutput(serverName);

      checkinginput(vector);

      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
}

void checkinginput(Vector<int> value){
      outputsState = httpPOSTRequest(serverName);
      Serial.println(outputsState);
      JSONVar myObject = JSON.parse(outputsState);
  
   vector.push_back(77);
}

void checkingoutput(const char* serverName){
      outputsState = httpPOSTRequest(serverName);
      Serial.println(outputsState);
      JSONVar myObject = JSON.parse(outputsState);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);
    
       //myObject.keys(); //can be used to get an array of all the keys in the object
     for(int i=0;i< myObject.length();i++){
      int pin = (myObject[i]["Vid"]);
      JSONVar State = myObject[i]["State"];
      String Typeoftype = JSON.stringify(State["Typeoftype"]);
      int value = State["Value"][0];
      String msg = "GPIO - " + String(pin) + " set to - " + String(value) + " is a"  + Typeoftype ;
      Serial.println(msg) ;  
      pinMode(pin, OUTPUT);

      if(Typeoftype == "Digital"){digitalWrite(pin,value); } 
      else if (Typeoftype == "Analog") {AnalogWrite(pin,value);}
      else if (Typeoftype == "Tune") {   Tone(pin, value);delay(1000);Tone(pin,0);delay(1000);}
      else {Serial.print("no result matched");}
      }
}

String httpPOSTRequest(const char* serverName) {
      WiFiClient client;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
   
     //If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"BoardName\":\"ESP32_01Home\",\"Type\":\"Output\"}");
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      
      String payload = http.getString();
      Serial.println(payload);    
      // Free resources
      http.end();

  return payload;
}


void AnalogWrite(int pin ,int PWM){
  ledcSetup(0, 5000, 8); // 16 channels,freq,resolution in bits
  ledcAttachPin(pin, 0);
  ledcWrite(0, PWM);
}

void Tone(int pin ,int freq){
  ledcSetup(1, freq, 8); // 16 channels,freq,resolution in bits (2000-5000)
  ledcAttachPin(pin, 0);
  ledcWrite(0, 127);
}
