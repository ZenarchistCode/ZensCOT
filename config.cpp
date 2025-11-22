/*
	(c) 2025 | ZenCOT | Zenarchist
*/

class CfgPatches
{
	class ZenCOT
	{
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"JM_COT_Scripts"
		};
	};
};

class CfgMods
{
	class ZenCOT
	{
		author = "Zenarchist";
		type = "mod";
		inputs = "ZenCOT/data/inputs.xml";
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = 
				{ 
					"ZenCOT/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] = 
				{ 
					"ZenCOT/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] = 
				{ 
					"ZenCOT/scripts/5_Mission"
				};
			};
		};
	};
};