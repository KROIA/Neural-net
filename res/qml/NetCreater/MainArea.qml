import QtQuick 2.0
import "../Basic"

Net{
    netId:0
    Component.onCompleted: {
        var a=netListVisu.addNewNet()
        console.debug(a)
        netId=a
    }
}

