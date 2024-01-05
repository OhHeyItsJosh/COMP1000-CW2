#ifndef DATAVALIDATION_H
#define DATAVALIDATION_H

#include <cstdint>
#include <functional>
#include "QString"
#include "QWidget"
#include "QDebug"
#include "QMessageBox"

namespace DataValidation
{
bool wordCountCheck(const QString& input, int32_t min, int32_t max);
void showInvalidBoundsMessage(QWidget* parent, int32_t min, int32_t max);
}


#endif // DATAVALIDATION_H
