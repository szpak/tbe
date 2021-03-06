/* The Butterfly Effect
 * This file copyright (C) 2016 Klaas van Gend
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

Image {
    source: img("ActionDelete")

    //visible: TODO: only for user-inserted objects OR in level creator mode

    width: ResizeInfo.handleWidth
    height: ResizeInfo.handleHeight
    sourceSize.width: width
    sourceSize.height: height

    MouseArea {
        anchors.fill: parent
        onClicked: {
            theDecorator.startNewUndo("HandleDelete", this);
            // delete will only happen on the commit...
            theDecorator.startNewUndo("", null);
        }
    }
}
