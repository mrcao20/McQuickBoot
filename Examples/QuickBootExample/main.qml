import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Component.onCompleted: {
        $.invoke("controller2.method1").then(function(){
            console.log("qml请求method1完成");
        });
        //! 和http的url拼接方式一样，{arg}必须和method2中的参数名一样
        $.invoke("controller2.method2?arg={0}".format("qml请求method2参数")).then(function(ret){
            console.log("qml请求method2完成", ret);
        });
        var method2Obj = {
            arg: "qml请求method2参数"
        };
        $.invoke("controller2.method2", method2Obj).then(function(ret){
            console.log("qml请求method2完成", ret);
        });

        var method4Obj = {
            vo: {
                req: "qml请求method4参数"
            }
        }
        $.invoke("controller2.method4", method4Obj).then(function(ret){
            console.log("qml请求method4完成", ret.ret);
        });
    }
}
