#pragma once

#include <QMainWindow>
#include "ui_carex.h"

class Carex : public QMainWindow
{
	Q_OBJECT

	Ui::Carex Ui_;

	QImage Image_;

	QGraphicsScene Scene_;
public:
	Carex ();
	virtual ~Carex ();
private:
	void Rebuild ();
private slots:
	void openFile ();
};
