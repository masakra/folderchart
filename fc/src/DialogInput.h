
#ifndef DIALOGINPUT_H
#define DIALOGINPUT_H

#include <QDialog>

class QListWidget;

class DialogInput : public QDialog
{
	Q_OBJECT

	private:
		void createWidgets();

		QListWidget * current,
					* exclude;

		void selectToList( QListWidget * list, const QString & icon );
		void deleteFromList( QListWidget * list ) const;

	private Q_SLOTS:
		void selectCurrent();
		void deleteCurrent();

		void selectExclude();
		void deleteExclude();

	public:
		DialogInput( QWidget * parent = 0 );

		QString path() const;

		const QListWidget & cur() const;

		const QListWidget & exc() const;
};

#endif

