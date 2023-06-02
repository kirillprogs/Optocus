//
// Created by Lilly on 26.05.2023.
//

#include <QMenuBar>
#include <QPushButton>
#include "OptStyle.h"

void OptStyle::drawControl(QStyle::ControlElement control, const QStyleOption *option, QPainter *painter,
                           const QWidget *widget) const {
    switch (control) {
        case CE_MenuBarEmptyArea:
            painter->fillRect(option->rect, BERKELEY_BLUE);
            break;
        case CE_PushButton: {
            QPalette pal = option->palette;

            if (option->state & (QStyle::State_MouseOver | QStyle::State_Sunken)) {
                pal.setColor(QPalette::Button, BERKELEY_BLUE.lighter(50));
            } else {
                pal.setColor(QPalette::Button, CERULEAN_BLUE);
            }

            pal.setColor(QPalette::ButtonText, HONEYDEW);
            painter->fillRect(option->rect, pal.button());

            painter->setPen(pal.buttonText().color());
            painter->drawText(option->rect, Qt::AlignCenter,
                              qstyleoption_cast<const QStyleOptionButton *>(option)->text);
        }
            break;
        case CE_ScrollBarSlider:

            break;
        default:
            QProxyStyle::drawControl(control, option, painter, widget);
    }
}

void OptStyle::polish(QWidget *widget){
    if (qobject_cast<QPushButton *>(widget))
        widget->setAttribute(Qt::WA_Hover, true);
    QPalette pal = widget->palette();
    pal.setColor(QPalette::Window, NON_PHOTO_BLUE);
    pal.setColor(QPalette::WindowText, PLAIN_BLACK);
    pal.setColor(QPalette::Button, CERULEAN_BLUE);
    pal.setColor(QPalette::ButtonText, HONEYDEW);
    pal.setColor(QPalette::Base, MINT_GREEN);
    pal.setColor(QPalette::Text, BERKELEY_BLUE);
    widget->setPalette(pal);
}

void OptStyle::unpolish(QWidget *widget) {
    if (qobject_cast<QPushButton *>(widget))
        widget->setAttribute(Qt::WA_Hover, false);
}

int OptStyle::pixelMetric(PixelMetric metric,
                                    const QStyleOption *option,
                                    const QWidget *widget) const{
    switch (metric) {
        case PM_ComboBoxFrameWidth:
            return 8;
        case PM_ScrollBarExtent:
            return QProxyStyle::pixelMetric(metric, option, widget) + 4;
        default:
            return QProxyStyle::pixelMetric(metric, option, widget);
    }
}

int OptStyle::styleHint(StyleHint hint, const QStyleOption *option,
                                  const QWidget *widget,
                                  QStyleHintReturn *returnData) const {
    switch (hint) {
        case SH_DitherDisabledText:
            return int(false);
        case SH_EtchDisabledText:
            return int(true);
        default:
            return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
}

void OptStyle::drawComplexControl(OptStyle::ComplexControl control, const QStyleOptionComplex* option,
                        QPainter* painter, const QWidget* widget = nullptr) const {
    if (control == QStyle::CC_ScrollBar) {
        const auto* sliderOption = qstyleoption_cast<const QStyleOptionSlider*>(option);
        if (sliderOption) {
            if (sliderOption->subControls & QStyle::SC_ScrollBarGroove) {
                painter->setPen(Qt::NoPen);
                painter->setBrush(MINT_GREEN);
                painter->drawRect(sliderOption->rect.adjusted(0, 1, -1, -1));
            }
            if (sliderOption->subControls & QStyle::SC_ScrollBarSlider) {
                QRect handleRect = subControlRect(QStyle::CC_ScrollBar, sliderOption, QStyle::SC_ScrollBarSlider, widget);
                painter->setPen(Qt::NoPen);
                painter->setBrush(SALMON_PINK);
                painter->drawRect(handleRect.adjusted(0, 1, -1, -1));
            }
            if (sliderOption->subControls & QStyle::SC_ScrollBarSubLine) {
                QRect subLineRect = subControlRect(QStyle::CC_ScrollBar, sliderOption, QStyle::SC_ScrollBarSubLine, widget);
                painter->setPen(Qt::NoPen);
                painter->setBrush(NON_PHOTO_BLUE);
                painter->drawRect(subLineRect);
                // arrow up
                painter->setPen(BERKELEY_BLUE);
                painter->setBrush(BERKELEY_BLUE);
                painter->drawPolygon(QPolygonF() << QPointF(subLineRect.center().x() - 4, subLineRect.center().y() + 3)
                                                 << QPointF(subLineRect.center().x(), subLineRect.center().y() - 3)
                                                 << QPointF(subLineRect.center().x() + 4, subLineRect.center().y() + 3));
            }
            if (sliderOption->subControls & QStyle::SC_ScrollBarAddLine) {
                QRect addLineRect = subControlRect(QStyle::CC_ScrollBar, sliderOption, QStyle::SC_ScrollBarAddLine, widget);
                painter->setPen(Qt::NoPen);
                painter->setBrush(NON_PHOTO_BLUE);
                painter->drawRect(addLineRect);

                // arrow down
                painter->setPen(BERKELEY_BLUE);
                painter->setBrush(BERKELEY_BLUE);
                painter->drawPolygon(QPolygonF() << QPointF(addLineRect.center().x() - 4, addLineRect.center().y() - 3)
                                                 << QPointF(addLineRect.center().x(), addLineRect.center().y() + 3)
                                                 << QPointF(addLineRect.center().x() + 4, addLineRect.center().y() - 3));
            }
        }
    } else {
        QProxyStyle::drawComplexControl(control, option, painter, widget);
    }
}