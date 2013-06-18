/*

    Bouncin' SP copyright jarills 2013

    This file is part of Bouncin' SP.

    Bouncin' SP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bouncin' SP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bouncin' SP.  If not, see <http://www.gnu.org/licenses/>.

*/

#pragma once

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

