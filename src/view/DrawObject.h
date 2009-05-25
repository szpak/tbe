/* The Butterfly Effect 
 * This file copyright (C) 2009  Klaas van Gend
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

#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include "ode/ode.h"
#include <QGraphicsItem>

// forward declarations
class BaseObject;


/** class DrawObject
  *
  * This class abstracts the actual drawing of objects
  * 
  */

class DrawObject : public QGraphicsItem
{
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	DrawObject (BaseObject* aBaseObjectPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawObject ( );

	/// overriden from QGraphicsItem
    virtual QRectF boundingRect() const;

    /// overriden from QGraphicsItem
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

protected:
	/// overriden from QGraphicsItem
    void advance(int step);

protected:
    // Protected attribute accessor methods
	//  

	/**
	 * Set the value of theBodyID
	 * @param new_var the new value of theBodyID
	 */
	void setTheBodyID ( dBodyID new_var )
	{	theBodyID = new_var; }

	/**
	 * Get the value of theBodyID
	 * @return the value of theBodyID
	 */
	dBodyID getTheBodyID ( )
	{	return theBodyID; }

	void applyPosition(void);
	
private:
	// Private attributes
	//  

	dBodyID theBodyID;
	BaseObject* theBaseObjectPtr;
	
private:
	void initAttributes ( ) ;
};

#endif // DRAWOBJECT_H
