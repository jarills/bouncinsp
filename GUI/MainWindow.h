#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

#include <memory>

class Song;

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

    void on_update_song();

    void on_pattern_select(QModelIndex idx);

private:
    Ui::MainWindow *ui;

    std::shared_ptr< Song > song_;
};

#endif // MAINWINDOW_H
