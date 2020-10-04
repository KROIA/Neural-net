import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import "../Basic"
import "../"
Item {
    id:multiNetArea
    width: 100
    height: 100
    TopBar{
        id:topBar
        anchors.top:multiNetArea.top
        anchors.left: multiNetArea.left
        anchors.right: multiNetArea.right
        height: 70
    }
    MultiNet{
        anchors.top:topBar.bottom
        anchors.bottom: multiNetArea.bottom
        anchors.left: multiNetArea.left
        anchors.right: multiNetArea.right
        xNetPerTab: generalVariable.netXAxis
        yNetPerTab: generalVariable.netYAxis
        updateModus: generalVariable.updateModus
        updateInterval: generalVariable.updateInterval
        tabIndex: generalVariable.tabId
        visuNeuronModus: generalVariable.visuNeuronModus
    }
}
