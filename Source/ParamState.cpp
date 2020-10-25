/*
  ==============================================================================

    ParamState.cpp
    Created: 26 Jul 2020 12:00:55pm
    Author:  Keith

  ==============================================================================
*/

#include "ParamState.h"


TemporaryStudioSet::TemporaryStudioSet()
{
  for (int i = 0; i < 16; ++i) {
    parts_.insert(0, std::make_unique<Part>());
  }
}

Part* TemporaryStudioSet::getPart(const int partNumber)
{
  if (partNumber < 1 || partNumber > 16) {
     DBG ("ERROR: invalid part number: " + String(partNumber));
     return nullptr;
   }
   return parts_[partNumber - 1];
}

ToneId TemporaryStudioSet::getToneForPart(const int partNumber)
{
  Part* part = getPart(partNumber);
   if (part == nullptr) {
     // Return empty ToneId.
     return ToneId();
   }
  return part->toneId;
}

Part* ParamState::getActivePart(int partNumber)
{
  return tempStudioSet_.getPart(partNumber);
}

ToneId ParamState::getToneForActivePart(int partNumber)
{
  return tempStudioSet_.getToneForPart(partNumber);
}
