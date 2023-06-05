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
#include <QStyleOptionSlider>

class OptStyle : public QProxyStyle {
Q_OBJECT
public:
    static constexpr QColor MINT_GREEN = QColor(205, 234, 229);
    static constexpr QColor NON_PHOTO_BLUE = QColor(168, 218, 220);
    static constexpr QColor BERKELEY_BLUE = QColor(29, 53, 87);
    static constexpr QColor HONEYDEW = QColor(241, 250, 238);
    static constexpr QColor CERULEAN_BLUE = QColor(69, 123, 157);
    static constexpr QColor PLAIN_BLACK = QColor(0, 0, 0);
    static constexpr QColor RED_PANTONE = QColor(230, 57, 70);
    static constexpr QColor SALMON_PINK = QColor(236, 154, 154);
    static constexpr QColor ORANGE_WEB = QColor(249, 166, 32);
    static constexpr QColor SILVER = QColor(188, 184, 177);
    OptStyle(): QProxyStyle("Fusion") {}
    void polish(QWidget *widget) override;
    void unpolish(QWidget *widget) override;
    void drawControl(ControlElement control, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const override;

    void drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget) const override;
};


#endif //OPTOCUS_OPTSTYLE_H
