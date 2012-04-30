/* The Butterfly Effect
 * This file copyright (C) 2012  Klaas van Gend
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

#ifndef VIEWLINK_H
#define VIEWLINK_H

#include "ViewObject.h"

// forward declarations
class AbstractObject;

/** class ViewLink
  *
  * This class draws the DetonatorBox class, which has no physics representation,
  * and cannot be moved *BUT* can be clicked to reveal "hints"...
  */

class ViewLink : public ViewObject
{

	Q_OBJECT

public:
	// Constructors/Destructors
	//

	/// Image Constructor
        explicit ViewLink (AbstractObject* aAbstractObjectPtr, const QString& anImageName);

	/**
	 * Empty Destructor
	 */
        virtual ~ViewLink ( );


        /// todo
        void setEndpoints(const Vector& aFirstPoint,
                          const Vector& aSecondPoint);

private:
	// prevent copy constructor / assignment operator
        ViewLink(const ViewLink&);
        const ViewLink& operator= (const ViewLink&);

        QGraphicsLineItem* theLinePtr;
};

#endif // VIEWLINK_H
