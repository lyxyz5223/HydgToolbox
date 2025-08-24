#pragma once
#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QFont>
#include <QRect>
#include <QString>

class PluginCardDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    PluginCardDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        painter->save();
        // 绘制卡片背景
        QRect rect = option.rect;
        painter->setRenderHint(QPainter::Antialiasing);
        QColor bgColor = option.state & QStyle::State_Selected ? QColor(220, 240, 255) : QColor(245, 245, 245);
        painter->setBrush(bgColor);
        painter->setPen(Qt::NoPen);
        painter->drawRoundedRect(rect.adjusted(2,2,-2,-2), 8, 8);

        // 获取插件信息
        QString id = index.data(Qt::UserRole).toString();
        QString name = index.data(Qt::UserRole + 1).toString();
        QString version = index.data(Qt::UserRole + 2).toString();
        QString author = index.data(Qt::UserRole + 3).toString();
        QString description = index.data(Qt::UserRole + 4).toString();
        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));

        // 图标
        QRect iconRect(rect.left() + 10, rect.top() + 10, 48, 48);
        icon.paint(painter, iconRect);

        // 名称
        QFont nameFont = option.font;
        nameFont.setBold(true);
        nameFont.setPointSize(12);
        painter->setFont(nameFont);
        painter->setPen(QColor(40, 40, 40));
        painter->drawText(rect.left() + 70, rect.top() + 25, rect.width() - 80, 20, Qt::AlignLeft | Qt::AlignVCenter, name);

        // 版本和作者
        QFont infoFont = option.font;
        infoFont.setPointSize(10);
        painter->setFont(infoFont);
        painter->setPen(QColor(80, 80, 80));
        painter->drawText(rect.left() + 70, rect.top() + 45, rect.width() - 80, 18, Qt::AlignLeft | Qt::AlignVCenter,
                          QString("版本: %1    作者: %2").arg(version, author));

        // 描述
        QFont descFont = option.font;
        descFont.setPointSize(9);
        painter->setFont(descFont);
        painter->setPen(QColor(100, 100, 100));
        painter->drawText(rect.left() + 10, rect.top() + 65, rect.width() - 20, rect.height() - 75,
                          Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop, description);

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        return QSize(240, 110); // 卡片大小
    }
};
