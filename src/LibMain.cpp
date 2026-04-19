#include "LibMain.h"

#include <gigperformer/sdk/GPMidiMessages.h>
#include <gigperformer/sdk/GPUtils.h>
#include <gigperformer/sdk/GigPerformerAPI.h>

namespace gigperformer
{
namespace sdk
{

GigPerformerAPI *CreateGPExtension(LibraryHandle handle)
{
    return new LibMain(handle);
}

} // namespace sdk
} // namespace gigperformer

// List of menu items
std::vector<std::string> menuNames = {"Show Window", "Hide Window", "Set Dirty", "Set Clean"};

int LibMain::GetMenuCount()
{
    return static_cast<int>(menuNames.size());
}

std::string LibMain::GetMenuName(int index)
{
    std::string text;
    if (index >= 0 && static_cast<std::size_t>(index) < menuNames.size())
    {
        text = menuNames[index];
    }

    return text;
}

void LibMain::InvokeMenu(int index)
{
    if (index >= 0 && static_cast<std::size_t>(index) < menuNames.size())
    {
        switch (index)
        {
        case 0:
            
            break;
        case 1:
            
            break;
        case 2:
            
            break;
        case 3:
            
            break;

        default:
            break;
        }
    }
}

bool caseInsensitiveCompare(const std::string &str1, const std::string &str2)
{
    // A lambda comparator that converts characters to unsigned char first
    // to avoid potential undefined behavior with negative char values
    auto compareChar = [](unsigned char c1, unsigned char c2) { return std::tolower(c1) == std::tolower(c2); };

    // Use std::ranges::equal with the custom comparator
    return std::ranges::equal(str1, str2, compareChar);
}

// looks for songname in setlist and switches to it if it finds it
bool LibMain::switchToSongname(std::string songname)
{
    int count = gigperformer::sdk::GigPerformerFunctions::getSongCount();
    for (int i = 0; i < count; i++)
    {
        if (caseInsensitiveCompare(gigperformer::sdk::GigPerformerFunctions::getSongName(i), songname))
        {
            gigperformer::sdk::GigPerformerFunctions::switchToSong(i, 0);
            // scriptLog("Switching to " + songname, 0);
            return true;
        }
    }
    return false;
}

void LibMain::ProcessOSC(juce::OSCMessage message)
{
    std::string address, widgetname, songname;

    // auto msg = message.getAddressPattern().toString().toStdString();
    // if (message.size() == 1 && message[0].isFloat32())

    address = message.getAddressPattern().toString().toStdString();
    widgetname = "sm_" + address.substr(1);
    scriptLog("Recv " + address, 0);

    if (address == "/play" && message.size() == 1 && message[0].isInt32())
    {
        // setWidgetValue(widgetname, (float) 1.0 * message[0].getInt32());
    }

    if (address == "/barBeat" && message.size() == 3)
    {
        setWidgetCaption("sm_bar", std::to_string(message[0].getInt32()));
        setWidgetCaption("sm_beat", std::to_string(message[1].getInt32()));
        setWidgetCaption("sm_bpm", std::to_string(message[2].getFloat32()));
    }

    if (address == "/songLoaded" && message.size() == 3)
    {
        // scriptLog("Recv " + songname, 0);
        if (inSetlistMode())
        {
            switchToSongname(message[0].getString().toStdString());
        }
    }
}

