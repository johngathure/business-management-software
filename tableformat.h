#ifndef TABLEFORMAT_H
#define TABLEFORMAT_H

#include <QStyledItemDelegate>


class TableFormat : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TableFormat(QObject* parent = 0);

    virtual QString displayText(const QVariant &value, const QLocale &locale) const;

    ~TableFormat();
};

#endif // TABLEFORMAT_H
