/*
  ==============================================================================

    LookAndFeels.h
    Created: 9 Feb 2017 6:44:51pm
    Author:  Samuel Thompson Parke-Wolfe

  ==============================================================================
*/

#ifndef LOOKANDFEELS_H_INCLUDED
#define LOOKANDFEELS_H_INCLUDED

#include "JuceHeader.h"

class LookAndFeel_Mixer : public LookAndFeel_V3
{
public:
    
    LookAndFeel_Mixer();
    
    void drawToggleButton (Graphics& g, ToggleButton& button, bool isMouseOverButton, bool isButtonDown) override;
    
};



#endif  // LOOKANDFEELS_H_INCLUDED
