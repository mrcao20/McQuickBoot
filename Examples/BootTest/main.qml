import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
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
        $.get("con.invoke2").then(function(){
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
            onMessage: function(msg){
                console.log("qml message", msg);
            },
            onClose: function() {
                console.log("ccccccccc");
            }
        };
//        ws = $.qs("socket", data);
        
        $.get("con.invoke1?aaa=b");
        $.get("con.invoke22").then(function(res){
            console.log("invoke22", JSON.stringify(res));
        });
        $.get("con1.invoke1?aaa=b").then(function(result) {
            console.log("con1");
            console.log(">>>>>>>>>1.", JSON.stringify(result));
        });
//        aaa({errMsg: "aaa"});
        $.get("con.invoke2").then(function(result){
            console.log("con");
            console.log(">>>>>>>>>.", result, result.errMsg);
        });
        console.log("sync invoke", JSON.stringify($.syncInvoke("con.invoke4")));
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
                },
                t2: {
                    text2: "text2"
                }
            }
        };

//        console.log($.__proto__.post);
//        console.log($);
        $.post("con.invoke3", data1);
        $.post("con.invoke5", data1).then(function(res){
            console.log("invoke5", JSON.stringify(res));
        });
        $.post("con.invoke6", data1).then(function(res){
            console.log("invoke6", JSON.stringify(res));
        });
        
//        $.get("app.dirPath").then(function(result){
//            console.log("dir path:", result)
//        });
//        console.log("file path:", $.syncInvoke("app.filePath"));
        
//        $.get("con.invoke4").then(function(result) {
//            console.log("invoke4>>>>>>>>>", JSON.stringify(result));
//            $.post("con.invoke3", {
//                       o: result
//                   });
//        });
    }
    
    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        
        Page1 {
        }
        
        Page {
            Label {
                text: qsTr("Second page")
                anchors.centerIn: parent
            }
        }
    }
    
    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("First")
        }
        TabButton {
            text: qsTr("Second")
        }
    }
}
