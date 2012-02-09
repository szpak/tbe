/* The Butterfly Effect
 * This file copyright (C) 2009,2010  Klaas van Gend
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

#ifndef EDITLEVELPROPERTIES_H
#define EDITLEVELPROPERTIES_H

#include <QtGui/QDialog>
#include <QItemDelegate>

// forward declarations
class Level;
class Background;
class QStandardItemModel;
class QStandardItem;

namespace Ui {
    class EditLevelProperties;
}

// this header file contains two class declarations:
// first EditLevelProperties, then ColorPickerDelegate


/// this class allows editing of various Level and World settings
class EditLevelProperties : public QDialog {
    Q_OBJECT
public:
	EditLevelProperties(Level* aLevelPtr, QWidget *parent = 0);
    ~EditLevelProperties();

protected:
    void changeEvent(QEvent *e);
	void populateTableAndGradient(Background* aBGPtr);
	void writeChanges(void);

private slots:
	/// overridden to actually write the results to the Level/World
	void slot_accepted();

	/// to update the gradient based on the model
	void slot_modelItemChanged(QStandardItem* anItem);

private:
	Level* theLevelPtr;
	QStandardItemModel* theModel;
    Ui::EditLevelProperties *m_ui;
};

// --------------------------------------------------------------------------
// --------------------------------------------------------------------------


/// this is the Delegate to edit a field using a QDoubleSpinBox
class ColorPickerDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	ColorPickerDelegate(QObject *parent);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
						  const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
					  const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
							  const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // EDITLEVELPROPERTIES_H