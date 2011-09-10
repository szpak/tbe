/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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

#include <QState>
#include <QTimer>
#include <QSignalTransition>
#include <QPropertyAnimation>
#include "PieMenu.h"
#include "ViewObject.h"

#include <cmath>
#include "tbe_global.h"



void NamedState::onEntry ( QEvent * event )
{
	DEBUG4("SimulationControls-SimState %s onEntry!\n", ASCII(theName));
	QState::onEntry(event);
}




ActionIcon::ActionIcon(ActionType anActionType,
					   const QString & aFileName,
					   bool isEnabled,
					   QGraphicsWidget* aParentPtr)
	: QGraphicsSvgItem(aFileName, aParentPtr)
{
	// we *must* have a parent (PieMenu!)
	Q_ASSERT(aParentPtr!=NULL);


	// we want to scale ourselfs so we are 2/3 of theRadius as width
	// (or height - square!) that allows the centerpiece to be twice
	// as big and everything just touches :-)
	const qreal theRadius = 100.0;
	qreal myCurWidth = boundingRect().width();
	qreal mySmallWidth = 0.5*0.667*theRadius;
	qreal mySmallScale = 2*mySmallWidth/myCurWidth;
	qreal myLargeWidth = 0.5*1.333*theRadius;
	qreal myLargeScale = 2*myLargeWidth/myCurWidth;
	QPointF myOuterPos(theRadius*cos(anActionType*45.0/180.0*PI) - mySmallWidth,
					   -theRadius*sin(anActionType*45.0/180.0*PI) + mySmallWidth);
	QPointF mySInnerPos(-mySmallWidth, 0);
	QPointF myLInnerPos(-myLargeWidth, 0);

	QState* myStartState = new NamedState(&theIconStateMachine, "Start "+aFileName);
	QState* myOuterState = new NamedState(&theIconStateMachine, "Outer "+aFileName);
	QState* myInnerState = new NamedState(&theIconStateMachine, "Inner "+aFileName);
	QState* myActivState = new NamedState(&theIconStateMachine, "Activ "+aFileName);

	theIconStateMachine.setInitialState(myStartState);

	// setup the transition animation
	QPropertyAnimation* myPosAnim = new QPropertyAnimation(this, "pos");
	myPosAnim->setDuration(500);
	myPosAnim->setEasingCurve(QEasingCurve::InOutBack);

	QPropertyAnimation* mySizAnim = new QPropertyAnimation(this, "scale");
	mySizAnim->setDuration(500);
	mySizAnim->setEasingCurve(QEasingCurve::InOutBack);

	// where to go from start...
	QAbstractTransition* myTransPtr;
	if (anActionType != ACTION_MOVE)
		myTransPtr=myStartState->addTransition(aParentPtr, SIGNAL(moveToPositions()), myOuterState);
	else
		myTransPtr=myStartState->addTransition(aParentPtr, SIGNAL(moveToPositions()), myInnerState);
	myTransPtr->addAnimation(myPosAnim);
	myTransPtr->addAnimation(mySizAnim);

	// and the other state transitions
	myTransPtr=myOuterState->addTransition(this, SIGNAL(clicked(ActionIcon*)), myInnerState);
	myTransPtr->addAnimation(myPosAnim);
	myTransPtr->addAnimation(mySizAnim);
	myTransPtr=myInnerState->addTransition(this, SIGNAL(clicked(ActionIcon*)), myActivState);
	myTransPtr->addAnimation(myPosAnim);
	myTransPtr->addAnimation(mySizAnim);
	myTransPtr=myInnerState->addTransition(this, SIGNAL(moveBack()), myOuterState);
	myTransPtr->addAnimation(myPosAnim);
	myTransPtr->addAnimation(mySizAnim);

	connect(this, SIGNAL(clicked(ActionIcon*)), aParentPtr, SLOT(iconClicked(ActionIcon*)));

	// set the positions for the various states
	myStartState->assignProperty(this, "pos", mySInnerPos);
	myStartState->assignProperty(this, "scale", mySmallScale);

	myOuterState->assignProperty(this, "pos", myOuterPos);
	myOuterState->assignProperty(this, "scale", mySmallScale);

	myInnerState->assignProperty(this, "pos", myLInnerPos);
	myInnerState->assignProperty(this, "scale", myLargeScale);

	theIconStateMachine.start();
}


PieMenu::PieMenu(ViewObject* aParentPtr)
	: QGraphicsWidget(aParentPtr)
{
	// nothing to do?
}

void PieMenu::iconClicked(ActionIcon* anIconPtr)
{
	emit theCurrentInnerIconPtr->moveBack();
	theCurrentInnerIconPtr = anIconPtr;
}

void PieMenu::setup()
{
	ActionIcon* myHRezIcon = new ActionIcon(ActionIcon::ACTION_HRESIZE,
											"../images/ActionResizeHori.svg", true, this);
	ActionIcon* myVRezIcon = new ActionIcon(ActionIcon::ACTION_VRESIZE,
											"../images/ActionResizeVerti.svg", true, this);
	ActionIcon* myRotIcon  = new ActionIcon(ActionIcon::ACTION_ROTATE,
											"../images/ActionRotate.svg", false, this);
	ActionIcon* myMoveIcon = new ActionIcon(ActionIcon::ACTION_MOVE,
											"../images/ActionMove.svg", true, this);
	ActionIcon* myDelIcon = new ActionIcon(ActionIcon::ACTION_DELETE,
											"../images/ActionDelete.svg", true, this);
	ActionIcon* mySpecialIcon = new ActionIcon(ActionIcon::ACTION_EDITSPECIAL,
											"../images/ActionDelete.svg", false, this);
	theCurrentInnerIconPtr = myMoveIcon;

	DEBUG1("start\n");
	QTimer::singleShot(401, this, SLOT(startMove()));
	DEBUG1("final\n");
}
