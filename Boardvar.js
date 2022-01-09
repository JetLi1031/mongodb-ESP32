//let start from schema

const mongoose = require('mongoose')
const Schema = mongoose.Schema;

const Board = mongoose.Schema({
    _id:Number,
    Bid:Number,
    APIKey:String,
    BoardName:String,

})

const Boardvar = mongoose.Schema({
    _id:Number,
    Bid:Number,
    Vid:Number,
    varname:String,
    Type: String,
    State: [Schema.Types.Mixed], //input:[1/0/0.56] output:[{"Date":"0.543","value":"11.2"},{"Date":"","value"":"""}]
    rendercomponent: String,
})



module.exports = {
    Boardi :mongoose.model("Board",Board) ,
    Boardvari : mongoose.model("Boardvar",Boardvar) ,
}


