#pragma once

#include "coreModels.h"
#include "coreUtils.h"

using namespace CMDUtils;

bool handleChanges(Record& record, CMDParseResult& parsedArgs, std::vector<RecordField>& in_changedFields);