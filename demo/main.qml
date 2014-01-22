import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0

import WoWObjectViewer 1.0

ApplicationWindow {
    title: "WoW Object Viewer"

    width: 800
    height: 600

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
            rotationY: 20;
        }

        SpellVisual {
            id: visual

            scene: scene
        }

        MouseArea {
            id: mouseArea

            property real lastX;
            property real lastY;

            property real lastWorldX;
            property real lastWorldY;

            property Model selectedModel;

            property bool selectingCaster;
            property bool selectingTarget;

            anchors.fill: scene

            acceptedButtons: Qt.LeftButton | Qt.RightButton
            hoverEnabled: true

            onPressed: {
                selectedModel = scene.selectedModel;

                lastX = mouseX;
                lastY = mouseY;

                lastWorldX = scene.worldX;
                lastWorldY = scene.worldY;
            }

            onReleased: {
                selectedModel = null;
            }

            onClicked: {
                if (mouse.button == Qt.LeftButton) {
                    if (selectingCaster) {
                        visual.caster = scene.selectedModel;
                        selectingCaster = false;
                    }

                    if (selectingTarget) {
                        visual.target = scene.selectedModel;
                        selectingTarget = false;
                    }
                }

                if (mouse.button == Qt.RightButton) {
                    scene.removeModel(scene.selectedModel);
                }
            }

            onPositionChanged: {
                scene.mouseX = mouseX;
                scene.mouseY = mouseY;

                if (mouse.buttons & Qt.LeftButton) {
                    if (!selectedModel) {
                        scene.rotationX += 360 * (mouseX - lastX) / width;
                        scene.rotationY += 360 * (mouseY - lastY) / height;
                    } else {
                        selectedModel.x += scene.worldX - lastWorldX;
                        selectedModel.y += scene.worldY - lastWorldY;

                        if (selectedModel.x > 10.0)
                            selectedModel.x = 10.0;
                        else if (selectedModel.x < -10.0)
                            selectedModel.x = -10.0;

                        if (selectedModel.y > 10.0)
                            selectedModel.y = 10.0;
                        else if (selectedModel.y < -10.0)
                            selectedModel.y = -10.0;
                    }

                    lastX = mouseX;
                    lastY = mouseY;

                    lastWorldX = scene.worldX;
                    lastWorldY = scene.worldY;
                }
            }

            onWheel: {
                if (selectedModel) {
                    selectedModel.orientation += wheel.angleDelta.y / 8;
                } else {
                    scene.distance -= wheel.angleDelta.y / 120;
                }
            }
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
                id: row

                spacing: 5

                TextField {
                    id: displayId

                    placeholderText: "Display ID"

                    onAccepted: add.clicked()
                }

                Button {
                    id: add

                    text: "Add"

                    onClicked: {
                        var model = Qt.createQmlObject('import WoWObjectViewer 1.0; Creature {displayId: ' + displayId.text + ';}', scene);
                        scene.addModel(model);
                    }
                }

                TextField {
                    id: visualId

                    placeholderText: "Visual"

                    onAccepted: cast.clicked()
                }

                Button {
                    text: "Set caster"

                    onClicked: mouseArea.selectingCaster = true;
                }

                Button {
                    text: "Set target"

                    onClicked: mouseArea.selectingTarget = true;
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

            Row {
                id: characterRow

                spacing: 5

                TextField {
                    id: race
                    placeholderText: "Race"
                }

                TextField {
                    id: gender
                    placeholderText: "Gender"
                }

                Button {
                    text: "Create"

                    onClicked: {
                        var character = Qt.createQmlObject('import WoWObjectViewer 1.0; Character {race: ' + race.text + '; gender: ' + gender.text + ';}', scene);
                        scene.addModel(character);
                    }
                }
            }
        }
    }
}
