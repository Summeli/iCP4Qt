import QtQuick 1.0

Rectangle {
    width: 360
    height: 360
    Button {
        text: "connect"
        anchors.centerIn: parent
        MouseArea {
            anchors.fill: parent
            onClicked: {
                c_tester.connect();
                //Qt.quit();
            }
        }
    }
}
