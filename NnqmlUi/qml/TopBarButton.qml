import QtQuick 2.12
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
Button {
    id:button
    height: parent.height
    anchors.margins: 5
    background: Rectangle {
            implicitWidth: 100
            implicitHeight: 40
            opacity: enabled ? 1 : 0.3
            border.color: button.down ?  "#800000":"#a52a2a"
            border.width: 1
            radius: 2
            color: button.down ? "#1e90ff":"#191970"
        }
    contentItem: Text {
            font.pointSize: 13
            font.family: "Helvetica";
            font.bold: true
            text: button.text
            opacity: enabled ? 1.0 : 0.3
            color: button.down ?  "#800000":"#a52a2a"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

}
