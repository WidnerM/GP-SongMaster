#include "CallbackSendmidi.h"
#include "LibMain.h"

LibMain *lib = new LibMain(nullptr); // mrw - this is needed as a reference to LibMain

void DelayTimer::timerCallback()
{
    if (lib == nullptr)
        return;
    if (dirty)
    {
        lib->scriptLog("delayTimer callback " + std::to_string(dirty), 0);
    }
}

void DelayTimer::setDirty(bool value)
{
    dirty = value;
}

/*  SoftbuttonTimer *SoftbuttonTimer::sbInstance = nullptr;


SoftbuttonTimer::SoftbuttonTimer()
{
    currentmessage = "";
    destinationport = "";
}

SoftbuttonTimer::~SoftbuttonTimer()
{
    currentmessage = nullptr;
    destinationport = nullptr;
} 

void SoftbuttonTimer::initialize()
{
    if (sbInstance == nullptr)
    {
        sbInstance = new SoftbuttonTimer();
    }
}

void SoftbuttonTimer::timerCallback()
{
    // LibMain::scriptLog("Callback", 0);
    // this->stopTimer();
}

void SoftbuttonTimer::setCurrentMessage(std::string midimessage)
{
    if (sbInstance != nullptr)
    {
        currentmessage = midimessage;
    }
} */
