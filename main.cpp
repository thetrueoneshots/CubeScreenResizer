#include <fstream>
#include "main.h"

void Popup(const char* title, const char* msg) {
	MessageBoxA(0, msg, title, MB_OK | MB_ICONINFORMATION);
}

/* Mod class containing all the functions for the mod.
*/
class Mod : GenericMod {

	/* Hook for the chat function. Triggers when a user sends something in the chat.
	 * @param	{std::wstring*} message
	 * @return	{int}
	*/
	virtual int OnChat(std::wstring* message) override {
		return 0;
	}

	/* Function hook that gets called every game tick.
	 * @param	{cube::Game*} game
	 * @return	{void}
	*/
	virtual void OnGameTick(cube::Game* game) override {
		return;
	}

	/* Function hook that gets called on intialization of cubeworld.
	 * [Note]:	cube::GetGame() is not yet available here!!
	 * @return	{void}
	*/
	virtual void Initialize() override {
		std::string separator(" ");
		auto offset = 0x04;
		std::string line;

		// Open file
		std::ifstream myfile("options.cfg");
		if (!myfile.is_open()) {
			Popup("Error", "Cannot open config file for screen size.");
			return;
		}

		std::getline(myfile, line);
		// Read first line and set resolutionX
		std::getline(myfile, line);
		auto index = line.find(separator);
		if (index != std::string::npos) {
			try {
				auto resolutionX = std::stoi(line.substr(index + 1));
				auto addressX = 0x13591C;
				auto baseX = (char*)CWBase() + addressX + offset;
				WriteByte(baseX, resolutionX & 0xFF);
				WriteByte(baseX + 0x01, resolutionX >> 8);
				
			}
			catch (...) {
				Popup("Error", "Cannot set resolutionX.");
			}
		}

		// Read second line and set resolutionY
		std::getline(myfile, line);
		index = line.find(separator);
		if (index != std::string::npos) {
			try {
				const auto resolutionY = std::stoi(line.substr(index + 1));
				auto addressY = 0x135914;
				auto baseY = (char*)CWBase() + addressY + offset;
				WriteByte(baseY, resolutionY & 0xFF);
				WriteByte(baseY + 0x01, resolutionY >> 8);
			}
			catch (...) {
				Popup("Error", "Cannot set resolutionY.");
			}
		}

		return;
	}
};

// Export of the mod created in this file, so that the modloader can see and use it.
EXPORT Mod* MakeMod() {
	return new Mod();
}