import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
Window {
    id:window
    width: 1500
    height: 900
    visible: true
    Definition{
        id:def
    }
    SingelNetWorkArea{
        anchors.fill: parent
        //visible: !topBar.multiView
    }
    /*MultiNetWorkArea{
        anchors.fill:parent
        visible: topBar.multiView
    }/*
    UiIntegratableNet{
        anchors.fill:parent
    }*/
}
