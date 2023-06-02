//
// Created by Lilly on 26.05.2023.
//

#ifndef OPTOCUS_MANUALPAGEWIDGET_H
#define OPTOCUS_MANUALPAGEWIDGET_H

#include <QWidget>
#include <QScrollBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class ManualPageWidget : public QWidget  {
Q_OBJECT
public:
    enum class ManualType {
        PhenomPage,
        DevicesPage
    };
    explicit ManualPageWidget(ManualType type, QWidget *parent = nullptr);
private:
    ManualType page_type;
    QFile* jsonFile;
    QString getHeaders(QLayout &contentLayout);
    QJsonArray getJsonArray();
    QWidget * addImages(QJsonObject object);
    QLabel *addLabel(QJsonObject object, const QString& type, int fontSize);
    static QPixmap loadPixmapFromUrl(const QUrl &url);
};


#endif //OPTOCUS_MANUALPAGEWIDGET_H
