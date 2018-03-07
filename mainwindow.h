#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QSharedPointer>
#include <QTimer>
#include "htmlparser.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

public slots:
    void on_StartButton_clicked();

    void on_StopButton_clicked();

    void on_Parse_timer_loop();

    void on_Parser_End(QVector<QString> List);

signals:
    void request_Parse();

public:
    Ui::MainWindow* ui;
    QSharedPointer<laskdjlaskdj12::HtmlRankParser> Parser;
    QSharedPointer<QThread> ParserThread;
    QSharedPointer<QTimer> ParseLoopTime;
    unsigned int LoopTime;
};

#endif // MAINWINDOW_H
