import QtQuick 2.0
import QtQuick.Controls 2.13
TopBar {
    property int visuNeuronModus: modus.currentIndex
    property bool moveable: moveSwitch.checked
    Row{
    Switch{id:moveSwitch
        text: "moveable"
    }
    ComboBox {
        id:modus
        model: ["Value", "Function", "Combined"]
    }
    }
}
