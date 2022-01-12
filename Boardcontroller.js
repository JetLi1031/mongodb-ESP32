
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
     if(req.body.Type == "Input"){ /* debugged  {Boardname,Type,State:{Typeoftype,Value{Date,reading}}} */
         console.log("this in an input type");
        let BoardId;
        Boardi.findOne({"BoardName":req.body.Boardname},{"Bid":1,"_id":0}).then((myObj)=>{
            req.body.Setting.forEach(element => {
                let {Value,Vid,Typeoftype} = element;           
                    Boardvari.aggregate([{$match: {"Vid": Vid}},{$project: {"Bid": "$Bid",arraysize: {$size: "$State.Value"}}}]).then((filterres) => {  //to find the number of array
                        // console.log(filterres[0]["arraysize"]);
                        if (filterres[0]["arraysize"] >= 500){

                            Boardvari.updateOne({"Vid":Vid,"Bid":myObj["Bid"]},{$pop:{"State.Value":-1}})
                            .then((res)=> {console.log(res)}).catch((err)=> {console.log(err)}); 
                        }
                        if(Value["reading"] != null ){
                        let objbepushed = {"Date":Value["Date"],"reading":Value["reading"]};               
                        Boardvari.findOneAndUpdate({"Vid":Vid,"Bid":myObj["Bid"]},
                            {$push:{"State.Value":objbepushed},$set:{"State.Typeoftype":Typeoftype}} //option can be added
                        ).then((res)=> {console.log(res)}).catch((err)=> {console.log(err)}); 
                    } else {console.log("This is an empty reading !")}
                } );    
            });
            BoardId = myObj["Bid"];;
            // console.log(BoardId);
            Boardvari.find({"Type":"Input","Bid":BoardId},{"_id":0,"Vid":1,"State.Typeoftype":1}).then((resulte) => {
                res.json(resulte);
            }).catch((err) => console.error(err)) 

        });
     }

 
 
    
/* debugged  {Boardname,Type,} */
if(req.body.Type == "Output"){
    console.log(req.body)
    Boardi.findOne({"BoardName":req.body.BoardName},{"Bid":1,"_id":0}).then((myObj)=>{
        Boardvari.find({"Bid":myObj["Bid"]},{"Vid":1,"State":1,"_id":0}).then((myobj) => {
            console.log(myobj); 
            res.json(myobj);})
            .catch((error)=>{console.log(error)})
        // console.log(myObj.Bid)
    })
}

}


