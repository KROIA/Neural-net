import QtQuick 2.0

Column {
    property variant conArr: []
    property variant conArrType: []
    property variant conArrID:[]
    property string titel: ""
    property int fontSize: 13
    visible: conArr.length>0
    spacing: 10
    Text {text: "\n"+titel;font.bold: true;font.pointSize: fontSize}
    Row{
        spacing: 25
        Column{
            Text {font.pointSize: fontSize;text: " Id "}
            Repeater{
            model: conArr.length
            Text {
                text: conArrID[conArr[index]]
                font.pointSize: fontSize
            }
            }
        }
        Column{
            Text {text: " Type "}
            Repeater{
            model: conArr.length
            Text {
                text: getTypeString(conArrType[conArr[index]])
                font.pointSize: fontSize
            }
            }
        }
        Column{
            Text {text: " Weight "}
            Repeater{
            model: conArr.length
            Text {
                text: conWeight[conArr[index]]
                font.pointSize:fontSize
                }
            }
        }
    }
}
