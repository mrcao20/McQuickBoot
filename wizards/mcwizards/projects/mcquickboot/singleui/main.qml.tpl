import QtQuick %{QtQuickVersion}
import QtQuick.Controls %{QtQuickControlsVersion}
@if %{UseVirtualKeyboard}
import %{QtQuickVirtualKeyboardImport}
@endif

ApplicationWindow {
    id: window
    
    visible: true
    width: 640
    height: 480
    title: qsTr("%{ProjectName}")
    objectName: "mainApp"
    
    function activeApp() {
        window.show();
        window.raise();
        window.requestActivate();
    }
@if %{UseVirtualKeyboard}

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
@endif
}
