//
// Created by Lilly on 26.05.2023.
//

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QEventLoop>
#include "PhenomPageWidget.h"
#include "../style/OptStyle.h"

PhenomPageWidget::PhenomPageWidget(QWidget *parent) : QWidget(parent)  {
    // panel for info
    QWidget *contentWidget = new QWidget(this);
    QScrollArea *scrollArea = new QScrollArea(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);

    // panel for navigation
    QWidget *navigationWidget = new QWidget(this);
    QScrollArea *navigationScroll = new QScrollArea(this);
    QVBoxLayout *navigationLayout = new QVBoxLayout(navigationWidget);

    QString navText = getHeaders(*contentLayout);

    // label that handle links to the topics
    QLabel *navLabel = new QLabel(this);
    QFont navFont = navLabel->font();
    navFont.setPointSize(16);
    navLabel->setFont(navFont);
    navLabel->setText(navText);
    navLabel->setTextFormat(Qt::RichText);
    navLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    navigationLayout->addWidget(navLabel);


    connect(navLabel, &QLabel::linkActivated, this, [scrollArea](const QString& link) {
        // we got string "#{num}" and should get only num without #
        int sectionId = link.mid(1).toInt();
        // we've done labels for topics to get them like children of scrollArea
        QWidget* sectionWidget = scrollArea->widget()->findChild<QWidget*>(QString::number(sectionId));
        if (sectionWidget) {
            QScrollBar *mainScrollBar = scrollArea->verticalScrollBar();
            int sectionY = sectionWidget->geometry().y();
            mainScrollBar->setValue(sectionY);
        }
        else {
            qDebug() << "Something went wrong...";
        }
    });


    scrollArea->setWidget(contentWidget);
    navigationScroll->setWidget(navigationWidget);

    // style?
    QPalette pal;
    navigationScroll->setWidgetResizable(true);
    scrollArea->setWidgetResizable(true);
    pal.setColor(QPalette::Window, OptStyle::HONEYDEW);
    contentWidget->setAutoFillBackground(true);
    contentWidget->setPalette(pal);
    navigationWidget->setAutoFillBackground(true);
    navigationWidget->setPalette(pal);


    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(scrollArea, 6);
    mainLayout->addWidget(navigationScroll, 1);
    setLayout(mainLayout);
}

QString PhenomPageWidget::getHeaders(QLayout& contentLayout) {
    QString navText;

    QJsonArray phenomenaArray = getJsonArray();
    int counter = 0;
    for (const QJsonValueRef& value : phenomenaArray) {
        QJsonObject phenomenonObj = value.toObject();

        QLabel *headerLabel = addLabel(phenomenonObj, "name", 24);
        headerLabel->setObjectName(QString::number(counter));

        contentLayout.addWidget(headerLabel);
        contentLayout.addWidget(addLabel(phenomenonObj, "description", 14));
        contentLayout.addWidget(addImages(phenomenonObj));

        navText += "<a href=\"#" + QString::number(counter++) + "\">" + phenomenonObj["name"].toString() + "</a><br>";
    }
    return navText;
}

QJsonArray PhenomPageWidget::getJsonArray() {

    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Не вдалося відкрити файл " << jsonFile.fileName();
        return {};
    }
    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    return jsonDoc.array();
}

QWidget * PhenomPageWidget::addImages(QJsonObject object) {
    QString imagePath = object["image"].toString();

    QWidget *imageWidget = new QWidget(this);
    QLabel* imgLabel = new QLabel(this);
    QHBoxLayout *imageLayout = new QHBoxLayout();
    QUrl imageUrl(imagePath);
    QPixmap image = loadPixmapFromUrl(imageUrl);

    if (!image.isNull()) {
        imgLabel->setFixedWidth(this->width());
        imgLabel->setFixedHeight(this->height()/2);
        imgLabel->setPixmap(image.scaled(imgLabel->width(),imgLabel->height(),Qt::KeepAspectRatio));
        imgLabel->setAlignment(Qt::AlignCenter);
    }
    imageLayout->addWidget(imgLabel);
    imageWidget->setLayout(imageLayout);
    return imageWidget;
}

QPixmap PhenomPageWidget::loadPixmapFromUrl(const QUrl& url) {
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(QNetworkRequest(url));

    QEventLoop eventLoop;
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    QPixmap pixmap;
    pixmap.loadFromData(reply->readAll());
    return pixmap;
}

QLabel * PhenomPageWidget::addLabel(QJsonObject object, const QString& type, int fontSize) {
    QString string = object[type].toString();

    QLabel *label = new QLabel(this);
    label->setText(string);
    label->setWordWrap(true);
    label->setMinimumWidth(this->width());

    QFont font = label->font();
    font.setPointSize(fontSize);
    label->setFont(font);
    return label;
}