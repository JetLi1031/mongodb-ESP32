#include <ArduinoJson.h>
char output[100];
void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
while (!Serial) continue;
   
  Serial.print("shit");

}

void loop() {
  // put your main code here, to run repeatedly:
  StaticJsonDocument<200> doc;




  doc["Type"]="Input";
  JsonObject Value = doc.createNestedObject("Value");
  Value["Date"]= "11/22";
  Value["reading"]="5";
  serializeJson(doc,output);
  Serial.println(output);

  
  }
