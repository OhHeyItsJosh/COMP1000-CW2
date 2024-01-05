#include "dataValidation.h"
#include <sstream>

bool DataValidation::wordCountCheck(const QString& input, int32_t min, int32_t max)
{
    QStringList inputSplit = input.split(" ");

    // lower bounds check
    if (inputSplit.length() < min)
        return false;

    // if there is no upper bound
    if (max == -1)
        return true;

    // upper bounds check
    if (inputSplit.length() > max)
        return false;

    return true;
}

void DataValidation::showInvalidBoundsMessage(QWidget* parent, int32_t min, int32_t max)
{
    std::stringstream builder;
    if (max == -1)
        builder << "This field requires at least " << min << " words";
    else if (min == max)
        builder << "This field requires " << min << " word(s)";
    else
        builder << "This field requires between " << min << " - " << max << " words";

    QMessageBox::critical(parent, "Invalid Input", QString::fromStdString(builder.str()));
}
