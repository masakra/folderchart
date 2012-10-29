
#include "Chart.h"

#include <QtGui>

#include "nightcharts.h"

Chart::Chart( QWidget * parent )
	: QWidget( parent )
{
}

void
Chart::paintEvent( QPaintEvent * )
{
	QPainter painter;
	painter.begin( this );

	Nightcharts PieChart;

	PieChart.setType( Nightcharts::Histogramm );		//{Histogramm,Pie,DPie};
	PieChart.setLegendType( Nightcharts::Vertical );	//{Round,Vertical}
	PieChart.setCords( 10, 15, width() - 120, height() - 25 );

	for ( int i = 0; i < pieces.size(); ++i )
		PieChart.addPiece( pieces[ i ].caption, pieces[ i ].color, pieces[ i ].value );

	PieChart.draw( &painter );

	PieChart.setLegendCords( width() - 100, 10 );

	PieChart.drawLegend( &painter );
}

void
Chart::clear()
{
	pieces.clear();

	update();
}

void
Chart::addPiece( qreal value, const QString & caption )
{
	Piece piece;

	piece.value =  qRound( value * 10 ) / 10.;
	piece.caption = caption;
	piece.color = QColor( qrand() % 200 + 55, qrand() % 200 + 55, qrand() % 200 + 55 );

	pieces << piece;
}

QSize
Chart::sizeHint() const
{
	return QSize( 300, 300 );
}

