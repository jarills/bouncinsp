#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QStandardItemModel>

#include "Song.h"
#include "Pattern.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( ui->patternTable, SIGNAL(clicked(QModelIndex)), this, SLOT(on_pattern_select(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionClose_triggered()
{
    song_.reset();
}

void MainWindow::on_actionReload_triggered()
{
}

void MainWindow::on_actionOpen_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Song"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    song_ = std::make_shared< Song >( dir.toAscii().data() );
    song_->load_patterns();

    if ( song_ )
    {
        ui->statusBar->showMessage( "Successfully loaded " + dir + "." );
    }

    on_update_song();
}

void MainWindow::on_update_song()
{
    ui->patternTable->reset();

    if ( !song_ ) return;

    auto patterns = song_->patterns();

    QStandardItemModel *model = new QStandardItemModel(patterns.size(), 2, this);

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Length in Bars")));

    int row = 0;
    for ( auto p : patterns )
    {
        QStandardItem *name_item = new QStandardItem(QString( p->pattern_name().c_str() ));
        model->setItem(row, 0, name_item);
        QStandardItem *length_item = new QStandardItem( QString::number(p->pattern_length()) );
        model->setItem(row, 1, length_item);
        row++;
    }

    ui->patternTable->setModel(model);
}

void MainWindow::on_pattern_select(QModelIndex idx)
{
    if ( !song_ ) return;

    auto patterns = song_->patterns();

    auto ptn = patterns[idx.row()];


}
