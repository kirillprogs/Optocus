//
// Created by Lilly on 26.05.2023.
//

#ifndef OPTOCUS_OPTSTYLE_H
#define OPTOCUS_OPTSTYLE_H

#include <QPainterPath>
#include <QWidget>
#include <QStyleOption>
#include <QStyleHintReturn>
#include <QPainter>
#include <QPalette>
#include <QProxyStyle>
#include <QImage>

class OptStyle : public QProxyStyle {
Q_OBJECT
public:
    static constexpr QColor NON_PHOTO_BLUE = QColor(168, 218, 220);
    static constexpr QColor BERKELEY_BLUE = QColor(29, 53, 87);
    static constexpr QColor HONEYDEW = QColor(241, 250, 238);
    static constexpr QColor CERULEAN_BLUE = QColor(69, 123, 157);
    static constexpr QColor PLAIN_BLACK = QColor(0, 0, 0);
    static constexpr QColor RED_PANTONE = QColor(230, 57, 70);
    OptStyle() {

    }
    void polish(QWidget *widget) override;
    void drawControl(ControlElement control, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const override;

};


#endif //OPTOCUS_OPTSTYLE_H
