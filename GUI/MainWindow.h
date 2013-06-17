#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelection>

#include <memory>

class Song;
class Pattern;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionExit_triggered();

    void on_actionClose_triggered();

    void on_actionReload_triggered();

    void on_actionOpen_triggered();

    void on_pattern_name_changed(const QString & str);

    void on_pattern_select(const QItemSelection & selection);

    void on_browseFolderButton_clicked();

    void on_destinationEdit_textChanged(const QString &arg1);

    void on_exportButton_clicked();

private:

    void on_update_song();
    void update_pattern_names();
    void buildPatternView();

private:
    Ui::MainWindow *ui;

    std::shared_ptr< Song > song_;
    std::shared_ptr< Pattern > active_pattern_;
};

#endif // MAINWINDOW_H
