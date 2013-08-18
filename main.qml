import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0

import WoWObjectViewer 1.0

ApplicationWindow {
    title: "WoW Object Viewer"

    visible: true

    Rectangle {
        anchors.top: controls.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        anchors.topMargin: 5

        color: "black"

        Model {
            id: model

            anchors.fill: parent

            animation: animation.value
        }

        MouseArea {
            property real lastX;
            property real lastY;

            anchors.fill: model

            onPressed: {
                lastX = mouseX;
                lastY = mouseY;
            }

            onPositionChanged: {
                model.rotationX += 360 * (mouseX - lastX) / width;
                model.rotationY -= 360 * (mouseY - lastY) / height;

                lastX = mouseX;
                lastY = mouseY;
            }

            onWheel: model.distance -= wheel.angleDelta.y / 120;
        }
    }

    GroupBox {
        id: controls

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5

        title: "Model Control"

        Column {
            anchors.fill: parent

            spacing: 5

            Row {
                spacing: 5

                Button {
                    text: "Load model"

                    onClicked: modelFile.open()
                }

                FileDialog {
                    id: modelFile

                    nameFilters: [ "M2 (*.m2)" ]

                    onAccepted: model.model = fileUrl;
                }

                Button {
                    text: "Load skin #1"

                    onClicked: textureFile1.open()
                }

                FileDialog {
                    id: textureFile1

                    nameFilters: [ "BLP (*.blp)" ]

                    onAccepted: model.setTexture(11, fileUrl);
                }

                Button {
                    text: "Load skin #2"

                    onClicked: textureFile2.open()
                }

                FileDialog {
                    id: textureFile2

                    nameFilters: [ "BLP (*.blp)" ]

                    onAccepted: model.setTexture(12, fileUrl);
                }

                Button {
                    text: "Load skin #3"

                    onClicked: textureFile3.open()
                }

                FileDialog {
                    id: textureFile3

                    nameFilters: [ "BLP (*.blp)" ]

                    onAccepted: model.setTexture(13, fileUrl);
                }
            }

            Row {
                spacing: 5

                Label {
                    anchors.verticalCenter: parent.verticalCenter

                    text: "Animation:"
                }

                SpinBox {
                    id: animation
                }

                Button {
                    id: startStop

                    //anchors.left: parent.left
                    //anchors.right: parent.right

                    states: [
                        State {
                            name: "start"
                            when: !model.animating
                            PropertyChanges { target: startStop; text: "Start" }
                        },
                        State {
                            name: "stop"
                            when: model.animating
                            PropertyChanges { target: startStop; text: "Stop" }
                        }
                    ]

                    onClicked: model.animating = !model.animating
                }
            }
        }
    }
}
