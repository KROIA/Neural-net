import QtQuick.Controls 2.0
import QtQuick 2.12
import QtQuick.Controls 1.4

Button {
    id:button
    height: parent.height
    anchors.margins: 5
    Rectangle{
        anchors.fill: parent
        color: "grey"//"blue"
        radius: 5
        Text {
            id: name
            text: button.text
        }
    }

}
