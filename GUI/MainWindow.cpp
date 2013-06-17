#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QStandardItemModel>
#include <QProgressDialog>

#include "Song.h"
#include "Pattern.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel(0, 1, this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    ui->patternTable->setModel(model);

    connect(ui->patternTable->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_pattern_select(QItemSelection)));

    connect( ui->patternNameEdit, SIGNAL(textEdited(QString)), this, SLOT(on_pattern_name_changed(QString)));

    ui->patternFrame->setEnabled(false);
    ui->exportButton->setEnabled(false);
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
    on_update_song();
}

void MainWindow::on_actionReload_triggered()
{
    if ( !song_ ) return;

    auto path = song_->base_folder();
    song_.reset();
    song_ = std::make_shared< Song >( path );
    on_update_song();
}

void MainWindow::on_actionOpen_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Song"),
                                                 "",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    if ( dir.isEmpty() )
    {
        return;
    }

    song_ = std::make_shared< Song >( dir.toStdString() );
    song_->load_patterns();

    if ( song_ )
    {
        ui->statusBar->showMessage( "Successfully loaded " + dir + "." );
    }

    on_update_song();
}

void MainWindow::on_pattern_name_changed(const QString &str)
{
    if ( !active_pattern_ ) return;

    active_pattern_->set_pattern_name(str.toStdString());

    update_pattern_names();
}

void MainWindow::update_pattern_names()
{
    QStandardItemModel *model = (QStandardItemModel*)ui->patternTable->model();

    if ( !song_ || !model ) return;

    auto patterns = song_->patterns();

    int row = 0;
    for ( auto p : patterns )
    {
        auto item = model->item(row,0);
        if ( !item )
        {
            item = new QStandardItem();
            model->setItem(row, 0, item);
        }

        item->setText(QString( p->pattern_name().c_str() ));
        row++;
    }
}

void MainWindow::on_update_song()
{
    QStandardItemModel *model = (QStandardItemModel*)ui->patternTable->model();

    model->removeRows(0, model->rowCount());

    ui->songInfoLabel->setText("Empty.");

    if ( !song_ ) return;

    auto patterns = song_->patterns();

    model->setRowCount(patterns.size());

    update_pattern_names();

    QString songDesc = QString((char*)song_->base_folder().c_str())
            + QString(" (")
            + QString::number(song_->patterns().size())
            + " patterns @ " + QString::number(song_->bpm() )+ " bpm)";
    ui->songInfoLabel->setText(songDesc);
}

void MainWindow::on_pattern_select(const QItemSelection &selection)
{
    if ( !song_ )
    {
        ui->patternFrame->setEnabled(false);
        return;
    }

    auto patterns = song_->patterns();
    std::shared_ptr< Pattern > ptn;

    if( !selection.indexes().isEmpty() )
    {
        unsigned i = selection.indexes().first().row();
        if ( i < patterns.size() )
        {
            ptn = patterns[i];
        }
    }

    QString pattern_name;
    int tracks = 0;

    if ( ptn )
    {
        pattern_name = ptn->pattern_name().c_str();
        tracks = ptn->pads_playing().size();
    }

    ui->patternFrame->setEnabled(!!ptn);

    ui->patternNameEdit->setText(pattern_name);
    ui->patternNameEdit->setEnabled(!!ptn);

    ui->labelNumTracks->setText(QString::number(tracks));

    ui->bpmBox->setValue(song_->bpm());

    active_pattern_ = ptn;
}

void MainWindow::on_browseFolderButton_clicked()
{
    QString dest = QFileDialog::getExistingDirectory(this, tr("Export Folder"),
                                                 ui->destinationEdit->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    if ( !dest.isEmpty() )
    {
        ui->destinationEdit->setText(dest);
    }
}

void MainWindow::on_destinationEdit_textChanged(const QString &arg1)
{
    using namespace boost::filesystem;

    const std::string path = arg1.toStdString();
    bool ok = exists(path) && is_directory(path);

    QString style = ok ? "color:green;" : "color:red;";
    ui->destinationEdit->setStyleSheet(style);
    ui->exportButton->setEnabled(ok);
}

void MainWindow::on_exportButton_clicked()
{
    if ( !song_ || !active_pattern_ ) return;

    using namespace boost::filesystem;

    Song::ExportOptions export_options;
    export_options.normalize_ = ui->normalizeCheckbox->isChecked();
    export_options.path_ = ui->destinationEdit->text().toStdString();

    QProgressDialog progress("Exporting", "Cancel", 0, 100, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    progress.setValue(10);

    song_->export_pattern(*active_pattern_, export_options);

    progress.setValue(100);
}
