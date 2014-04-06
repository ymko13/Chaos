-- new script file
function OnInitProperties(self)
  Debug:PrintLine("New Cell: "..self:GetProperty("m_pos.x").." "....self:GetProperty("m_pos.y"))
end