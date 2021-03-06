/* The Butterfly Effect
 * This file copyright (C) 2016  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

import QtQuick 2.0
import TBEView 1.0


// This is the QML representation of the ViewWorldItem instance, that talks
// directly with the World instance.
// The ViewWorldItem instantiates all ViewObjects from C++.
ViewWorldItem {

    function insertGradient(aList) {
        backgroundcanvas.gradientList = aList;
    }

    // This rectangle is just for the backdrop. For now, that's only gradients.
    // Hopefully re-allow a picture image backdrop in the near future. That was
    // disabled back in the milestone 8 times due to performance issues...
    Rectangle {
        border.color: "black"
        anchors.fill: parent
        Canvas {
            id: backgroundcanvas
            property var gradientList
            z: 0.0
            anchors.fill: parent
            anchors.topMargin: 1
            anchors.leftMargin: 1
            anchors.rightMargin: 1
            anchors.bottomMargin: 1
            onPaint: {
                var ctx
                ctx = backgroundcanvas.getContext('2d');
                ctx.beginPath();
                var gradient = ctx.createLinearGradient(50, backgroundcanvas.height, 50, 0);
                for (var colstop in gradientList) {
                    gradient.addColorStop(colstop, gradientList[colstop]);
                }
                ctx.fillStyle = gradient;
                ctx.fillRect(0,0,backgroundcanvas.width,backgroundcanvas.height);
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true;
        onPositionChanged: {
            GameFlow.slot_mouse_move(xwh2m(mouse.x), y2m(mouse.y));
        }
    }
}
