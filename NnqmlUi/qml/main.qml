import Mainwindowclass 1.0
import QtQuick.Controls 2.0
import QtQuick 2.12
import QtQuick.Controls 1.4
import "jsSource.js" as MyScript

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 400
    property variant win;
    Ui{
        id:ui
        property bool setNewCon: false
        property real zoom: 1
        property int ansicht: 0
        /*ListModel {
                id: activationFunc
                ListElement { text: "Linear"}
                ListElement { text: "ReLu"}
                ListElement { text: "Binary"}
                ListElement { text: "Gaussian"}
                ListElement { text: "Sigmoid"}
            }*/
    }
    MainInfo{
        id:mainInfo
        y:topBar.height
        width: window.width-sideBar.width
        height: window.height-topBar.height
        anchors.top: topBar.bottom
        anchors.right: sideBar.left
        anchors.left: window.left
        anchors.bottom: window.bottom
    }

    SideBar{
        id:sideBar
        width:1
        height: window.height-topBar.height
        x: window.width-sideBar.width
        y: topBar.height
        color: "lightgrey"
    }
    TopBar{
        id:topBar
        width: window.width
        height: 75
        anchors.top: window.top
    }


}
