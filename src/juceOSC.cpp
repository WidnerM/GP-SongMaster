#include "LibMain.h"
#include "juceOSC.h"

LibMain *LibMain::lib = nullptr;

AnOSCSender::AnOSCSender()
{
    // addListener(this);
}

AnOSCSender::~AnOSCSender()
{
    disconnect();
}

bool AnOSCSender::connectToHost(const juce::String &hostname, int port)
{
    return connect(hostname, port);
}

AnOSCReceiver::AnOSCReceiver()
{
    addListener(this);
}

AnOSCReceiver::~AnOSCReceiver()
{
    disconnect();
}

void AnOSCReceiver::initialize()
{
    juce::MessageManager::getInstance()->callAsync([]() {});
}

void AnOSCReceiver::finalize()
{
    juce::MessageManager::deleteInstance();
}

bool AnOSCReceiver::connectToPort(int port)
{
    return connect(port);
}

void AnOSCReceiver::oscMessageReceived(const juce::OSCMessage &message)
{
    // auto msg = message.getAddressPattern().toString().toStdString();

    // auto msgx = msg.toString();
    // auto msgy = msgx.toStdString();
    // if (message.size() == 1 && message[0].isFloat32())                           // [5]
    // rotaryKnob.setValue(juce::jlimit(0.0f, 10.0f, message[0].getFloat32())); // [6]
    LibMain::lib->ProcessOSC(message);
}

//void AnOSCReceiverWithAddress::oscMessageReceived(const juce::OSCMessage &message)
//{
//    auto msg = message.getAddressPattern().toString().toStdString();

    // auto msgx = msg.toString();
    // auto msgy = msgx.toStdString();
    // if (message.size() == 1 && message[0].isFloat32())                           // [5]
    // rotaryKnob.setValue(juce::jlimit(0.0f, 10.0f, message[0].getFloat32())); // [6]
    // xlib->scriptLog(msg, 0);
//}