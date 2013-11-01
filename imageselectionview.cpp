#include "imageselectionview.h"
#include <QMouseEvent>

ImageSelectionView::ImageSelectionView (QWidget *parent)
: QGraphicsView (parent)
{
}

void ImageSelectionView::mousePressEvent (QMouseEvent *event)
{
	PressPoint_ = event->pos ();
	QGraphicsView::mousePressEvent (event);
}

void ImageSelectionView::mouseMoveEvent (QMouseEvent *event)
{
	if (event->buttons () != Qt::NoButton)
		EmitOnRelease_ = true;

	QGraphicsView::mouseMoveEvent (event);
}

void ImageSelectionView::mouseReleaseEvent (QMouseEvent *event)
{
	QGraphicsView::mouseReleaseEvent (event);

	if (PressPoint_ == event->pos () || EmitOnRelease_)
	{
		if (PressPoint_ == event->pos ())
			scene ()->setSelectionArea ({});
		emit selectionChanged ();
	}
}
