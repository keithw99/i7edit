/*
  ==============================================================================

    Controller.h
    Created: 26 Jul 2020 11:57:48am
    Author:  Keith

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ParamState.h"

class Controller : public OSCReceiver
{
  
private:
  ParamState paramState_;
};
