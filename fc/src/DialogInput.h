
#ifndef DIALOGINPUT_H
#define DIALOGINPUT_H

#include <QDialog>

class QLineEdit;
class QListWidget;

class DialogInput : public QDialog
{
	Q_OBJECT

	private:
		void createWidgets();

		QLineEdit * current;

		QListWidget * exclude;

	private Q_SLOTS:
		void selectCurrent();

		void selectExclude();
		void deleteExclude();

	public:
		DialogInput( QWidget * parent = 0 );

		QString path() const;

		const QListWidget & exc() const;
};

#endif

