/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"


Component* createMainContentComponent();


class MySplash1 : public SplashScreen
{
//    Image* splashImage;
//    Time timeA;
//    Time timeB;
    
    String _title;
public:
    MySplash1(const String& title, int width, int height, bool useDropShadow)
    :
    SplashScreen(title, width, height, useDropShadow)
    {
        _title = title;
    }
    void paint (Graphics& g)
    {
        
        //SplashScreen::paint(g);
        g.setColour(Colours::darkgrey);
        g.fillRect(getLocalBounds());
        g.setColour(Colours::lightgrey);
        g.drawText(_title, getLocalBounds(), Justification::centred);
    }
    
};

class MySplash2 : public SplashScreen
{
    //    Image* splashImage;
    //    Time timeA;
    //    Time timeB;
    
    String _title;
public:
    MySplash2(const String& title, int width, int height, bool useDropShadow)
    :
    SplashScreen(title, width, height, useDropShadow)
    {
        _title = title;
    }
    void paint (Graphics& g)
    {
        
        //SplashScreen::paint(g);
        g.setColour(Colours::darkgrey);
        g.fillRect(getLocalBounds());
        g.setColour(Colours::lightgrey);
        
        Rectangle<int> area(getLocalBounds());
        area.reduce(40, 0);
        
        String text0("Incomplete is a malleable piece of artwork which allows its content to be moulded by your aesthetic intuition.");
        String text1("The interface enables you to interact with Incomplete's system through a grid of 8x10 squares.");
        String text2("Each square triggers the playback of a sample; samples from the first eight columns are looped sequences while samples from the last two columns are one-shot sounds.");
        String text3("Each vertical column stores samples from a different type of instrument.");
        String text4("Within each column, only one sample can be played at any given time.");
                    
        String text5("By combining, removing and introducing the samples stored in the different columns you will be able to shape Incomplete's musical content thus becoming its co-author.");
        
        float gap = area.getWidth()/6.0;
        g.drawText(text0, area.removeFromTop(gap), Justification::centred, 50);
        g.drawText(text1, area.removeFromTop(gap), Justification::centred, 50);
        g.drawText(text2, area.removeFromTop(gap), Justification::centred, 50);
        g.drawText(text3, area.removeFromTop(gap), Justification::centred, 50);
        g.drawText(text4, area.removeFromTop(gap), Justification::centred, 50);
        g.drawText(text5, area.removeFromTop(gap), Justification::centred, 50);
    }
    
};


//==============================================================================
class TheosAppApplication  : public JUCEApplication
{
    
    //ScopedPointer<SplashScreen> splash;
    SplashScreen *splash1, *splash2, *splash3;
    
    
public:
    //==============================================================================
    TheosAppApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..
        

        mainWindow = new MainWindow (getApplicationName());
        
        
        int size = 654;
        
        //640/2, 1136/2
        splash3 = new MySplash2 ("Instructions", size, size, false);
        splash3->deleteAfterDelay(RelativeTime::seconds (60), true);
        
        splash2 = new MySplash1 ("Fedor Shtern", size, size, false);
        splash2->deleteAfterDelay(RelativeTime::seconds (2), false);
        
        
        //BinaryData::_17015043_1459264774097503_575012170_o_jpg;
        
        Image image = ImageFileFormat::loadFrom (BinaryData::immage_png, (size_t) BinaryData::immage_pngSize);
        
        image = image.rescaled(size, size);
        
        splash1 = new SplashScreen("Pic", image, false);
        splash1->deleteAfterDelay(RelativeTime::seconds (2), false);

        
        //SplashScreen (const String& title, const Image& image, bool useDropShadow)
        
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..
        
        splash1 = nullptr;
        splash2 = nullptr;
        splash3 = nullptr;

        
        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainContentComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (createMainContentComponent(), true);
            setResizable (true, true);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (TheosAppApplication)
