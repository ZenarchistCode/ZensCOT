modded class JMObjectSpawnerModule
{
	void JMObjectSpawnerModule()
	{
		GetRPCManager().AddRPC("ZenCOT_RPC", "RPC_ShowOnMapRequest", this, SingeplayerExecutionType.Both);
		GetRPCManager().AddRPC("ZenCOT_RPC", "RPC_ShowOnMapSend", this, SingeplayerExecutionType.Both);
	}

	override void RegisterKeyMouseBindings() 
	{
		super.RegisterKeyMouseBindings();

		Bind(new JMModuleBinding("ZenCopyPositionOnCursor", "UAZenCOT_CopyPosition", true));
	}

	// Client -> server receives
	void RPC_ShowOnMapRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1<string> data;
		if (!ctx.Read(data))
		{
			Error("Failed to read data");
			return;
		}

		string className = data.param1;

		if (className == "")
			return;
		
		array<EntityAI> found = new array<EntityAI>;
		ZenEntityTracker.GetByClassname(className, found);
		
		if (!found || found.Count() == 0)
		{
			COTCreateNotification(sender, new StringLocaliser("No items with classname " + className + " were found."));
			return;
		}

		array<string> names = new array<string>;
		array<vector> positions = new array<vector>;
		
		foreach (EntityAI entity : found)
		{
			if (!entity)
				continue;
			
			string lifetimeText = "";
			float lifetime = (entity.GetLifetime() / 60);
			
			if (lifetime > 60)
			{
				lifetime = lifetime / 60;
				lifetimeText = " " + Math.Round(lifetime) + "h";
			}
			else 
			{
				lifetimeText = " " + Math.Round(lifetime) + "m";
			}

			names.Insert(ZenEntityTracker.ZenCOT_GetObjectName(entity) + lifetimeText /*" id=" + entity.GetID()*/);
			positions.Insert(entity.GetPosition());
		}
		
		GetRPCManager().SendRPC("ZenCOT_RPC", "RPC_ShowOnMapSend", new Param2<ref array<string>, ref array<vector>>(names, positions), true, null);
	}
	
	// Server -> client receives
	void RPC_ShowOnMapSend(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if (!GetPermissionsManager().HasPermission("Admin.Map.View"))
			return;
		
		Param2<ref array<string>, ref array<vector>> data;
		if (!ctx.Read(data))
		{
			Error("Failed to read data");
			return;
		}
		
		array<string> itemNames = data.param1;
		array<vector> itemPositions = data.param2;
	
		Print("[ZenCOT] Received marker count=" + itemNames.Count() + "/" + itemPositions.Count());
		
		if (itemNames.Count() != itemPositions.Count())
		{
			Error("Array mismatch!");
			return;
		}
	
		array<ref JMMapTempMark> marks = new array<ref JMMapTempMark>;
		for (int i = 0; i < itemNames.Count(); i++)
		{
			vector pos = itemPositions.Get(i); 
			string label = itemNames.Get(i);
	
			JMMapTempMark m = new JMMapTempMark();
			m.m_Pos   = pos;
			m.m_Label = label;
			marks.Insert(m);
		}
	
		JMMapModule mapMod;
		if (Class.CastTo(mapMod, GetModuleManager().GetModule(JMMapModule)))
		{
			mapMod.SetTempItemMarks(marks, "Markers");
			mapMod.Show();
		}
	}

	// Copy object info @ cursor to clipboard
	void ZenCopyPositionOnCursor(UAInput input)
	{
		if (!g_Game || !input.LocalPress())
			return;

		if (GetGame().GetUIManager().GetMenu())
			return;

		if (!GetPermissionsManager().HasPermission("COT"))
			return;

		if (!GetCommunityOnlineToolsBase().IsActive())
		{
			//COTCreateLocalAdminNotification(new StringLocaliser("STR_COT_NOTIFICATION_WARNING_TOGGLED_OFF"));
			return;
		}

		string notification = "";
		string pos;
		string ori;
		
		Object objTarget = ZenCOT_GetObjectAtCrosshair();
		if (!objTarget || g_Game.IsLeftCtrlDown())
			objTarget = g_Game.GetPlayer();

		if (!objTarget)
			return;

		string objType = ZenEntityTracker.ZenCOT_GetObjectName(objTarget);
		pos = objTarget.GetPosition().ToString(false);
		ori = objTarget.GetOrientation().ToString(false);
		notification = "Copied to clipboard position of object\n\n" + objTarget.GetType() + " @\n\n" + pos + "\n\nOrientation: " + ori;

		if (!g_Game.ZenCOT_IsLeftAltDown())
		{
			array<string> split = new array<string>;
			
			pos.Split(" ", split);
			if (split.Count() == 3)
			{
				pos = split[0] + ", " + split[1] + ", " + split[2];
			}

			split.Clear();

			ori.Split(" ", split);
			if (split.Count() == 3)
			{
				ori = split[0] + ", " + split[1] + ", " + split[2];
			}
		}

		g_Game.CopyToClipboard(objTarget.GetType() + "\nPos: " + pos + "\nOri: " + ori + "\n\n");
		COTCreateLocalAdminNotification(new StringLocaliser(notification), "set:ccgui_enforce image:HudBuild", 10);
	}

	// Alternative to COT's get object at which ignores Land_ and static objects.
	Object ZenCOT_GetObjectAtCrosshair(float distance = 1000.0)
	{
	    vector rayStart = g_Game.GetCurrentCameraPosition();
	    vector rayEnd = rayStart + g_Game.GetCurrentCameraDirection() * distance;
	    auto objs = GetObjectsAt(rayStart, rayEnd, g_Game.GetPlayer(), 0, null);

	    if(objs != null && objs.Count() > 0)
	        return objs.Get(0);

	    return NULL;
	}

	// Add ability to hold CTRL down to delete far away targets.
	// Only difference = 1000.0 GetObjectAtCursor distance override (code last copied from COT on 22nd November 2025)
	override void DeleteCursor(UAInput input)
	{
		if (!g_Game)
			return;

		if (!g_Game.IsLeftCtrlDown())
		{
			super.DeleteCursor(input);
			return;
		}

		if (!input.LocalPress())
			return;

		if (GetGame().GetUIManager().GetMenu())
			return;

		if (!GetPermissionsManager().HasPermission("Entity.Delete"))
			return;

		if (!GetCommunityOnlineToolsBase().IsActive())
		{
			COTCreateLocalAdminNotification(new StringLocaliser("STR_COT_NOTIFICATION_WARNING_TOGGLED_OFF"));
			return;
		}

		Object obj = GetObjectAtCursor(true, 1000.0);

		if (!obj)
			return;

		m_AutoShow = !IsVisible();
		if (m_AutoShow)
			Show();

		JMObjectSpawnerForm form;
		if (Class.CastTo(form, GetForm()))
			form.DeleteCursor(obj);
	}
}