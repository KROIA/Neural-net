import QtQuick 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
ToggleButtonStyle  {
    id:leftSideButtonStyle
    property int textRotation:0
    background: Rectangle
        {
            implicitWidth: 25
            implicitHeight: 100
            color: control.checked ? "silver" : "darkgrey"
        }
    label: Label {
        color: "black"
        x:0
        y:0
        width: control.height
        height: control.width
        rotation: textRotation
        text: control.text
        transformOrigin:Item.Center
        horizontalAlignment: Text.AlignHCenter
    }
}

