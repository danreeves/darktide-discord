#include "lua.h"
#include "PluginApi128.h"
// #include "lua_helpers.h"

#ifdef _DEBUG
#include <Windows.h>
#endif

#include "clue/clue.hpp"

#include "discord/discord.h"

#include <sstream>

discord::Core *core{};
discord::Activity activity{};

LuaApi128 *lua{};
LoggingApi *logger{};

static int set_state(lua_State *L)
{
	if (lua->isstring(L, 1))
	{
		const char *value = lua->tolstring(L, 1, nullptr);
		activity.SetState(value);
		lua->pushboolean(L, 1); // success
	}
	else
	{
		lua->pushboolean(L, 0); // error
	}
	return 1; /* number of results */
}

static int set_details(lua_State *L)
{
	if (lua->isstring(L, 1))
	{
		const char *value = lua->tolstring(L, 1, nullptr);
		activity.SetDetails(value);
		lua->pushboolean(L, 1); // success
	}
	else
	{
		lua->pushboolean(L, 0); // error
	}
	return 1; /* number of results */
}

static int set_class(lua_State *L)
{
	if (lua->isstring(L, 1) && lua->isstring(L, 2))
	{
		const char *archetype = lua->tolstring(L, 1, nullptr);
		const char *details = lua->tolstring(L, 2, nullptr);
		activity.GetAssets().SetSmallImage(archetype);
		activity.GetAssets().SetSmallText(details);
		lua->pushboolean(L, 1); // success
	}
	else
	{
		lua->pushboolean(L, 0); // error
	}
	return 1; /* number of results */
}

static int set_party_size(lua_State *L)
{
	if (lua->isnumber(L, 1) && lua->isnumber(L, 2))
	{
		int current_size = lua->tonumber(L, 1);
		int max_size = lua->tonumber(L, 2);
		activity.GetParty().GetSize().SetCurrentSize(current_size);
		activity.GetParty().GetSize().SetMaxSize(max_size);
		lua->pushboolean(L, 1); // success
	}
	else
	{
		lua->pushboolean(L, 0); // error
	}
	return 1; /* number of results */
}

static int set_start_time(lua_State *L)
{

	std::time_t result = std::time(nullptr);
	activity.GetTimestamps().SetStart(result);
	lua->pushboolean(L, 1); // success
	return 1;				/* number of results */
}

static void update()
{
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
										   {
		if (result != discord::Result::Ok) {
			char message[255] = "";
			sprintf_s(message, "non-zero update result: %d", result);
			logger->info("DarktideDiscord", message);
		} });
}

static int lua_update(lua_State *L)
{
	update();
	return 0;
}

static void setup_game(GetApiFunction get_engine_api)
{
	lua = (LuaApi128 *)get_engine_api(LUA_API_ID);
	logger = (LoggingApi *)get_engine_api(LOGGING_API_ID);

	lua->set_module_number("DarktideDiscord", "VERSION", 1);
	lua->add_module_function("DarktideDiscord", "set_state", set_state);
	lua->add_module_function("DarktideDiscord", "set_details", set_details);
	lua->add_module_function("DarktideDiscord", "set_class", set_class);
	lua->add_module_function("DarktideDiscord", "set_party_size", set_party_size);
	lua->add_module_function("DarktideDiscord", "set_start_time", set_start_time);
	lua->add_module_function("DarktideDiscord", "update", lua_update);

	__int64 id = 1111429477055090698;
	auto result = discord::Core::Create(id, DiscordCreateFlags_NoRequireDiscord, &core);
	activity.GetAssets().SetLargeImage("darktide");
	update();

#ifdef _DEBUG
	MessageBoxA(NULL, "done discord stuff", "Loaded", 0);
#endif
}

static const char *get_name()
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
	// rawray::lua::update(dt);
}

static void shutdown()
{
#ifdef _DEBUG
	// MessageBoxA(NULL, "shutdown", "shutdown", 0);
#endif
	// rawray::lua::unload();
}

extern "C"
{
	void *get_dynamic_plugin_api(unsigned api)
	{
		if (api == PLUGIN_API_ID)
		{

			// MessageBoxA(NULL, "creating plugin", "get_dynamic_plugin_api", 0);
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
	PLUGIN_DLLEXPORT void *get_plugin_api(unsigned api)
	{
		// MessageBoxA(NULL, "get_plugin_api", "get_plugin_api", 0);
		return get_dynamic_plugin_api(api);
	}
#endif
}
