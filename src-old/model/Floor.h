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

#ifndef FLOOR_H
#define FLOOR_H

#include "RectObject.h"

/**
  * class Floor
  * 
  * This class defines a floor. Essentially, a floor is static (no mass)
  * and only resizable in width.
  */

class Floor : public RectObject
{
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	Floor ( );

	/**
	 * Empty Destructor
	 */
	virtual ~Floor ( );

	// Public attribute accessor methods
	//  

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{
		return QObject::tr("Floor");
	}

	/// Floors are by definition static
	virtual b2BodyType getObjectType(void) const
	{	return b2_staticBody; }

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{
		return QObject::tr("This is a floor piece: flat and horizontal.");
	}

	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return false;	}

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return HORIZONTALRESIZE;	}
};

#endif // FLOOR_H