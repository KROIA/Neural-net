import QtQuick 2.0
import QtQuick.Controls 2.13
TopBar {
    property int neuronModus: modus.currentIndex
    Row{
    Switch{
        text: "movable"
    }
    ComboBox {
        id:modus
        model: ["Value", "Function", "Combined"]
    }
    }
}
