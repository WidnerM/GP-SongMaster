#include "LibMain.h"

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
std::vector<std::string> menuNames = {"Follow SM BPM", "Don't Follow SM BPM", "Set Dirty", "Set Clean"};

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
            followSMbpm = true;
            break;
        case 1:
            followSMbpm = false;
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
        setPlayheadState(message[0].getInt32() == 1);
    }

    if (address == "/playhead" && message.size() == 1 && message[0].isFloat32())
    {
        // VIStageOSC.send("/VI-Stage/SeekPosition", message[0].getFloat32());
        setWidgetCaption("sm_playhead", std::to_string(message[0].getFloat32()));
        lastplayheadtime = std::chrono::steady_clock::now();
    }

    if (address == "/barBeat" && message.size() == 3)
    {
        // we don't want to request the playhead position too often
        if (lastplayheadtime + std::chrono::milliseconds(100) < std::chrono::steady_clock::now())
        {
            SongMasterOSC.send("/playhead"); // request playhead position from Song Master
        }
        if (followSMbpm)
        {
            setBPM(message[2].getFloat32());
        }
        setWidgetCaption("sm_bar", std::to_string(message[0].getInt32()));
        setWidgetCaption("sm_beat", std::to_string(message[1].getInt32()));
        setWidgetCaption("sm_bpm", std::to_string(message[2].getFloat32()));
    }

    if (address == "/bpm" && message.size() == 1)
    {
        if (followSMbpm)
        {
            setBPM(message[0].getFloat32());
        }
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

