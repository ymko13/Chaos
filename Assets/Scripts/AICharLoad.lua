-- new script file
currentTask = "";
function OnCreate(self)
  Debug:PrintLine("Character Created")
end

function OnBeforeSceneLoaded(self)
end

function OnAfterSceneLoaded(self)
end

function OnUpdateSceneBegin(self)
  Debug:PrintLine("Health: "..self:GetProperty("m_stats.m_health"))
end

function OnInitProperties(self)
  self:SetProperty("m_stats.m_health_regen", true)
  self:SetProperty("m_stats.m_health", "100")
  self:SetProperty("m_stats.m_health_max", "1000")
  self:SetProperty("m_stats.m_health_regen_rate", "1")
  self:SetProperty("m_stats.m_movement_speed_normal", "300")
  
  self:SetProperty("m_destination.x", "0")
  self:SetProperty("m_destination.y", "0")
  self:SetProperty("m_destination.z", "0")
end
