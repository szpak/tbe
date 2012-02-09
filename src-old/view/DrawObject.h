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

#include <QGraphicsItem>
#include <QCursor>

// forward declarations
class BaseObject;
class QUndoStack;
class QUndoCommand;
class UndoObjectChange;
class Anchors;
class DrawWorld;
class ImageRenderer;

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

	/// Simple Constructor
	DrawObject (BaseObject* aBaseObjectPtr);

	/// Image Name Constructor
	DrawObject (BaseObject* aBaseObjectPtr, const QString& anImageName);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawObject ( );

	/// overriden from QGraphicsItem
    virtual QRectF boundingRect() const;

	/** @returns the aspect ratio of this draw object
	  * before any scaling. It always returns (width/height)
	  * This is based on the Document size (SVG) or pixmap size.
	  */
	virtual float getUnscaledAspectRatio(void) const;

	/** Generate a bitmap (QPixmap) of size aWidth*aHeight
	  * and draw the object within the bitmap.
	  * Make sure its aspect ratio keeps correct.
	  * if aWidth or aHeight is zero, the bitmap will be scaled to fit the
	  * current view (i.e. the resizinggraphicsview).
	  * @param aWidth   width of the bitmap in screen pixels or 0
	  * @param aHeight  height of the bitmap in screen pixels or 0
	  * @returns QPixmap*, caller is responsible for destruction.
	  *
	  * TODO/FIXME: Future improvement: use QPixmapCache classes
	  */
	QPixmap* createBitmap(int aWidth=0, int aHeight=0);


	/// overriden from QGraphicsItem
    virtual void advance(int step);

	/// covenience version without event of focusInEvent
	virtual void setFocusToMe (void);

	BaseObject* getBaseObjectPtr(void) const
		{ return theBaseObjectPtr; }

	/// overriden from QGraphicsItem
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);

	bool pushUndo(QUndoCommand* anUndo);

	/** static function, called by DrawWorld to indicate whether
	  * the simulation is running or not.
	  * Currently, the only customer of this info is paint(),
	  * which has to decide if it draws all objects or not.
	  * @param aBool  true to signal that sim is running.
	  */
	static void setIsSimRunning(bool aBool);

	static void setUndoStackPtr(QUndoStack* aPtr);

	/// setup caching for this object
	void setupCache(void);

	/** Set the Z-ordering (which object is drawn on top of which).
	  * the highest number is drawn on top.
	  *  - default for objects is 2
	  *  - default for Scenery is 0.1
	  * the ZValue is actually set in setupCache()
	  * @param aZValue the Z Value to set drawing to
	  * OVERRIDDEN FROM QGraphicsItem
	  */
	virtual void setZValue(float aZValue)
	{ theZValue = aZValue; }

protected:
	float theZValue;

	/** collision detection, sets isCollidingDuringDrag if true
	  * @returns true if collision detected
	  */
	bool checkForCollision(void);

	static QUndoStack* getUndoStackPtr(void);

	/** overridden from QGraphicsItem
	 *  we want to know when the user selects the object
	 *    - so we can have theEditState take action
	 *  @param event the even to handle
	 */
	virtual void focusInEvent ( QFocusEvent * event );

	/** overridden from QGraphicsItem
	 *  if the user drags the object around, this even will be called for each pixel.
	 *  let's actually adjust the coordinates!!!
	 *  
	 *  @param event the even to handle
	 */
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
	virtual void myMouseMoveEvent ( const QPointF& aPos, const QPointF& bPos = QPointF(0,0) );

	/** overridden from QGraphicsItem
	 *  we want to know when the user *releases* the left mouse button 
	 *    - so we can take action if it is a drag 
	 *  
	 *  @param event the even to handle
	 */
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

protected:
	// Protected members
	//

	virtual void applyPosition(void);

	/// removes the collision cross - if there is one
	void removeCollisionCross(void);
	
protected:
	// Protected attributes
	//  

	BaseObject* theBaseObjectPtr;

	static const qreal theScale;
	
	// current angle of the object in radians!
	qreal	theOldAngle;
	
	/** Pointer to an Achors class
	 *  This pointer is shared across all DrawObjects,
	 *  as such you shouldn't assume it's yours
	 */
	static Anchors* theAnchorsPtr;

private:
	/// list of pointers to ImageRenderers
	QList<ImageRenderer*>   theImageRenderers;

	/// scaled/rendered pixmap cached for faster display
	/// created by setupCache()
	QList<QPixmap*>        theCachePixmapPtrs;

	/// true if DrawObject is in Caching mode
	bool isCaching;
	/// will be set to a value while we're caching - see setupCache()
	int  theCachingIndex;

	/// keeps the last used image index - to ensure redraw on static objects
	/// see also the advance() member code
	int theLastImageIndex;

protected:
	/// @returns the renderer for the current image frame
	ImageRenderer* getRenderer(void) const;

	/// pointer for undeleting this object
	///   - only usable *after* a deregister() !!!
	DrawWorld* theUndeleteDrawWorldPtr;

	/// pointer to QUndoCommand for move
	UndoObjectChange* theUndoMovePtr;
	QPointF theMoveHotspot;

private:
	virtual void initAttributes ( ) ;

	// all below friends are needed for Undoing actions
	friend class UndoDeleteCommand;    // delete
	friend class UndoInsertCommand;
	friend class UndoObjectChange;		// the new Undo command


	/** this class-in-class displays the Cross over the
	  * item in case of a collision
	  */
	class Cross : public QGraphicsItem
	{
	public:
		Cross(DrawObject* aParent);
		virtual ~Cross();

                /// overridden to scale to parent
                virtual QRectF boundingRect() const;

		/// overriden from QGraphicsItem
		virtual void paint(QPainter *painter,
						   const QStyleOptionGraphicsItem *option,
						   QWidget *widget);
	private:
		/// we only need one Cross Renderer for all DrawObjects...
		static ImageRenderer*	theCrossRendererPtr;

		BaseObject* theBaseObjectPtr;

		// kill possibility for copy constructor&assignment operator
		Cross(const DrawObject::Cross&);
		const DrawObject::Cross& operator=(const DrawObject::Cross&);
	};

	Cross* theCrossPtr;
	friend class Cross;

private:
	// kill the default constructor, copy constructor & assignment operator
	DrawObject();
	DrawObject(const DrawObject&);
	const DrawObject& operator=(const DrawObject&);
};

#endif // DRAWOBJECT_H