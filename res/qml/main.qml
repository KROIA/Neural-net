import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "NetCreater/" as Creator
import "MultiView/" as Multi
import "SingelView/" as Singel
import "IntegratableNet/" as Integrate

Window {
    id:window
    width: 1500
    height: 900
    visible: true
    Definition{
        id:def
    }
    TabBar{
        id:modusSelector
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        TabButton{
            id:singel
            width: 100
            text: "singel"
        }
        TabButton{
            id:multi
            width: 100
            text: "multi"
        }
        TabButton{
            id:creator
            width: 100
            text: "creator"
        }
        TabButton{
            id:integrate
            width: 100
            text: "integrate"
        }
    }
    Item{
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: modusSelector.bottom

        Singel.Main{
            anchors.fill:parent
            visible: singel.checked
        }
        Creator.Main{
            anchors.fill:parent
            visible: creator.checked
        }/*

        Multi.Main{
            anchors.fill:parent
            visible: multi.checked
        }
        Integrate.Main{
            anchors.fill:parent
            visible: integrate.checked
        }*/
    }
}
