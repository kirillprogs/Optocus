//
// Created by Lilly on 26.05.2023.
//

#include <QMenuBar>
#include "OptStyle.h"

void OptStyle::drawControl(QStyle::ControlElement control, const QStyleOption *option, QPainter *painter,
                           const QWidget *widget) const {
    if (control == CE_MenuBarEmptyArea && widget && qobject_cast<const QMenuBar*>(widget->parentWidget())) {
        painter->fillRect(option->rect, m_menuPanelColor);
    } else {
        QProxyStyle::drawControl(control, option, painter, widget);
    }
}

void OptStyle::setTexture(QPalette &palette, QPalette::ColorRole role, const QImage &image) {
    for (int i = 0; i < QPalette::NColorGroups; ++i) {
        QBrush brush(image);
        brush.setColor(palette.brush(QPalette::ColorGroup(i), role).color());
        palette.setBrush(QPalette::ColorGroup(i), role, brush);
    }
}
