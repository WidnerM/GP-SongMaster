#pragma once
#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_osc/juce_osc.h>

using namespace juce;

// juce::OSCSender SM_OSC_sender;
class AnOSCSender : public Component, public OSCSender
{
  public:
    AnOSCSender();
    virtual ~AnOSCSender() override;
    bool connectToHost(const String &hostname, int port);   
  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnOSCSender)
};

// OSC Receiver class.  Instantiate, then call connectToPort(portnumber)
class AnOSCReceiver : public Component,
                      public OSCReceiver,
                      public OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
  public:
    //==============================================================================
    AnOSCReceiver();
    virtual ~AnOSCReceiver() override;

    void static initialize();
    void static finalize();

    bool connectToPort(int port);

  private:
    //==============================================================================
    void oscMessageReceived(const juce::OSCMessage &message) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnOSCReceiver)
};

/*  class MyOSCReceiver : public juce::Component,
                      public juce::OSCReceiver,
                      public juce::OSCReceiver::Listener<juce::OSCReceiver::MessageLoopCallback>
{
  public:
    //==============================================================================
    MyOSCReceiver()
    {
        addListener(this);
    }

    virtual ~MyOSCReceiver()
    {
        disconnect();
    }

    bool connectToPort(int port)
    {
        return connect(port);
    }

  private:
    // trying to figure out (unsuccessfully) how to make this MyOSCReceiver usable directly in LibMain
    void oscMessageReceived(const juce::OSCMessage &message) override;
    {
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyOSCReceiver)
}; */


/*  class AnOSCReceiverWithAddress
    : public Component,
      public OSCReceiver,
      public OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback>
{
  public:
    //==============================================================================
    AnOSCReceiverWithAddress()
    {
        connect(54321);
        addListener(this, "/play");
    }

    virtual ~AnOSCReceiverWithAddress()
    {
        disconnect();
    }


  private:
    //==============================================================================
    void oscMessageReceived(const juce::OSCMessage &message) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnOSCReceiverWithAddress)
};
*/