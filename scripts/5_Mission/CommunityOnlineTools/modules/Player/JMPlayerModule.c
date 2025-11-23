modded class JMPlayerModule
{
	override private void Exec_Heal(array<string> guids, PlayerIdentity ident, JMPlayerInstance instance = NULL)
	{
		super.Exec_Heal(guids, ident, instance);

		array<JMPlayerInstance> players = GetPermissionsManager().GetPlayers(guids);

		for (int i = 0; i < players.Count(); i++)
		{
			PlayerBase player = PlayerBase.Cast(players[i].PlayerObject);
			if (!player)
				continue;

			bool sleepDefine = false;
			#ifdef ZENSLEEP
			sleepDefine = true;
			#endif

			ZenFunctions.DebugMessage("HEAL! sleepDefine=" + sleepDefine);

			#ifdef ZENSLEEP
			ZenFunctions.DebugMessage("ZENSLEEP!");
			player.GetStatZenFatigue().Set(player.GetStatZenFatigue().GetMax());
			#endif
		}
	}
}