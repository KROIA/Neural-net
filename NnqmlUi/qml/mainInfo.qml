import QtQuick.Controls 2.0
import QtQuick 2.12
import QtQuick.Controls 1.4
import QtCharts 2.3
Rectangle{
    id:mainInfo
    NetInCircel{
    zoom:ui.zoom
    }

    property bool sourceLoaded: false
    /*ListView {
        id: root
        focus: true
        anchors.fill: parent
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        highlightMoveDuration: 250
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds

        model: ListModel {
            ListElement {component: "Net.qml"}
            ListElement {component: "ErrorChart.qml"}
        }

        delegate: Loader {
            width: root.width
            height: root.height

            source: component
            asynchronous: true

            onLoaded: sourceLoaded = true
        }
    }*/
}
