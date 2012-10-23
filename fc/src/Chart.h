
/** \class Chart
 *
 * \brief Widget with histogramm chart
 */

#ifndef CHART_H
#define CHART_H

#include <QWidget>

class Chart : public QWidget
{
	private:
		/** \struct Piece
		 *
		 * \brief Element of histogramm
		 */
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

		/** \fn void addPiece( qreal value, const QString & caption )
		 *
		 * \brief Add an alement of histogramm
		 */
		void addPiece( qreal value, const QString & caption );

		/** \fn void clear()
		 *
		 * \brief Removes all pieces
		 */
		void clear();
};

#endif

