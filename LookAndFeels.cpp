/*
  ==============================================================================

    LookAndFeels.cpp
    Created: 9 Feb 2017 6:44:51pm
    Author:  Samuel Thompson Parke-Wolfe

  ==============================================================================
*/

#include "LookAndFeels.h"

LookAndFeel_Mixer::LookAndFeel_Mixer()
{
    
}

void LookAndFeel_Mixer::drawToggleButton (Graphics& g, ToggleButton& button, bool isMouseOverButton, bool isButtonDown)
{
    
    Rectangle<int> buttonArea = button.getLocalBounds();
    const int edgeX = 4;
    const int edgeY = 4;

//    buttonArea.removeFromLeft (edge);
//    buttonArea.removeFromTop (edge);
    buttonArea.reduce(edgeX, edgeY);
    // shadow
//    g.setColour (Colours::lightgrey.withAlpha (0.5f));
//    g.fillRect (buttonArea);
    
//    const int offset = isButtonDown ? -edge / 2 : -edge;
//    buttonArea.translate (offset, offset);
    
    if(button.getToggleState())
    {
        g.setColour (Colours::lightgrey.withAlpha (1.0f));
    } else {
        g.setColour (Colours::lightgrey.withAlpha (0.25f));
    }
    g.fillRect (buttonArea);
    
}
