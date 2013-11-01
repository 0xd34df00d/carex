#pragma once

#include <QGraphicsView>

class ImageSelectionView : public QGraphicsView
{
	Q_OBJECT

	bool EmitOnRelease_ = false;
	QPoint PressPoint_;
public:
	ImageSelectionView (QWidget* = 0);
protected:
	void mousePressEvent (QMouseEvent*);
	void mouseMoveEvent (QMouseEvent*);
	void mouseReleaseEvent (QMouseEvent*);
signals:
	void selectionChanged ();
};
