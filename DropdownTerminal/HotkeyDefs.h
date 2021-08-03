#pragma once
#include <map>
#include <string>

/*
 I've used magic numbers to decouple this header from the WINAPI
 A detailed overview over missing hotkeys and/or mods can be found here:
 https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerhotkey
*/
std::map<const std::string, const uint64_t> KEYMODS = {
	{"ALT", 0x0001},
	{"CTRL", 0x0002},
	{"SHIFT", 0x0004},
	{"WIN", 0x0008}
};

/*
 I've used magic numbers to decouple this header from the WINAPI
 A detailed overview over missing hotkeys and/or mods can be found here:
 https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
*/
std::map<const std::string, const uint32_t> HOTKEYS = {
        {"L-MOUSE", 0x01},
        {"R-MOUSE", 0x02},
        {"M-MOUSE", 0x04},
        {"X1-MOUSE", 0x05},
        {"X-MOUSE", 0x06},
        {"BACKSPACE", 0x08},
        {"TAB", 0x09},
        {"CLEAR", 0x0C},
        {"ENTER", 0x0D},
        {"SHIFT", 0x10},
        {"CONTROL", 0x11},
        {"ALT", 0x12},
        {"PAUSE", 0x13},
        {"CAPS_LOCK", 0x14},
        {"IME Hangul mode", 0x15},
        {"IME On", 0x16},
        {"IME Junja mode", 0x17},
        {"IME final mode", 0x18},
        {"IME Hanja mode", 0x19},
        {"IME Kanji mode", 0x19},
        {"IME Off", 0x1A},
        {"ESC", 0x1B},
        {"IME convert", 0x1C},
        {"IME nonconvert", 0x1D},
        {"IME accept", 0x1E},
        {"IME mode change request", 0x1F},
        {"SPACE", 0x20},
        {"Page Up", 0x21},
        {"Page Down", 0x22},
        {"END", 0x23},
        {"HOME", 0x24},
        {"LEFT", 0x25},
        {"UP", 0x26},
        {"RIGHT", 0x27},
        {"DOWN", 0x28},
        {"SELECT", 0x29},
        {"PRINT", 0x2A},
        {"EXECUTE", 0x2B},
        {"PRINT-SCREEN", 0x2C},
        {"INSERT", 0x2D},
        {"DELETE", 0x2E},
        {"HELP", 0x2F},
        {"0", 0x30},
        {"1", 0x31},
        {"2", 0x32},
        {"3", 0x33},
        {"4", 0x34},
        {"5", 0x35},
        {"6", 0x36},
        {"7", 0x37},
        {"8", 0x38},
        {"9", 0x39},
        {"A", 0x41},
        {"B", 0x42},
        {"C", 0x43},
        {"D", 0x44},
        {"E", 0x45},
        {"F", 0x46},
        {"G", 0x47},
        {"H", 0x48},
        {"I", 0x49},
        {"J", 0x4A},
        {"K", 0x4B},
        {"L", 0x4C},
        {"M", 0x4D},
        {"N", 0x4E},
        {"O", 0x4F},
        {"P", 0x50},
        {"Q", 0x51},
        {"R", 0x52},
        {"S", 0x53},
        {"T", 0x54},
        {"U", 0x55},
        {"V", 0x56},
        {"W", 0x57},
        {"X", 0x58},
        {"Y", 0x59},
        {"Z", 0x5A},
        {"LEFT WINKEY", 0x5B},
        {"RIGHT WINKEY", 0x5C},
        {"APP KEY", 0x5D},
        {"Sleep", 0x5F},
        {"NUMPAD 0", 0x60},
        {"NUMPAD 1", 0x61},
        {"NUMPAD 2", 0x62},
        {"NUMPAD 3", 0x63},
        {"NUMPAD 4", 0x64},
        {"NUMPAD 5", 0x65},
        {"NUMPAD 6", 0x66},
        {"NUMPAD 7", 0x67},
        {"NUMPAD 8", 0x68},
        {"NUMPAD 9", 0x69},
        {"MULT", 0x6A},
        {"ADD", 0x6B},
        {"SEPERATOR - kp was das sein soll", 0x6C},
        {"MINUS", 0x6D},
        {"KOMMA", 0x6E},
        {"DIVIDE", 0x6F},
        {"F1", 0x70},
        {"F2", 0x71},
        {"F3", 0x72},
        {"F4", 0x73},
        {"F5", 0x74},
        {"F6", 0x75},
        {"F7", 0x76},
        {"F8", 0x77},
        {"F9", 0x78},
        {"F10", 0x79},
        {"F11", 0x7A},
        {"F12", 0x7B},
        {"F13", 0x7C},
        {"F14", 0x7D},
        {"F15", 0x7E},
        {"F16", 0x7F},
        {"F17", 0x80},
        {"F18", 0x81},
        {"F19", 0x82},
        {"F20", 0x83},
        {"F21", 0x84},
        {"F22", 0x85},
        {"F23", 0x86},
        {"F24", 0x87},
        {"NUMLOCK", 0x90},
        {"SCROLL LOCK", 0x91},
        {"LSHIFT", 0xA0},
        {"RSHIFT", 0xA1},
        {"LCTRL", 0xA2},
        {"RCTRL", 0xA3},
        {"LMENU", 0xA4},
        {"RMENU", 0xA5},
        {"OEM_PLUS", 0xBB},
        {"OEM_COMMA", 0xBC},
        {"OEM_MINUS", 0xBD},
        {"OEM_PERIOD", 0xBE}
};

std::string getHotkeyNameByUINT(const uint32_t key)
{
    for (auto it = HOTKEYS.begin(); it != HOTKEYS.end(); ++it)
    {
	    if (it->second == key)
	    {
            return it->first;
	    }
    }
    return "";
}

std::string getModHotkeyNameByUINT(const uint32_t key)
{
    for (auto it = KEYMODS.begin(); it != KEYMODS.end(); ++it)
    {
	    if (it->second == key)
	    {
            return it->first;
	    }
    }
    return "";
}