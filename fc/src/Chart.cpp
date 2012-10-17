
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
	QFont font;
	painter.begin(this);
	Nightcharts PieChart;

	PieChart.setType( Nightcharts::Histogramm );//{Histogramm,Pie,DPie};
	PieChart.setLegendType( Nightcharts::Vertical );//{Round,Vertical}
	PieChart.setCords( 20, 20, this->width() - 40, this->height() - 40 );

	for ( int i = 0; i < pieces.size(); ++i ) {
		PieChart.addPiece( pieces[ i ].caption, pieces[ i ].color, pieces[ i ].value );
	}
	/*
	PieChart.addPiece( "Item1", QColor( 200, 10, 50 ), 34 );
	PieChart.addPiece( "Item2", Qt::green, 27 );
	PieChart.addPiece( "Item3", Qt::cyan, 14 );
	PieChart.addPiece( "Item4", Qt::yellow, 7 );
	PieChart.addPiece( "Item5", Qt::blue, 4 );
	*/
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

