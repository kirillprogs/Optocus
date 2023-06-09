//
// Created by Lilly on 26.05.2023.
//

#ifndef OPTOCUS_FORMULASPAGEWIDGET_H
#define OPTOCUS_FORMULASPAGEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QFile>

class FormulasPageWidget : public QWidget {
Q_OBJECT
public:
    explicit FormulasPageWidget(QWidget *parent = nullptr);
private:
    static QJsonArray getFormulas();
};



#endif //OPTOCUS_FORMULASPAGEWIDGET_H
