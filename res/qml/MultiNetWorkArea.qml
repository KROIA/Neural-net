import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
Item {
    id:multiNetArea
    MultiNetTopBar{
        id:topBar
        anchors.top:multiNetArea.top
        anchors.left: multiNetArea.left
        anchors.right: multiNetArea.right
        height:multiNetArea.height*0.1
    }
    MultiNet{
        anchors.top:topBar.bottom
        anchors.bottom: multiNetArea.bottom
        anchors.left: multiNetArea.left
        anchors.right: multiNetArea.right
        xNetPerTab: topBar.netXAxis
        yNetPerTab: topBar.netYAxis
        updateTime: topBar.updateTimer
        tabIndex: topBar.tabId
    }
}
