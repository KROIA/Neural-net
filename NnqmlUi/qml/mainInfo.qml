import QtQuick.Controls 2.0
import QtQuick 2.12
import QtQuick.Controls 1.4
import QtCharts 2.3
Rectangle{
    id:mainInfo
    property bool sourceLoaded: false
    Net{

    }

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
