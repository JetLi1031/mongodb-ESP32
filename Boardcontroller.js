
const {Boardi,Boardvari} = require("./Boardvar.js")


// Boardvari.find({"_id":5}).limit(1).then((myobj) => {console.log(myobj);}).catch((error)=>{console.log(error)})

//show all info with limit number
//GET 
exports.index = (req,res) => {
    Boardvari.find().limit().then((myobj) => {console.log(myobj);res.send(myobj);}).catch((error)=>{console.log(error)})
}
//GET
exports.view = (req,res) => {
    Boardvari.findById(req.params.id).then((myobj) => {console.log(myobj);res.send(myobj);}).catch((error)=>{console.log(error)})

}
//Create by post
exports.new  = (req,res) => {
    let createobj = {
        "_id":req.body.id,
        "Bid":req.body.Bid,
        "Vid":req.body.varid,
        "API":req.body.api,
        "varname":req.body.name,
        "Type":req.body.Type,
        "sample":[req.body.number],
        "rendercomponent":req.body.render
    }
    Boardvari(createobj).save().then((doc) => {  res.json({
        message : "new variable created",
        data : createobj,
        info : doc
    }); console.log(doc)})
    .catch((error)=>{ res.json({
        message : "error",
        errormsg : error
    });console.log(error)})
}

//update by post
exports.update = (req,res) => {
    let updateobject = {
        "_id":req.body.id,
        "Bid":req.body.Bid,
        "Vid":req.body.varid,
        "API":req.body.api,
        "varname":req.body.name,
        "Type":req.body.Type,
        "sample":[req.body.number],
        "rendercomponent":req.body.render
    } 
    Boardvari.findByIdAndUpdate(req.id,updateobject).then((doc) => { res.json({
        message : "new variable created",
        data : createobj,
        info : doc
    }); console.log(doc)})
    .catch((error)=>{ res.json({
        message : "error",
        errormsg : error
    });console.log(error)})
}


exports.delete = (req,res) => {
    Boardvari.remove({"_id":req.params.id}).then((myobj) => {console.log(myobj);}).catch((error)=>{console.log(error)})
}

//to interface with bord {types,boardname,vid,reading,api} 
exports.boardinterface = (req,res) => {   //update only mah (varid) ,(bid) , 
     //remember verify Api  here
     console.log(req.body.Type);
    if(req.body.Type == "Input"){
        let {boardname,reading,vid} = req.body; 
        Boardi.find({"BoardName":boardname},{"Bid":1,"_id":0}).then((myObj)=>{
            Boardvari.updateOne({"VID":vid,"Bid":myObj["Bid"]},{"State":reading}).then((resul) => console.log(resul))
        })
         res.json("updated")
}
/* debugged  {Boardname,Type,} */
if(req.body.Type == "Output"){
    Boardi.findOne({"BoardName":req.body.BoardName},{"Bid":1,"_id":0}).then((myObj)=>{
        Boardvari.find({"Bid":myObj["Bid"]},{"Vid":1,"State":1,"_id":0}).then((myobj) => {
            console.log(myobj); 
            res.json(myobj);})
            .catch((error)=>{console.log(error)})
        // console.log(myObj.Bid)
    })
}

}






// BoardvarModel = mongoose.model('Jesuschris', Boardvar);
// let msg =  boardvar({_id:10,API:"dawdaw",varname:"dawdw",Type:"output",sampleread:[22,33],rendercomponent:"graph"})
// msg.save()
// .then(doc => {
//   console.log(doc)
// })
// .catch(err => {
//   console.error(err)
// })

