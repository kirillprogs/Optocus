//
// Created by Lilly on 26.05.2023.
//

#ifndef OPTOCUS_MODELINGPAGEWIDGET_H
#define OPTOCUS_MODELINGPAGEWIDGET_H


#include <QWidget>
#include "../graphics/GraphicsPanel.h"

class ModelingPageWidget : public QWidget  {
    Q_OBJECT
public:
    explicit ModelingPageWidget(QWidget *parent = nullptr);
private:
    // GraphicsPanel *graphicsPanel;
};


#endif //OPTOCUS_MODELINGPAGEWIDGET_H
