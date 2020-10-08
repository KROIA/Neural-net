import QtQuick 2.0
import QtQuick.Controls 2.13
import "../Basic"
Item {
    id: element
    Topbar{
        id:topbarCreator
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 100
        netId: net.netId
    }

    Net{
        id:net
        anchors.top: topbarCreator.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        moveable:topbarCreator.moveable
        onNetIdChanged: {
            topbarCreator.netId=net.netId
        }
        Component.onCompleted: {
            var a=netListVisu.addNewNet()
            netId=a
            net.updateNetStrucVisu(netId)
            net.updateNetData(netId)
        }
        changableInput: true
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
