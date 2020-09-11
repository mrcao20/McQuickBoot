import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    property var ws: null
    property int index: 0
    
    function aaa(result) {
        if(index > 100000)
            return;
        console.log("con", index++);
        console.log(result, result.errMsg);
        $.get("con.invoke2").syncThen(function(){
            console.log("aaaaaaaaaa>>>>>>>>>");
        });
//        $.post("con.invoke3", data);
        console.log(Qt.formatDateTime(new Date(), "dddd-yyyy-MM-dd-MMM-hh-mm-ss"));
    }
    
    Component.onCompleted: {
        var data = {
            onOpen: function(){
                console.log("qml onOpen aaaaaaaaa");
                ws.send("bbbbbbbb");
            },
            isOpenSync: true,
            onMessage: function(msg){
                console.log("qml message", msg);
            },
            isMessageSync: true,
            onClose: function() {
                console.log("ccccccccc");
            }
        };
        ws = $.qs("socket", data);
        
        $.get("con1.invoke1").syncThen(function(result) {
            console.log("con1");
            console.log(">>>>>>>>>1.", result, result.errMsg);
        });
        aaa({errMsg: "aaa"});
        $.get("con.invoke2").syncThen(function(result){
            console.log("con");
            console.log(">>>>>>>>>.", result, result.errMsg);
        });
        var data1 = {
            o: {
                text: "aaa",
                t: [{
                        aaa: "bbbb"
                    },
                    {
                        aaa: "ccccc"
                    }
                ],
                tt: {
                    999: [
                        {
                            aaa: "qqqq"
                        }
                    ]
                }
            }
        };

        console.log($.__proto__.post);
        console.log($);
        $.post("con.invoke3", data1);
        
        $.get("app.dirPath").syncThen(function(result){
            console.log("dir path:", result)
        });
        console.log("file path:", $.syncInvoke("app.filePath"));
        
        $.get("con.invoke4").syncThen(function(result) {
            console.log("invoke4>>>>>>>>>", JSON.stringify(result));
            $.post("con.invoke3", {
                       o: result
                   });
        });
    }
}
