#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlQuery>
#include <QString>
#include <QVector>
#include <QDebug>
namespace laskdjlaskdj12
{
class HtmlRankParser : public QObject
{
    Q_OBJECT
public:
    HtmlRankParser(QString HtmlLink)
    {
        WebSiteName = HtmlLink;
    }
    ~HtmlRankParser()
    {

    }

protected:
    QVector<QString> ParseRank(QByteArray Source, QString WebSiteName)
    {
        QVector<QString> RetRank;
        if (WebSiteName == "https://www.daum.net")
        {
            QByteArray DaumHtml = Source;

            for (int i = 1; i <= 10; i++)
            {
                int RankElementIndex = DaumHtml.indexOf("<div class=\"roll_txt @" + QString::number(i) + "\">");

                //Todo ">" 8개 를 지난 엘리멘트에 들어있는  검색어 순위 목록을 추출함
                unsigned int RightCrossCount = 0;
                while (RightCrossCount < 8)
                {
                    if (DaumHtml.at(RankElementIndex) == '>')
                    {
                        RightCrossCount += 1;
                    }

                    if (RightCrossCount < 8)
                    {
                        RankElementIndex++;
                    }
                }

                unsigned int StartPointIndex = RankElementIndex;
                //"</a>의 엘리멘트까지 의 검색어를 찾아서 추출함
                bool IsLeftCrossEnd = false;

                while (!IsLeftCrossEnd)
                {
                    if (DaumHtml.at(RankElementIndex) == '<')
                    {
                        IsLeftCrossEnd = true;
                        continue;
                    }
                    RankElementIndex++;
                }

                unsigned int EndPointIndex = RankElementIndex;

                unsigned int RankVocSize = EndPointIndex - StartPointIndex;
                QByteArray RankVocabliy = DaumHtml.mid(StartPointIndex + 1, RankVocSize - 1);

                QString RankVocabliyString = QString::fromUtf8(RankVocabliy);

                RetRank.append(RankVocabliyString);
                qDebug() << RankVocabliyString;
            }

        }

        else
        {
            qDebug() << "NotSupport";
        }

        return RetRank;
    }

public slots:
    void Start()
    {
        HttpConnect = QSharedPointer<QNetworkAccessManager>::create();
        connect(HttpConnect.data(), SIGNAL(finished(QNetworkReply*)), this, SLOT(on_NetworkReply(QNetworkReply*)));
    }

    void on_RequestParse()
    {
        QNetworkRequest HttpRequest;
        HttpRequest.setUrl(WebSiteName);

        HttpConnect->get(HttpRequest);
    }

    void on_NetworkReply(QNetworkReply* reply)
    {
        QVector<QString> RankList;
        if (reply->error() != QNetworkReply::NoError)
        {
            emit ParseClear(RankList);
            return;
        }

        RankList = ParseRank(reply->readAll(), WebSiteName);

        emit ParseClear(RankList);
    }

signals:
    void ParseClear(QVector<QString> Rank);

private:
    QString WebSiteName;
    QSharedPointer<QNetworkAccessManager> HttpConnect;
};
}
#endif // HTMLPARSER_H
