#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QObject>
#include <QString>
#include <QThread>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_StartButton_clicked()
{
    ui->LogEditor->append("파싱 시작");

    //Parser 초기화
    Parser = QSharedPointer<laskdjlaskdj12::HtmlRankParser>::create("https://www.daum.net");

    ParserThread = QSharedPointer<QThread>::create();

    //파싱 쓰레드 초기화
    Parser->moveToThread(ParserThread.data());
    connect(ParserThread.data(), SIGNAL(started()), Parser.data(), SLOT(Start()));
    connect(this, SIGNAL(request_Parse()), Parser.data(), SLOT(on_RequestParse()));
    connect(Parser.data(), SIGNAL(ParseClear(QVector<QString>)), this, SLOT(on_Parser_End(QVector<QString>)));
    ParserThread->start();

    //타이머 초기화
    unsigned int LoopTime = ui->LoopTimeEditor->text().toInt();
    ParseLoopTime = QSharedPointer<QTimer>::create();
    connect(ParseLoopTime.data(), SIGNAL(timeout()), this, SLOT(on_Parse_timer_loop()));
    ParseLoopTime->start(LoopTime);
}

void MainWindow::on_StopButton_clicked()
{
    if (ParseLoopTime.isNull())
    {
        ui->LogEditor->setPlainText("시작이 되지 않았습니다.");
        return;
    }

    ParseLoopTime->stop();
    disconnect(ParseLoopTime.data(), SIGNAL(timeout()), this, SLOT(on_Parse_timer_loop()));
}

void MainWindow::on_Parse_timer_loop()
{
    ui->LogEditor->append("추출중..");
    emit request_Parse();
}

void MainWindow::on_Parser_End(QVector<QString> List)
{
    ui->LogEditor->append("추출완료");

    for (int i = 0; i < List.count(); i++)
    {
        QTableWidgetItem* RankListItem = new QTableWidgetItem;
        RankListItem->setText(List.at(i));
        ui->PotalSearchList->setItem(i, 0, RankListItem);
    }
}
