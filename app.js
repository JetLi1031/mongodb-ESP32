const express = require("express");
const app = express();
const bodyParser = require('body-parser');
// const mongoose = require('mongoose')
const mongoose = require('mongoose');


const server = '127.0.0.1:27017';
const database =  'IoTPlat';


mongoose.connect(`mongodb://${server}/${database}`, { useNewUrlParser: true});


// .then(() => {
//   console.log('Database connection successful')
// })
// .catch(err => {
//   console.error('Database connection error')
// })

(!mongoose.connection)?console.log("error"):console.log("connected to db");


app.use(bodyParser.urlencoded({extended:true}));
app.use(bodyParser.json());
app.use("/api",require("./apiroutes.js"));
app.get("/",(req,res)=> (res.json("Welcome to my page")));
app.listen("3000",()=> {console.log("connected to web")});