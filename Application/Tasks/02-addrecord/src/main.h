#pragma once

#include "coreUtils.h"
#include "coreModels.h"

std::optional<Record> parseRecordInput(CMDUtils::CMDParseResult& parsedArgs, Database& database);