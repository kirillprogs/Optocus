//
// Created by Lilly on 26.05.2023.
//

#include <QLabel>
#include <QVBoxLayout>
#include "HomePageWidget.h"

HomePageWidget::HomePageWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setAlignment(Qt::AlignCenter);

    // add logo
    QLabel* logoLabel = new QLabel(this);
    QPixmap logoPixmap("../images/logo.png");
    QSize desiredSize(250, 250);
    QPixmap scaledLogoPixmap = logoPixmap.scaled(desiredSize, Qt::KeepAspectRatio);
    logoLabel->setPixmap(scaledLogoPixmap);
    logoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(logoLabel);

    // add header
    QLabel* titleLabel = new QLabel("Optocus: Ваш супутник у світі оптики", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // authors
    QLabel* authorsLabel = new QLabel("Автори: Кирило Первушин, Лілія Паращак", this);
    authorsLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    layout->addWidget(authorsLabel);

    QLabel *instruction = new QLabel();
    instruction->setText("<style>a { color: rgb(69, 123, 157); }</style>"
            "<a href=\"https://docs.google.com/document/d/1GeObsPYyqNi_ZnEkZMar6FEwY4nj5gQyqymymfJwq3w/edit?usp=sharing\">Інструкція користувача</a>");
    instruction->setTextFormat(Qt::RichText);
    instruction->setTextInteractionFlags(Qt::TextBrowserInteraction);
    instruction->setOpenExternalLinks(true);
    instruction->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    layout->addWidget(instruction);

    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(48);
    titleLabel->setFont(titleFont);

    QFont authorsFont = authorsLabel->font();
    authorsFont.setPointSize(18);
    authorsLabel->setFont(authorsFont);
    instruction->setFont(authorsFont);

    mainLayout->addWidget(centralWidget);
}
