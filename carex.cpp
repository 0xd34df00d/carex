#include "carex.h"
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>

Carex::Carex ()
{
	Ui_.setupUi (this);
	Ui_.FullView_->setScene (&Scene_);

	auto toolbar = addToolBar (tr ("Tools"));
	toolbar->addAction (QIcon::fromTheme ("document-open"),
			tr ("Open file"), this, SLOT (openFile ()));
}

Carex::~Carex ()
{
}

void Carex::Rebuild ()
{
}

void Carex::openFile ()
{
	const auto& name = QFileDialog::getOpenFileName (this,
			tr ("Open image"),
			QDir::homePath (),
			tr ("Images (*.jpg *.png);;All files (*)"));
	if (name.isEmpty ())
		return;

	QImage image (name);
	if (image.isNull ())
	{
		QMessageBox::warning (this,
				tr ("Error"),
				tr ("Cannot open image %1")
					.arg (QFileInfo (name).fileName ()));
		return;
	}

	Image_.swap (image);

	Rebuild ();
}
