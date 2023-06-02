//
// Created by Lilly on 26.05.2023.
//

#ifndef OPTOCUS_PHENOMPAGEWIDGET_H
#define OPTOCUS_PHENOMPAGEWIDGET_H

#include <QWidget>
#include <QScrollBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class PhenomPageWidget : public QWidget  {
Q_OBJECT
public:
    explicit PhenomPageWidget(QWidget *parent = nullptr);
private:
    QFile jsonFile = QFile("../resources/phenomena.json");
    QString getHeaders(QLayout &contentLayout);
    QJsonArray getJsonArray();
    QWidget * addImages(QJsonObject object);
    QLabel *addLabel(QJsonObject object, const QString& type, int fontSize);
    static QPixmap loadPixmapFromUrl(const QUrl &url);
};


#endif //OPTOCUS_PHENOMPAGEWIDGET_H
