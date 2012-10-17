
#ifndef CHART_H
#define CHART_H

#include <QWidget>

class Chart : public QWidget
{
	private:
		struct Piece {
			qreal value;
			QString caption;
			QColor color;
		};

		QList< Piece > pieces;

	protected:
		virtual void paintEvent( QPaintEvent * e );
		virtual QSize sizeHint() const;

	public:
		Chart( QWidget * parent );

		void addPiece( qreal value, const QString & caption );

		void clear();
};

#endif

