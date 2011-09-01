/* This is from Qt reference documentation */

import QtQuick 1.0

Rectangle {
    property alias text: textItem.text

    width: 150;
    height: 70
    border.width: 1
    radius: 5
    smooth: true
    anchors.bottom: parent.bottom

    gradient: Gradient {
        GradientStop { position: 0.0; color: "darkGray" }
        GradientStop { position: 0.5; color: "black" }
        GradientStop { position: 1.0; color: "darkGray" }
    }

    Text {
        id: textItem
        anchors.centerIn: parent
        font.pointSize: 18
        color: "white"
    }

}
