#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "Wong_2.4GHz@unifi";
const char* password = "60127886883@60127882883";

//Your IP address or domain name with URL path
const char* serverName = "http://192.168.0.105:3000/api/interface";

// Update interval time set to 5 seconds
const long interval = 5000;
unsigned long previousMillis = 0;

String outputsState;

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
}

void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
      // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED ){ 
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
      JSONVar keys = myObject[0].keys();

      Serial.println(keys);

     for(int i=0;i< myObject.length();i++){
      int pin = (myObject[i][keys[0]]);
      int value = (myObject[i][keys[1]][0]);
      String msg = "GPIO - " + String(pin) + " set to - " + String(value) ;
      Serial.println(msg) ;  
      pinMode(pin, OUTPUT);
      digitalWrite(pin,value);  
     }

      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
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


