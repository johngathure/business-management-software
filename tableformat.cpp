#include "tableformat.h"

TableFormat::TableFormat(QObject* parent):
    QStyledItemDelegate(parent)
{

}

TableFormat::~TableFormat()
{

}

QString TableFormat::displayText(const QVariant &value, const QLocale &locale) const
{

    QString formatedNumber = locale.toCurrencyString(value.toFloat()," ");

    return formatedNumber;
}

