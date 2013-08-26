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

        ModelScene {
            id: scene

            anchors.fill: parent

            rotationX: 45;
            rotationY: -20;

            Model {
                id: caster

                y: -3
                orientation: 90
            }

            Model {
                id: target

                y: 3
                orientation: -90
            }
        }

        SpellVisual {
            id: visual

            caster: caster
            target: target
        }

        MouseArea {
            property real lastX;
            property real lastY;

            anchors.fill: scene

            onPressed: {
                lastX = mouseX;
                lastY = mouseY;
            }

            onPositionChanged: {
                scene.rotationX += 360 * (mouseX - lastX) / width;
                scene.rotationY -= 360 * (mouseY - lastY) / height;

                lastX = mouseX;
                lastY = mouseY;
            }

            onWheel: scene.distance -= wheel.angleDelta.y / 120;
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

                TextField {
                    placeholderText: "Caster"

                    onAccepted: caster.displayId = text
                }

                TextField {
                    placeholderText: "Target"

                    onAccepted: target.displayId = text
                }

                TextField {
                    id: visualId

                    placeholderText: "Visual"

                    onAccepted: cast.clicked()
                }

                Button {
                    id: cast

                    text: "Cast"

                    onClicked: {
                        visual.visual = visualId.text;
                        visual.start();
                    }
                }
            }
        }
    }
}
