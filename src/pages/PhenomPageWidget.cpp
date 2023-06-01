//
// Created by Lilly on 26.05.2023.
//

#include "PhenomPageWidget.h"
#include "../style/OptStyle.h"

PhenomPageWidget::PhenomPageWidget(QWidget *parent) {
    // panel for info
    QWidget *contentWidget = new QWidget(this);
    QScrollArea *scrollArea = new QScrollArea(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);

    scrollArea->setWidgetResizable(true); // idk, it doesn't work for now
    scrollArea->setWidget(contentWidget);

    // panel for navigation
    QWidget *navigationWidget = new QWidget(this);
    QScrollArea *navigationScroll = new QScrollArea(this);
    QVBoxLayout *navigationLayout = new QVBoxLayout(navigationWidget);

    // TODO: parse json here
    QString navText;
    for (int i = 1; i <= 20; ++i) {
        QLabel *sectionLabel = new QLabel(this);
        sectionLabel->setText("Розділ " + QString::number(i)); // here will be topic of optical phenomenon
        sectionLabel->setObjectName(QString::number(i));
        sectionLabel->setWordWrap(true); // it will be for main text
        sectionLabel->setMaximumWidth(this->width());
        QFont titleFont = sectionLabel->font();
        titleFont.setPointSize(24);
        sectionLabel->setFont(titleFont);
        contentLayout->addWidget(sectionLabel);
        navText += "<a href=\"#" + QString::number(i) + "\">Розділ " + QString::number(i) + "</a><br>";
    }

    // label that handle links to the topics
    QLabel *navLabel = new QLabel(this);
    QFont navFont = navLabel->font();
    navFont.setPointSize(16);
    navLabel->setFont(navFont);
    navLabel->setText(navText);
    navLabel->setTextFormat(Qt::RichText);
    navLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);


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

    navigationLayout->addWidget(navLabel);
    navigationScroll->setWidget(navigationWidget);

    // style?
    QPalette pal;
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