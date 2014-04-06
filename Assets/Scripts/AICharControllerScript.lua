-- new script file
currentTask = "";
function OnCreate(self)
  Debug:PrintLine("Character Created")
end

function SetState( self,  VState )
  self:SetProperty("m_new_state", VState)
end

function OnBeforeSceneLoaded(self)
end

function OnAfterSceneLoaded(self)
end

function OnUpdateSceneBegin(self)
  Debug:PrintLine("Health: "..self:GetProperty("m_stats.m_health"))  
end

function OnInitProperties(self)
  SetState(self, "sIdling")
  self:SetProperty("m_function_calls[mControllerState::sIdling][0]", "AiIdling_Enter")
end

function AiIdling_Enter(self)
	Debug:PrintLine("IDLING BABY")
end