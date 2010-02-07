/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "tbe_global.h"
#include "resizinggraphicsview.h"
#include "DrawObject.h"
#include "UndoDeleteCommand.h"
#include "ImageStore.h"

ResizingGraphicsView::ResizingGraphicsView (QWidget* aParent)
	: QGraphicsView(aParent)
{
	setAlignment(Qt::AlignCenter);

	setInteractive(true);
	setDragMode(QGraphicsView::RubberBandDrag);
	setRubberBandSelectionMode(Qt::ContainsItemShape);

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QSizePolicy vpol;
	vpol.setHorizontalPolicy(QSizePolicy::Expanding);
	vpol.setVerticalPolicy(QSizePolicy::Expanding);
	setSizePolicy(vpol);
}


void ResizingGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
	// if we're running out of the widget to the right, let's start the Drag&Drop
	QPoint myPos = event->pos();
	QGraphicsItem* myItem = itemAt(myPos);

	// anything that is a real object
	if (myItem!=NULL && event->buttons()!=Qt::NoButton)
	{
		// OK, here's a dirty trick.
		// If we get here, we hopefully know that myItem is a DrawObject
		DrawObject* myDOPtr = reinterpret_cast<DrawObject*>(myItem);
		BaseObject* myBOPtr = myDOPtr->getBaseObjectPtr();

		// calculate wether we are close to the right border...
		//
		// I need to map the width of the object to the width of the view.
		// Unfortunately, that can only be done for QPoints: a detour :-(
		QPointF myObjWidth = mapFromScene(myBOPtr->getTheWidth()/2.0, 0.0);
		if (myPos.x() + myObjWidth.x() > width()-5)
		{
			UndoDeleteCommand* myCommandPtr =
					new UndoDeleteCommand(myDOPtr, myBOPtr);

			// create MIME data
			QByteArray itemData;
			QDataStream dataStream(&itemData, QIODevice::WriteOnly);
			dataStream << "TBE-lala"; //item->getID();
			QMimeData* myMimeDataPtr = new QMimeData;
			myMimeDataPtr->setData(TBItem::DrawWorldMimeType, itemData);

			// add an icon to the QDrag
			// FIXME/TODO: improve the icon choosing capabilities
			QPixmap myPixmap = ImageStore::getPNGPixmap("SoccerBall")->scaledToWidth(32);
			QDrag *drag = new QDrag(this);
			drag->setMimeData(myMimeDataPtr);
			drag->setHotSpot(QPoint(myPixmap.width()/2, myPixmap.height()/2));
			drag->setPixmap(myPixmap);

			if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
			{
				myCommandPtr->push();
				return;
			}

			delete myCommandPtr;
			myCommandPtr = NULL;
		}
	}

	// no, we still have an object to move around
	QGraphicsView::mouseMoveEvent(event);
}
