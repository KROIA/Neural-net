import QtQuick 2.14
import QtQuick.Controls 2.4
Item {
    id:spinBoxItem
    property int to: 10
    property int from: 0
    property int value: spinBox.value
    property string text: ""
    property int spinBoxWidth: 200
    property int defaultValue: from
    width: 300
    height: 100
    Text {
        id:spinText
        font.pixelSize: parent.height*0.3
        text: spinBoxItem.text
        leftPadding:spinBoxWidth*0.05
        anchors.left:parent.left
        anchors.verticalCenter:spinBoxItem.verticalCenter
    }

    SpinBox{
        id:spinBox
        from: spinBoxItem.from
        to:spinBoxItem.to
        editable:true
        width:spinBoxItem.spinBoxWidth
        height: spinBoxItem.height*0.8
        inputMethodHints: Qt.ImhDigitsOnly
        font.pixelSize: height*0.6

        value:{
            value=defaultValue
        }

        anchors.right: spinBoxItem.right
        anchors.verticalCenter:spinBoxItem.verticalCenter
    }
}
