import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Component.onCompleted: {
        $.invoke("testController.testMethod").then(function (str) {
            console.debug(str)
        })
        //! 和http的url拼接方式一样，{arg}必须和method2中的参数名一样
        $.invoke("testController.testMethod3?str={0}".format("uri拼接方式调用"))
        var method3Obj = {
            "str": "通过具名object方式调用"
        }
        $.invoke("testController.testMethod3", method3Obj)
        var method3Arr = ["通过参数顺序array方式调用"]
        $.invoke("testController.testMethod3", method3Arr)
        var method4Obj = {
            "vo": {
                "req": "通过具名object方式调用"
            }
        }
        $.invoke("testController.testMethod4", method4Obj)
        var method4Arr = [{
                              "req": "通过参数顺序array方式调用"
                          }]
        $.invoke("testController.testMethod4", method4Arr)
    }
}
