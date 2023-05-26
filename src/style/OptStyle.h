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
    OptStyle() {
        m_backgroundColor = QColor("#A8DADC");
        m_menuPanelColor = QColor("#1D3557");
        m_menuTextColor = QColor("#F1FAEE");
        m_buttonColor = QColor("#457B9D");
        m_textColor = QColor("#000000");
}

    void polish(QWidget *widget) override {
        QPalette pal = widget->palette();
        pal.setColor(QPalette::Window, m_backgroundColor);
        pal.setColor(QPalette::WindowText, m_textColor);
        pal.setColor(QPalette::Button, m_buttonColor);
        pal.setColor(QPalette::ButtonText, m_textColor);
        pal.setColor(QPalette::Base, m_menuPanelColor);
        pal.setColor(QPalette::Text, m_menuTextColor);
        widget->setPalette(pal);
    }
    void drawControl(ControlElement control, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const override;

private:
    static void setTexture(QPalette &palette, QPalette::ColorRole role,
                           const QImage &image);
    QColor m_backgroundColor;
    QColor m_menuPanelColor;
    QColor m_menuTextColor;
    QColor m_buttonColor;
    QColor m_textColor;
};


#endif //OPTOCUS_OPTSTYLE_H
