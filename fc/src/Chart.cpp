
#include "Chart.h"

#include <QtGui>

#include "nightcharts.h"

Chart::Chart( QWidget * parent )
	: QWidget( parent )
{
}

void
Chart::paintEvent( QPaintEvent * e )
{
	QPainter painter;
	//QFont font;
	painter.begin( this );

	Nightcharts PieChart;

	PieChart.setType( Nightcharts::Histogramm );		//{Histogramm,Pie,DPie};
	PieChart.setLegendType( Nightcharts::Vertical );	//{Round,Vertical}
	PieChart.setCords( 20, 20, width() - 40, height() - 40 );

	for ( int i = 0; i < pieces.size(); ++i )
		PieChart.addPiece( pieces[ i ].caption, pieces[ i ].color, pieces[ i ].value );

	PieChart.draw( &painter );

	PieChart.setLegendCords( 30, 20 );

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

