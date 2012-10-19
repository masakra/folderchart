
#include "DialogInput.h"

#include <QtGui>

DialogInput::DialogInput( QWidget * parent )
	: QDialog( parent )
{
	setWindowTitle( tr("Folder chart") );

	createWidgets();
}

void
DialogInput::createWidgets()
{
	current = new QLineEdit( this );

	QPushButton * buttonSelectCurrent = new QPushButton( tr("..."), this ),
				* buttonExcAdd = new QPushButton( tr("&Add"), this ),
				* buttonExcDel = new QPushButton( tr("&Del"), this );

	connect( buttonSelectCurrent, SIGNAL( clicked() ), SLOT( selectCurrent() ) );

	connect( buttonExcAdd, SIGNAL( clicked() ), SLOT( selectExclude() ) );
	connect( buttonExcDel, SIGNAL( clicked() ), SLOT( deleteExclude() ) );


	QHBoxLayout * layoutExc = new QHBoxLayout();
	layoutExc->addStretch();
	layoutExc->addWidget( buttonExcAdd );
	layoutExc->addWidget( buttonExcDel );

	exclude = new QListWidget( this );

	QGridLayout * layout = new QGridLayout();

	QLabel * labelCurrent = new QLabel( tr("&path:"), this ),
		   * labelExclude = new QLabel( tr("e&xclude pathes:"), this );

	labelCurrent->setBuddy( current );
	labelExclude->setBuddy( exclude );

	QDialogButtonBox * buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok |
			QDialogButtonBox::Cancel );

	connect( buttonBox, SIGNAL( accepted() ), SLOT( accept() ) );
	connect( buttonBox, SIGNAL( rejected() ), SLOT( reject() ) );

	// 0 line
	layout->addWidget( labelCurrent, 0, 0 );
	// 1 line
	layout->addWidget( current, 1, 0 );
	layout->addWidget( buttonSelectCurrent, 1, 1 );
	// 2 line
	layout->addWidget( labelExclude, 2, 0 );
	// 3 line
	layout->addWidget( exclude, 3, 0 );
	// 4 line
	layout->addLayout( layoutExc, 4, 0 );
	// 5 line
	layout->addWidget( buttonBox, 5, 0, 1, 2 );


	setLayout( layout );
}

void
DialogInput::selectCurrent()
{
	QString dir = QFileDialog::getExistingDirectory( this,
			tr("Select current directory"), QDir::currentPath() );

	if ( ! dir.isEmpty() )
		current->setText( dir );
}

void
DialogInput::selectExclude()
{
	QString dir = QFileDialog::getExistingDirectory( this,
			tr("Select exclude directory"),
			current->text().isEmpty() ? QDir::currentPath() : current->text() );

	if ( ! dir.isEmpty() )
		exclude->addItem( dir );
/*
	QStringList dirs = QFileDialog::getOpenFileNames( this, tr("Select exclude directory"),
			QDir::currentPath(), QString(), 0, QFileDialog::ShowDirsOnly );
*/
}

void
DialogInput::deleteExclude()
{
	QListWidgetItem * item = exclude->currentItem();

	if ( item )
		delete item;
}

QString
DialogInput::path() const
{
	return current->text();
}

const QListWidget &
DialogInput::exc() const
{
	return *exclude;
}
