import Mainwindowclass 1.0
import QtQuick.Controls 2.0
import QtQuick 2.13
import QtQuick.Controls 1.4
ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 800
    property variant win;
    Ui{
        id:ui
        property real zoom: 1
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
        width:200
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
