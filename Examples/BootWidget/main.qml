import QtQuick 2.0

Item {
    Text {
        id: name
        anchors.fill: parent
        text: qsTr("text")
    }
    Component.onCompleted: {
        $.get("app.filePath").then(function(path){
            console.log(path);
        });
    }
}
