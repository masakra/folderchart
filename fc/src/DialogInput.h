
/** \class DialogInput
 *
 * \brief Dialog for input pathes
 *
 * Dialog prepares data for scanning selected folders with
 * excluding some folders.
 */
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

		/** \fn void selectToList( QListWidget * list, const QString & icon )
		 *
		 * \brief Append new item to list \a list and icon
		 *
		 * Icon defines by name \a icon from resource file without extention.
		 * Currently "blue" or "red".
		 */
		void selectToList( QListWidget * list, const QString & caption, const QString & icon );

		/** \fn void deleteFromList( QListWidget * list ) const
		 *
		 * \brief Removes an entry from list \a list
		 */
		void deleteFromList( QListWidget * list ) const;

	private Q_SLOTS:
		/** \fn void selectCurrent()
		 *
		 * \brief Add new path to select list
		 */
		void selectCurrent();

		/** \fn void deleteCurrent()
		 *
		 * \brief Removes an item from select list
		 */
		void deleteCurrent();

		/** \fn void selectExclude()
		 *
		 * \brief Add new path to select list
		 */
		void selectExclude();

		/** \fn void deleteExclude()
		 *
		 * \brief Removes an item from exclude list
		 */
		void deleteExclude();

	public:
		DialogInput( QWidget * parent = 0 );

		const QListWidget & cur() const;

		const QListWidget & exc() const;
};

#endif

