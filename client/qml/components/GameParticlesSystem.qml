import QtQuick 2.12
import QtQuick.Particles 2.13

import "qrc:/js/graphic_utils.js" as GU

Item {
    id: mainItem

    /*
    Canvas {
        id: cBG
        anchors.fill: parent
        renderStrategy: Canvas.Cooperative
        z: 50
    }

    Canvas {
        id: cFG
        anchors.fill: parent
        renderStrategy: Canvas.Cooperative
        z: 150
    }*/

    ParticleSystem {
        id: psMain
        anchors.fill: parent
        //running: true
        z: 100
    }

    ImageParticle {
        id:  ipLight
        color: "red"
        system: psMain
        source: "qrc:///res/particle.png"
        anchors.fill: parent
        clip: true
        groups: ["light"]
        z: 100
    }

    ImageParticle {
        id:  ipSymbol
        color: "white"
        system: psMain
        source: "qrc:///res/particle.png"
        anchors.fill: parent
        clip: true
        entryEffect: ImageParticle.Scale
        groups: ["symbol"]
        colorVariation: 1
        z: 100
    }

    Emitter {
        id: eCast
        anchors.fill: parent
        system: psMain
        size: 10
        endSize: 50
        enabled: false
        group: "light"
        z: 100

        lifeSpan: 2000

        onEmitParticles: {
            GU.onEmitCastParticles(particles);
        }
    }

    Emitter {
        id: eSymbol
        anchors.fill: parent
        system: psMain
        size: 10
        endSize: 20
        enabled: false
        group: "symbol"
        z: 100

        lifeSpan: 500

        onEmitParticles: {
            GU.onEmitSymbolParticles(particles);
        }
    }

    function applyCoefficient(coefficient) {
        eCast.size *= coefficient;
        eCast.endSize *= coefficient;
        eSymbol.size *= coefficient;
        eSymbol.endSize *= coefficient;
    }

    function updatePointCount(x, y) {
        if ((GU.lastTouchX !== x) || (GU.lastTouchY !== y)) {
            GU.updatePointCount(x, y);
        }
    }

    function onCastTimer() {
        var bc = GU.getCastTouchCount();
        if (bc > 0) {
            eCast.burst(bc);
        }
    }

    function drawSymbol(arr) {
        //console.log(arr);
        GU.arrSymbol = arr;
        eSymbol.burst(arr.length)
    }

    function finishGesture() {
        //console.log("start", GU.arrTouchX, GU.arrTouchY);
        Qt.core.processGesture(GU.arrTouchX, GU.arrTouchY);
        //console.log("finish");
        GU.finishGesture();
    }

    function clear() {
        psMain.stop();
        psMain.start();
    }
}
