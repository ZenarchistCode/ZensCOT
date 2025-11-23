class CfgPatches
{
	class ZenCOT
	{
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"ZenSleep"
		};
	};
};

class CfgMods
{
	class ZenCOT
	{
		dir = "ZenCOT";
		name = "Zen's COT Additions";
		author = "Zenarchist";
		credits = "";
		version = "1.0";
		type = "mod";
		dependencies[] = 
		{ 
			"Game", 
			"World", 
			"Mission" 
		};
		class defs
		{
			class gameScriptModule
			{
				files[] = 
				{ 
					"ZenCOT/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				files[] = 
				{ 
					"ZenCOT/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				files[] = 
				{ 
					"ZenCOT/scripts/5_mission"
				};
			};
		};
	};
};