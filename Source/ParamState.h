/*
  ==============================================================================

    ParamState.h
    Created: 26 Jul 2020 12:00:55pm
    Author:  Keith

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct Part {
  ToneId toneId;
};

class TemporaryStudioSet {
public:
  TemporaryStudioSet();
  Part* getPart(const int partNumber);
  ToneId getToneForPart(const int partNumber);
  
private:
  OwnedArray<Part> parts_;
};

class ParamState
{
public:
  Part* getActivePart(const int partNumber);
  ToneId getToneForActivePart(const int partNumber);
  
private:
  TemporaryStudioSet tempStudioSet_;
};
