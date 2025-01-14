#include "gxt_editor.hpp"
#include "../../cheat/file_system.hpp"
#include "../../hooking/native/natives.hpp"
#include <ini.h>

namespace Cheat
{
	namespace Features
	{
		/*
		Called by get_label_text hook, returns custom GXT string result if set in custom_strings or default_strings
		*/
		bool gxt_editor::get_string(std::string gxt_string, const char*& modified_string)
		{
			// Custom strings
			auto c_it = custom_strings.find(gxt_string);
			if (c_it != custom_strings.end())
			{
				modified_string = c_it->second.c_str();
				return true;
			}
			
			// Default strings
			auto d_it = default_strings.find(gxt_string);
			if (d_it != default_strings.end())
			{
				modified_string = d_it->second.c_str();
				return true;
			}

			return false;
		}
		void gxt_editor::create_custom(std::string gxt_string, std::string modified_string)
		{
			// Create file
			if (!std::filesystem::exists(file_system::paths::GXTFile))
			{
				file_system::write_file(file_system::paths::GXTFile, "; G MODZ MENU GO BRR - contents must always comply with INI file standards");
			}

			file_system::ini_file::write(modified_string, file_system::paths::GXTFile, "GXT", gxt_string);

			// Reload map data
			load_custom_file();
		}
		bool gxt_editor::remove_custom(std::string gxt_string)
		{
			if (file_system::ini_file::remove_key(file_system::paths::GXTFile, "GXT", gxt_string))
			{
				load_custom_file();
				return true;
			}			
			return false;
		}
		void gxt_editor::load_custom_file()
		{
			// Setup mINI structure and get INI data
			mINI::INIFile File(file_system::paths::GXTFile);
			mINI::INIStructure StructData;
			File.read(StructData);

			// No point continuing if the mINI structure is empty
			if (StructData.size() < 1)
				return;
			
			// Clear custom_strings
			if (!custom_strings.empty())
				custom_strings.clear();

			// Fill custom_strings
			for (auto const& it : StructData)
			{
				for (auto const& it2 : it.second)
				{
					if (UI::DOES_TEXT_LABEL_EXIST(it2.first.c_str()))
					{
						std::string GXT = it2.first;
						std::transform(GXT.begin(), GXT.end(), GXT.begin(), ::toupper);
						custom_strings.insert({ GXT, it2.second });
					}
				}
			}
		}

		// Maps
		std::map<std::string, std::string> gxt_editor::custom_strings;
		const std::map<std::string, std::string> gxt_editor::default_strings
		{
			{ "HUD_MPREENTER", "G MODZ ONLINE / BANN GO BRR" },
			{ "HUD_JOINING", "Loading G MODZ ONLINE" },
			{ "PM_QUIT_MP", "Leave G MODZ ONLINE" },
			{ "PM_ENTER_MP", "Join G MODZ GAMES" },
			{ "PM_EXIT_GAME", "Exit G MODZ GAMES" },
			{ "PM_GO", "Go G MODZ " },
			{ "PM_FRIEND_FM", "Join G MODZ BODDYS" },
			{ "PM_FIND_SESS", "Find new G MODZ LOBBY" }
		};
	}
}