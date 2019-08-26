import QtQuick 2.0

Item {
    id:connectionItem
    anchors.fill:parent
    property int xStart: 0
    property int yStart: 0
    property int xEnd: 0
    property int yEnd: 0
Rectangle{
    id:connectionRect
    property int x1:  if(connectionItem.xStart<connectionItem.xEnd)
                          return connectionItem.xStart
                      else
                          return connectionItem.xEnd
    property int y1: if(connectionItem.xStart<connectionItem.xEnd)
                         return connectionItem.yStart
                     else
                         return connectionItem.yEnd
    property int x2: if(connectionItem.xStart>=connectionItem.xEnd)
                         return connectionItem.xStart
                     else
                         return connectionItem.xEnd
    property int y2: if(connectionItem.xStart>=connectionItem.xEnd)
                         return connectionItem.yStart
                     else
                         return connectionItem.yEnd

    x:x1-((width/2)-(width/2)*Math.cos((Math.PI/180)*rotation))
    y:((y1-y2)/2)+y2
    width:Math.sqrt(Math.pow(x1-x2,2)+Math.pow(y1-y2,2))
    height: 10
    rotation:{
                console.debug(Math.acos((x2-x1)/width)*180/Math.PI)
        if(x1<x2&&y1>y2)
            return -(Math.acos((x2-x1)/width)*180/Math.PI)
        else
            return (Math.acos((x2-x1)/width)*180/Math.PI)
    }
    color: "red"
    }
}
