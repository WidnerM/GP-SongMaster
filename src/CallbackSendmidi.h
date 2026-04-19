#pragma once
// #include "LibMain.h"
#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

// mrw timer class definition
class DelayTimer : public juce::Timer
{
  public:
    void timerCallback() override;
    void setDirty(bool value);
    bool dirty = false;

};


/*  class SoftbuttonTimer : public juce::Timer
{
    public:
        SoftbuttonTimer();
        ~SoftbuttonTimer() override;

        void static initialize();
        void static finalize();

        void static setCurrentMessage(std::string midimessage); // the message to send
        // void static setDestination(std::string midiport);
        // virtual void timerCallback() override;
        static std::string currentmessage;
        static std::string destinationport;

    private:
        static SoftbuttonTimer *sbInstance;

}; */
