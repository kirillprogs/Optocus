//
// Created by Lilly on 26.05.2023.
//

#include <QLabel>
#include <QVBoxLayout>
#include "DevicesPageWidget.h"

DevicesPageWidget::DevicesPageWidget(QWidget *parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setAlignment(Qt::AlignCenter);

    QLabel* titleLabel = new QLabel("OPTICAL DEVICES PAGE", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(48);
    titleLabel->setFont(titleFont);

    mainLayout->addWidget(centralWidget);
}
