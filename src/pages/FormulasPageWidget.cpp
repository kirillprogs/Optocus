//
// Created by Lilly on 26.05.2023.
//

#include "FormulasPageWidget.h"
#include "../style/OptStyle.h"

FormulasPageWidget::FormulasPageWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QJsonArray topicsArray = getFormulas();

    QTableWidget *tableWidget = new QTableWidget(this);

    int rowCount = topicsArray.size();
    for (int i = 0; i < topicsArray.size(); ++i) {
        QJsonObject topicObject = topicsArray[i].toObject();
        QJsonArray formulasArray = topicObject.value("formulas").toArray();
        rowCount += formulasArray.size();
    }
    tableWidget->setRowCount(rowCount);
    tableWidget->setColumnCount(2);

    QStringList headers;
    headers << "Формули" << "Пояснення";
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->verticalHeader()->setVisible(false);

    QFont font("Arial", 14);
    tableWidget->setFont(font);

    int rowIndex = 0;
    for (int i = 0; i < topicsArray.size(); ++i) {
        QJsonObject topicObject = topicsArray[i].toObject();
        QString topicName = topicObject.value("name").toString();
        QJsonArray formulasArray = topicObject.value("formulas").toArray();

        // Add topic name spanning two columns
        QTableWidgetItem *topicItem = new QTableWidgetItem(topicName);
        topicItem->setBackground(OptStyle::MINT_GREEN);
        topicItem->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(rowIndex, 0, topicItem);
        tableWidget->setSpan(rowIndex, 0, 1, 2);

        rowIndex++; // Increase row index after adding the topic name

        for (int j = 0; j < formulasArray.size(); ++j) {
            QJsonObject formulaObject = formulasArray[j].toObject();
            QString formula = formulaObject.value("formula").toString();
            QString explanation = formulaObject.value("explanation").toString();

            // Add formula and explanation rows
            QTableWidgetItem *formulaItem = new QTableWidgetItem(formula);
            QTableWidgetItem *explanationItem = new QTableWidgetItem(explanation);

            tableWidget->setItem(rowIndex, 0, formulaItem);
            tableWidget->setItem(rowIndex, 1, explanationItem);

            formulaItem->setTextAlignment(Qt::AlignCenter);
            formulaItem->setFlags(formulaItem->flags() ^ Qt::ItemIsEditable);
            explanationItem->setFlags(explanationItem->flags() ^ Qt::ItemIsEditable);
            formulaItem->setBackground(OptStyle::HONEYDEW);
            explanationItem->setBackground(OptStyle::HONEYDEW);

            rowIndex++;
        }
    }

    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->resizeColumnsToContents();
    tableWidget->resizeRowsToContents();
    tableWidget->verticalHeader()->setDefaultSectionSize(50);

    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    layout->addWidget(tableWidget);
    setLayout(layout);
}

QJsonArray FormulasPageWidget::getFormulas() {
    QFile *jsonFile = new QFile("../resources/formulas.json");
    if (!jsonFile->open(QIODevice::ReadOnly)) {
        qDebug() << "Не вдалося відкрити файл " << jsonFile->fileName();
        return {};
    }
    QByteArray jsonData = jsonFile->readAll();
    jsonFile->close();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    return jsonDoc.array();
}
