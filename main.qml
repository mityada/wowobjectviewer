import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0

import WoWObjectViewer 1.0

ApplicationWindow {
    title: "WoW Object Viewer"

    visible: true

    Item {
        anchors.top: controls.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        anchors.topMargin: 5

        Model {
            id: model

            anchors.fill: parent
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

                TextField {
                    id: displayId

                    placeholderText: "Display ID"

                    onAccepted: loadCreature.clicked()
                }

                Button {
                    id: loadCreature

                    text: "Load creature"

                    onClicked: model.loadCreatureModel(displayId.text);
                }
            }

            Row {
                spacing: 5

                Label {
                    anchors.verticalCenter: parent.verticalCenter

                    text: "Animation:"
                }

                TextField {
                    id: animation

                    text: "0"

                    onAccepted: play.clicked()
                }

                Button {
                    id: play

                    text: "Play"

                    onClicked: model.animation = animation.text;
                }
            }
        }
    }
}
