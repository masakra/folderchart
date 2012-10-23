
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
	current = new QListWidget( this );
	current->resize( 200, 50 );

	QPushButton * buttonCurAdd = new QPushButton( tr("&Add"), this ),
				* buttonCurDel = new QPushButton( tr("&Del"), this ),
				* buttonExcAdd = new QPushButton( tr("&Add"), this ),
				* buttonExcDel = new QPushButton( tr("&Del"), this );

	connect( buttonCurAdd, SIGNAL( clicked() ), SLOT( selectCurrent() ) );
	connect( buttonCurDel, SIGNAL( clicked() ), SLOT( deleteCurrent() ) );

	connect( buttonExcAdd, SIGNAL( clicked() ), SLOT( selectExclude() ) );
	connect( buttonExcDel, SIGNAL( clicked() ), SLOT( deleteExclude() ) );


	QVBoxLayout * layoutCur = new QVBoxLayout(),
				* layoutExc = new QVBoxLayout();

	layoutCur->addWidget( buttonCurAdd );
	layoutCur->addWidget( buttonCurDel );
	layoutCur->addStretch();

	layoutExc->addWidget( buttonExcAdd );
	layoutExc->addWidget( buttonExcDel );
	layoutExc->addStretch();

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
	layout->addLayout( layoutCur, 1, 1 );
	// 2 line
	layout->addWidget( labelExclude, 2, 0 );
	// 3 line
	layout->addWidget( exclude, 3, 0 );
	layout->addLayout( layoutExc, 3, 1 );
	// 4 line
	layout->addWidget( buttonBox, 4, 0, 1, 2 );

	setLayout( layout );
}

void
DialogInput::selectCurrent()
{
	selectToList( current, "blue" );
}

void
DialogInput::deleteCurrent()
{
	deleteFromList( current );
}

void
DialogInput::selectExclude()
{
	selectToList( exclude, "red" );
}

void
DialogInput::deleteExclude()
{
	deleteFromList( exclude );
}

const QListWidget &
DialogInput::cur() const
{
	return *current;
}

const QListWidget &
DialogInput::exc() const
{
	return *exclude;
}

void
DialogInput::selectToList( QListWidget * list, const QString & icon )
{
	const QString dir = QFileDialog::getExistingDirectory( this,
			tr("Select exclude directory"),
			QDir::currentPath() );

	if ( ! dir.isEmpty() ) {
		QListWidgetItem * item = new QListWidgetItem();
		item->setText( dir );
		item->setIcon( QIcon(":/" + icon + ".png" ) );

		list->addItem( item );
	}
}

void
DialogInput::deleteFromList( QListWidget * list ) const
{
	QListWidgetItem * item = list->currentItem();

	if ( item )
		delete item;
}

