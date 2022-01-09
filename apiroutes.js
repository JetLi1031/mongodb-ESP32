const router = require("express").Router()
const controller = require("./Boardcontroller.js")


router.get("/",(req,res)=>{res.json({
    msg :"Welcome to message gateway"
}
)})

router.route("/index").get(controller.index).post(controller.new);

router.route("/id").get(controller.view).patch(controller.update).put(controller.update).delete(controller.delete);

router.route("/interface").post(controller.boardinterface)



module.exports = router;