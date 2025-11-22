modded class JMObjectSpawnerForm
{
	UIActionButton m_ZenShowOnMapButton;
	
	override void OnInit()
	{
		super.OnInit();
		
		// Panel that contains the search box + "X" button
		Widget actions = m_SearchBox.GetLayoutRoot().GetParent();

		if (!m_ZenShowOnMapButton && actions)
		{
			m_ZenShowOnMapButton = UIActionManager.CreateButton(actions, "Map", this, "OnClick_ZenShowOnMap");
			m_ZenShowOnMapButton.SetWidth(0.10);
			m_ZenShowOnMapButton.SetPosition(0.55); // directly before the "X" at 0.65
		}
	}
	
	void OnClick_ZenShowOnMap(UIEvent eid, UIActionBase action)
	{
		if (eid != UIEvent.CLICK) 
			return;

		string selected;
		if (m_ClassList && m_ClassList.GetSelectedRow() != -1)
			m_ClassList.GetItemText(m_ClassList.GetSelectedRow(), 0, selected);
		else 
		if (m_SearchBox)
			selected = m_SearchBox.GetText();

		if (selected != "" && m_Module)
		{
			GetRPCManager().SendRPC("ZenCOT_RPC", "RPC_ShowOnMapRequest", new Param1<string>(selected), true);
		}
	}
}