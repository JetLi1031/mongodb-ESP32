const mongoose = require('mongoose')
const {Boardi,Boardvari} = require("./Boardvar.js")

const server = '127.0.0.1:27017';
const database =  'IoTPlat';


mongoose.connect(`mongodb://${server}/${database}`, { useNewUrlParser: true});

const CreateBoardlist =[
    {
        _id:1,
        Bid:1,
        APIKey:"ABCD",
        BoardName:"ESP32_01Home",
    }
]


const Createdvarlist = [
    { 
        _id:1,
        Bid:1,
        Vid:4,
        varname:"switch1",
        Type: "Output",
        State: [1], //input:[1/0/0.56] output:[{"Date":"0.543","value":"11.2"},{"Date":"","value"":"""}]
        rendercomponent: "Button"},
    { _id:2,
        Bid:1,
        Vid:19,
        varname:"switch2",
        Type: "Output",
        State: [0], //input:[1/0/0.56] output:[{"Date":"0.543","value":"11.2"},{"Date":"","value"":"""}]
        rendercomponent: "Button"},
    { _id:3,
        Bid:1,
        Vid:22,
        varname:"switch3",
        Type: "Output",
        State: [1], //input:[1/0/0.56] output:[{"Date":"0.543","value":"11.2"},{"Date":"","value"":"""}]
        rendercomponent: "Button"},
    { _id:4,
        Bid:1,
        Vid:15,
        varname:"Pizeobuzzer",
        Type: "Output",
        State: [1000], //input:[1/0/0.56] output:[{"Date":"0.543","value":"11.2"},{"Date":"","value"":"""}]
        rendercomponent: "Slider"},

]



Boardi.insertMany(CreateBoardlist).then(()=> {console.log("sucess")}).catch((err)=> {console.log(err)})

Boardvari.insertMany(Createdvarlist).then(()=> {console.log("sucess")}).catch((err)=> {console.log(err)})