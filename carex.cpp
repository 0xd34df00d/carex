#include "carex.h"
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <qwt_plot_curve.h>
#include <qwt_plot_legenditem.h>
#include <qwt_curve_fitter.h>
#include "profiler.h"

class ResizeEventFilter : public QObject
{
	QGraphicsItem *Item_ = nullptr;
public:
	ResizeEventFilter (QObject *parent)
	: QObject (parent)
	{
	}

	void SetItem (QGraphicsItem *item)
	{
		Item_ = item;
	}

	bool eventFilter (QObject *obj, QEvent *e)
	{
		if (Item_ && e->type () == QEvent::Resize)
			static_cast<QGraphicsView*> (obj)->fitInView (Item_);

		return QObject::eventFilter (obj, e);
	}
};

Carex::Carex ()
: ResizeFitter_ (new ResizeEventFilter (this))
, AvgCurve_ (new QwtPlotCurve (tr ("Average")))
, MinCurve_ (new QwtPlotCurve (tr ("Min")))
, MaxCurve_ (new QwtPlotCurve (tr ("Max")))
{
	Ui_.setupUi (this);
	Ui_.FullView_->setScene (&Scene_);
	Ui_.FullView_->installEventFilter (new ResizeEventFilter (Ui_.FullView_));

	Ui_.Plotter_->setAxisAutoScale (QwtPlot::yLeft, false);
	Ui_.Plotter_->setAxisScale (QwtPlot::yLeft, 0, 255);
	Ui_.Plotter_->setAxisTitle (QwtPlot::yLeft, tr ("V"));
	Ui_.Plotter_->setAxisTitle (QwtPlot::xBottom, tr ("Pixel"));

	auto setup = [this] (QwtPlotCurve *curve, const QColor& color) -> void
	{
		curve->setPen (QPen { color });
		curve->setRenderHint (QwtPlotItem::RenderAntialiased);
		curve->attach (Ui_.Plotter_);
	};

	setup (AvgCurve_, Qt::black);
	setup (MaxCurve_, Qt::red);
	setup (MinCurve_, Qt::green);

	auto item = new QwtPlotLegendItem;
	item->setMaxColumns (1);
	item->setAlignment (Qt::AlignTop | Qt::AlignLeft);
	item->attach (Ui_.Plotter_);

	auto bgColor = palette ().color (QPalette::Button);
	bgColor.setAlphaF (0.8);
	item->setBackgroundBrush (bgColor);
	item->setBorderRadius (3);
	item->setBorderPen (QPen (palette ().color (QPalette::Dark), 1));

	auto toolbar = addToolBar (tr ("Tools"));
	toolbar->addAction (QIcon::fromTheme ("document-open"),
			tr ("Open file"), this, SLOT (openFile ()));
}

namespace
{
	template<typename T>
	QVector<QPointF> MkSamples (const std::vector<double>& xpoints, const std::vector<T>& data)
	{
		QVector<QPointF> result;
		for (size_t i = 0; i < std::min (xpoints.size (), data.size ()); ++i)
			result.push_back ({ xpoints [i], static_cast<double> (data [i]) });
		return result;
	}
}

void Carex::Rebuild ()
{
	ResizeFitter_->SetItem (nullptr);
	Scene_.clear ();

	auto item = Scene_.addPixmap (QPixmap::fromImage (Image_));
	item->setTransformationMode (Qt::SmoothTransformation);
	Ui_.FullView_->fitInView (item);
	ResizeFitter_->SetItem (item);

	const auto& stats = Profiler { Image_ } ();

	std::vector<double> xpoints;
	auto firstPt = -(stats.Avg_.size () / 2.);
	std::generate_n (std::back_inserter (xpoints),
			stats.Avg_.size (), [&firstPt] { return firstPt++; });

	AvgCurve_->setSamples (MkSamples (xpoints, stats.Avg_));
	MinCurve_->setSamples (MkSamples (xpoints, stats.MinVals_));
	MaxCurve_->setSamples (MkSamples (xpoints, stats.MaxVals_));

	Ui_.Plotter_->replot ();
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
