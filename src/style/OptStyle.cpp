//
// Created by Lilly on 26.05.2023.
//

#include <QMenuBar>
#include "OptStyle.h"

void OptStyle::drawControl(QStyle::ControlElement control, const QStyleOption *option, QPainter *painter,
                           const QWidget *widget) const {
    if (control == CE_MenuBarEmptyArea && widget && qobject_cast<const QMenuBar*>(widget->parentWidget())) {
        painter->fillRect(option->rect, BERKELEY_BLUE);
    }
    else if (control == QStyle::CE_PushButton && qstyleoption_cast<const QStyleOptionButton *>(option)) {
            QPalette pal = option->palette;

            if (option->state & (QStyle::State_MouseOver | QStyle::State_Sunken)) {
                pal.setColor(QPalette::Button, BERKELEY_BLUE.lighter(50));
            }else {
                pal.setColor(QPalette::Button, CERULEAN_BLUE);
            }

            pal.setColor(QPalette::ButtonText, HONEYDEW);
            painter->fillRect(option->rect, pal.button());

            painter->setPen(pal.buttonText().color());
            painter->drawText(option->rect, Qt::AlignCenter, qstyleoption_cast<const QStyleOptionButton *>(option)->text);
    }
    else {
        QProxyStyle::drawControl(control, option, painter, widget);
    }
}

void OptStyle::polish(QWidget *widget){
    QPalette pal = widget->palette();
    pal.setColor(QPalette::Window, NON_PHOTO_BLUE);
    pal.setColor(QPalette::WindowText, PLAIN_BLACK);
    pal.setColor(QPalette::Button, CERULEAN_BLUE);
    pal.setColor(QPalette::ButtonText, HONEYDEW);
    pal.setColor(QPalette::Base, BERKELEY_BLUE);
    pal.setColor(QPalette::Text, HONEYDEW);
    widget->setPalette(pal);
}
