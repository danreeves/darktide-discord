#include "lua.h"
#include "PluginApi128.h"
#include "lua_helpers.h"

#ifdef _DEBUG
#include <Windows.h>
#endif

#include "clue.hpp"

#include "discord_game_sdk/cpp/discord.h"


discord::Core* core{};


static void setup_game(GetApiFunction get_engine_api)
{
	auto lua = (LuaApi128*)get_engine_api(LUA_API_ID);

	__int64 id = 1111429477055090698;
	auto result = discord::Core::Create(id, DiscordCreateFlags_NoRequireDiscord, &core);
	discord::Activity activity{};
	activity.SetState("In the Mourning Star");
	activity.SetDetails("Modding");

	activity.GetAssets().SetLargeImage("darktide");
	activity.GetAssets().SetSmallImage("darktide");
	//activity.GetAssets().SetLargeText("darktide");
	//activity.GetAssets().SetSmallText("darktide");

	core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {

	});

MessageBoxA(NULL, "done discord stuff", "Loaded", 0);

}

static const char* get_name()
{
	return "rawray";
}

static void loaded(GetApiFunction get_engine_api)
{
	LOG_INFO("------Loaded RawRay-------");
#ifdef _DEBUG
	MessageBoxA(NULL, "Loaded", "Loaded", 0);
#endif
}

static void update(float dt)
{

	::core->RunCallbacks();
	//rawray::lua::update(dt);
}

static void shutdown()
{
#ifdef _DEBUG
	MessageBoxA(NULL, "shutdown", "shutdown", 0);
#endif
	//rawray::lua::unload();
}

extern "C" {
	void* get_dynamic_plugin_api(unsigned api)
	{
		if (api == PLUGIN_API_ID) {

			MessageBoxA(NULL, "creating plugin", "get_dynamic_plugin_api", 0);
			static PluginApi128 api{};
			api.get_name = get_name;
			api.setup_game = setup_game;
			api.loaded = loaded;
			api.update_game = update;
			api.shutdown_game = shutdown;
			return &api;
		}
		return nullptr;
	}

#if !defined STATIC_PLUGIN_LINKING
	PLUGIN_DLLEXPORT void* get_plugin_api(unsigned api)
	{
		MessageBoxA(NULL, "get_plugin_api", "get_plugin_api", 0);
		return get_dynamic_plugin_api(api);
	}
#endif
}
