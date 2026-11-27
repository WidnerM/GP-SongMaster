#pragma once

#include <gigperformer/sdk/GPMidiMessages.h>
#include <gigperformer/sdk/GPUtils.h>
#include <gigperformer/sdk/GigPerformerAPI.h>
#include <chrono>

#include "juceOSC.h"

#include "Version.h"

// define an XML string describing your product
const std::string XMLProductDescription = "<Library>"
                                          "<Product Name=\"" +
                                          PROJECT_TITLE + "\" Version=\"" + PROJECT_VERSION + "\" BuildDate=\"" +
                                          PROJECT_BUILD_DATE +
                                          "\"></Product>"
                                          "<Description>" +
                                          PROJECT_DESCRIPTION +
                                          "</Description>"
                                          "<ImagePath></ImagePath>"
                                          "</Library>";


// Define your class here - it MUST be called LibMain and it must inherit from
// GigPerformerAPI

class LibMain : public gigperformer::sdk::GigPerformerAPI
{
  protected:
    // These are for creating menu items in Gig Performer that can be used to trigger external functions provided by the
    // extension developer
    int GetMenuCount() override;
    std::string GetMenuName(int index) override;
    void InvokeMenu(int itemIndex) override;

  public:
    // These must be here but no need to do anything unless you want extra
    // behavior
    LibMain(LibraryHandle handle) : GigPerformerAPI(handle)
    {
        lib = this;
    }
    virtual ~LibMain()
    {
    }

    static LibMain *lib;

    // our global variables
    AnOSCReceiver SM_OSC_receive;
    AnOSCSender SM_OSC_sender, SongMasterOSC, VIStageOSC;
    std::string sm_oscaddress = "127.0.0.1:32123";
    std::string sm_vistageaddress = "127.0.0.1:54341";
    std::chrono::steady_clock::time_point lastplayheadtime = std::chrono::steady_clock::now();
    bool followSMbpm = false;

    // from LibMain.cpp
    void ProcessOSC(juce::OSCMessage message);
    bool switchToSongname(std::string songname);

    // void lambdaDemo(std::string textstring);

    /*  virtual void oscMessageReceived(const juce::OSCMessage &message) override
    {
        // auto msg = message.getAddressPattern().toString().toStdString();

        // auto msgx = msg.toString();
        // auto msgy = msgx.toStdString();
        // if (message.size() == 1 && message[0].isFloat32())                           // [5]
        // rotaryKnob.setValue(juce::jlimit(0.0f, 10.0f, message[0].getFloat32())); // [6]
        scriptLog("OSC Message Received: " + message.getAddressPattern().toString().toStdString(), 0);
        ProcessOSC(message);
    } */

    // Override the callback methods in which you are interested
    // and call RegisterCallback for each of these methods
    void OnOpen() override
    {
        // juce::MessageManager::getInstance()->callAsync([]() {});
        // delayTimer.startTimer(500);
        SM_OSC_receive.initialize();
                
        SongMasterOSC.connectToHost("127.0.0.1", 32123); // 32123 is Song Master's default OSC port
        VIStageOSC.connectToHost("127.0.0.1", 54341); // 
        SM_OSC_sender.send("/extension/SongMaster/Test", (float)1.5);

        if (!SM_OSC_receive.connectToPort(54321)) // 54321 is our receive port
            scriptLog("Could not connect to OSC receive port.", 1);
    }

    void OnClose() override
    {

        SongMasterOSC.disconnect();
        VIStageOSC.disconnect();
        SM_OSC_receive.disconnect();
        
        SM_OSC_receive.finalize();

        // juce::MessageManager::deleteInstance();
    }

    void OnGlobalPlayStateChanged(double playing) override
    {
        // juce::String log = "Playhead ";
        // log << ((playing > 0.0) ? "playing" : "stopped");
        // delayTimer.setDirty(playing == 1);
        // LogWindow::showWindow();
        // LogWindow::log(log);
    }

    // A midi device was added or removed
    void OnMidiDeviceListChanged(std::vector<std::string> &inputs, std::vector<std::string> &outputs) override
    {

    }


    void OnWidgetValueChanged(const std::string &widgetName, double newValue) override
    {
        // scriptLog("Widget Callback " + widgetName, 0);
        if (widgetName == "sm_play") SongMasterOSC.send("/play", (int) newValue * 1);
        if (widgetName == "sm_followsmbpm")
        {
            followSMbpm = (newValue == 1.0);
            setWidgetCaption("sm_followsmbpm", followSMbpm ? "Following" : "Local BPM");
        }
    }

    void OnWidgetCaptionChanged(const std::string &widgetName, const std::string &newValue) override
    {
        // scriptLog("Widget Callback " + widgetName, 0);
        if (widgetName == "sm_oscaddress")
        {
            SongMasterOSC.disconnect();
            sm_oscaddress = newValue;
            size_t pos = newValue.find(':');
            if (pos != std::string::npos)
            {
                if (SongMasterOSC.connectToHost(newValue.substr(0, pos), std::stoi("0" + newValue.substr(pos + 1))))
                {
                    scriptLog("Connected to " + newValue, 0);
                }
                else 
                {
                    scriptLog("Failed to connect to " + newValue, 1);
                }
            }
        }
        else if (widgetName == "sm_vistageaddress")
        {
            VIStageOSC.disconnect();
            sm_vistageaddress = newValue;
            size_t pos = newValue.find(':');
            if (pos != std::string::npos)
            {
                if (VIStageOSC.connectToHost(newValue.substr(0, pos), std::stoi("0" + newValue.substr(pos + 1))))
                {
                    scriptLog("Connected to " + newValue, 0);
                }
                else
                {
                    scriptLog("Failed to connect to " + newValue, 1);
                }
            }
        }
    } 

    // examine a vector of widgets and structure the Surface structure to reflect widgets in the rackspace or global
    // rackspace
    void buildSurfaceModel(std::vector<std::string> widgetlist)
    {
        std::string widgetname, prefix, type, bank, column;

        for (auto index = widgetlist.begin(); index != widgetlist.end(); ++index)
        {
            widgetname = *index;
            widgetname = widgetname.substr(widgetname.find(":") + 1);

            if (widgetname.substr(0, widgetname.find("_")) == "sm")
            {
                listenForWidget(widgetname, true);
                scriptLog("Listening for " + widgetname, 0);
            }
        }
    }

    void OnRackspaceActivated() override
    {
        uint8_t row, column, bank;
        std::string widgetname, setting;
        std::vector<std::string> widgetlist, globalwidgetlist;

        getWidgetList(globalwidgetlist, true);
        getWidgetList(widgetlist, false);
        widgetlist.insert(widgetlist.end(), globalwidgetlist.begin(), globalwidgetlist.end());
        buildSurfaceModel(widgetlist);
    }
    
    void Initialization() override
    {
        // Do any initialization that you need

        // .... your code here

        // Finally, register all the methods that you are going to actually use,
        // i.e, the ones you declared above as override
        registerCallback("OnOpen");
        registerCallback("OnClose");
        registerCallback("OnRackspaceActivated");
        registerCallback("OnWidgetValueChanged");
        registerCallback("OnWidgetCaptionChanged");
        // registerCallback("OnGlobalPlayStateChanged");
        // registerCallback("OnMidiDeviceListChanged");
    }

    // Generally don't touch this - simply define the constant
    // 'XMLProductDescription' at the top of this file with an XML description of
    // your product
    std::string GetProductDescription() override // This MUST be defined in your class
    {
        return XMLProductDescription;
    }

};
