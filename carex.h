#pragma once

#include <QMainWindow>
#include "ui_carex.h"

class QwtPlotCurve;

class ResizeEventFilter;

class Carex : public QMainWindow
{
	Q_OBJECT

	Ui::Carex Ui_;

	QImage Image_;

	QGraphicsScene Scene_;

	ResizeEventFilter * const ResizeFitter_;

	QwtPlotCurve * const AvgCurve_;
	QwtPlotCurve * const MinCurve_;
	QwtPlotCurve * const MaxCurve_;
public:
	Carex ();
private:
	void Rebuild ();
private slots:
	void openFile ();
};
