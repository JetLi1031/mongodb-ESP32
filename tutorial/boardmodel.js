// var MongoClient = require('mongodb').MongoClient;
// var url = "mongodb://127.0.0.1:27017/";

// function printSchema(obj) {
//     for (var key in obj) {
//         console.log( key, typeof obj[key]) ;
//     }
// };   

// async function Connecting (MongoClient) {
//     try{
//         db = await MongoClient.connect(url);
//         result= await db.db("IoTPlat").collection("boardvar").findOne({});
//         printSchema(result);
//     }
//     catch(error) {console.log(error);} 
   
// }

// Connecting(MongoClient);




const mongoose = require('mongoose')

const server = '127.0.0.1:27017';
const database =  'IoTPlat'

mongoose.connect(`mongodb://${server}/${database}`)
.then(() => {
  console.log('Database connection successful')
})
.catch(err => {
  console.error('Database connection error')
})

const Boardvar = mongoose.Schema({
    _id:Number,
    API:String,
    varname:String,
    Type: String,
    sampleread: Array,
    rendercomponent: String,
})
    BoardvarModel = mongoose.model('Jesuschris', Boardvar);
    let msg =  BoardvarModel({_id:5,API:"dawdaw",varname:"dawdw",Type:"output",sampleread:[22,33],rendercomponent:"graph"})
    msg.save()
    .then(doc => {
      console.log(doc)
    })
    .catch(err => {
      console.error(err)
    })

  
 