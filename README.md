# mongodb-ESP32

ESP32 interface (https://github.com/JetLi1031/mongodb-/blob/main/Arduino/Output/Output.ino )
- ESP32 connect to server repeatedly, to subscribe to new pin and change the pin based on user request state saved in database
- The interface support input and output state from ESP32

mongodb with express rest api ()
- user input boardname and variable id assiciated with board to switch specific pin on board to desired state
- newly created Board varible is requested by ESP32 and change to the requested state
- the reading from ESP32 will be saved to database
- schema https://github.com/JetLi1031/mongodb-/blob/main/Boardvar.js
- restful implementation https://github.com/JetLi1031/mongodb-/blob/main/Boardcontroller.js
{ _id:5,
  Bid:1,
  Vid:23,
  varname:"input1",
  Type: "Input",
  State: {Typeoftype: "Temperature" ,Value:[]}, //input:[1/0/0.56] output:[{"Date":"0.543","value":"11.2"},{"Date":"","value"":"""}]
  rendercomponent: "meter"},
-the output reading is stored  for logging purpose 

