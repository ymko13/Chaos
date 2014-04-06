--[[ TODO:
---- 1. Test, show on Tuesday
]]--

ValueToModel = {
  ["0"] = { ["Class"] = "VisBaseEntity_cl", ["Path"] = "Meshes/Floor.vmesh" },
  ["1"] = { ["Class"] = "VisBaseEntity_cl", ["Path"] = "Meshes/Friendly/Grass.vmesh" },
  ["2"] = { ["Class"] = "VisBaseEntity_cl", ["Path"] = "Meshes/Enemy/Base.vmesh" },
  ["3"] = { ["Class"] = "VisBaseEntity_cl", ["Path"] = "Meshes/Floor.vmesh" }
}

Texture = {
  Neutral = 0,
  Friendly = 1,
  Enemy = 2,
  Road = 3
}

Settings = {
  mapSizeMultiplyer = 5, --Multiplies the random factor by this value to determine size of room
  areaRooms = 150, -- how much area contributes to +1 max neutral room
  maxRooms = 8 --Divisor of width\height for getting max room size also used for size of rooms
}

ourMap = {}

function OnBeforeSceneLoaded()
  Console:SetVisible(true)

  local seed = 13--math.mod(os.time(), math.random(45000))
  Console:PrintLine("Seed: " .. seed)
  math.randomseed(seed)
  
  local size = Settings.mapSizeMultiplyer * (4 + math.random(2))
  local width = size + math.random(size)
  local height = size + math.random(size)
  Console:PrintLine("Map Width: " .. width .. " Map Height:" .. height)
  
  BakeMap(width,height)
  Console:PrintLine("Size of the map is [" .. #ourMap.coords .. ", " .. #ourMap.rooms .. ", "..#ourMap.paths.."]")
  for _,cell in pairs(ourMap.coords) do
    if(cell) then
      if(cell.z ~= -1) then
        local cl = ValueToModel[""..cell.z]["Class"]
        local path = ValueToModel[""..cell.z]["Path"]
        --Console:PrintLine("X: " .. cell.x .. " Y: " .. cell.y .. " Texture: " .. cell. z .. " Path: " .. path)
        Game:CreateEntity(Vision.hkvVec3(cell.x * 100.7,cell.y * 100.7,0), cl, path)
        Game:CreateStaticMeshInstance(Vision.hkvVec3(cell.x * 100.7, cell.y * 100.7, 0), path, true)
       end
    end
  end
  
  --Destroy the temporary global stuff from the memory to conserve it
  ValueToModel = nil
  Texture = nil
  Settings = nil
  ourMap = nil
end

function BakeMap(width, height)
  --Generate an empty map
  ourMap = Map:New(width,height) -- x , y

  --Generate rooms for the map
  local allRooms = {}
  --We want to create the player and enemy apart on longer sides of the map
  --So we make 2 different sets of values to work with it
  local playerRoom = Vision.hkvVec3(0,0,0)
  local enemyRoom = Vision.hkvVec3(0,0,0)
  local startPoint = Vision.hkvVec2(0,0)
  local endPoint = Vision.hkvVec2(0,0)
  --Initialise depending which side is longer
  if(width > height) then
    local roomMax = height / Settings.maxRooms
    
    playerRoom.x = math.random(2)               --x Position
    playerRoom.y = math.random(height)          --y Position
    playerRoom.z = math.random(roomMax) + 1     --size 
    
    enemyRoom.x = (width - 2) + math.random(2)  --x Position
    enemyRoom.y = math.random(height)           --y Position
    enemyRoom.z = math.random(roomMax) + 1      --size  
    
    startPoint = Vision.hkvVec2(4, 0)
    endPoint = Vision.hkvVec2(width - 4, height)
  else
    local roomMax = height / Settings.maxRooms
    
    playerRoom.x = math.random(width)           --x Position
    playerRoom.y = math.random(2)               --y Position
    playerRoom.z = math.random(roomMax) + 1     --size 
    
    enemyRoom.x =  math.random(width)           --x Position
    enemyRoom.y = (height - 2) + math.random(2) --y Position
    enemyRoom.z = math.random(roomMax) + 1      --size   
    
    startPoint = Vision.hkvVec2(0, 4)
    endPoint = Vision.hkvVec2(width, height - 4)
  end
  
  local fRoom = Room:New(playerRoom, Texture.Friendly)
  local eRoom = Room:New(enemyRoom, Texture.Enemy)
  
  table.insert(allRooms, fRoom)
  table.insert(allRooms, eRoom)
  
  --Make random rooms
  local emptyRooms = width * height / Settings.areaRooms
  for i=0,emptyRooms do
    local rr = Room.MakeRandom(startPoint, endPoint)
    
    --Check if room doesn't intersect(1 block difference) with other rooms
    local intersects = false
    for _,room in pairs(allRooms) do
      intersects = room:Intersects(rr)
      if(intersects) then
        break
      end
    end
    --If it does then try to make a new random room
    if(intersects) then
        i = i - 1
    elseif(not intersects) then --If it doesn't intersect then add it
      ourMap:AddRoom(rr)
      table.insert(allRooms, rr)  
    end
  end
  ourMap:AddRoom(fRoom)
  ourMap:AddRoom(eRoom)

  --! TODO !
  --Generate paths between the rooms
  local room1 = eRoom
  local room2 = nil
  
  for i=0,#allRooms do
    local distanceTo = nil
    for _,room in pairs(allRooms) do
      if(room ~= room1 and room.roadsTo == 0) then 
        local var = {}
        var["room"] = room
        var["distance"] = distanceTo or 9999
        var = room1:ifCloser(var)
        if(var) then
          room2 = var.room
          distanceTo = var.distance
        end
      end
    end
    ourMap:AddPathBetweenRooms(room1, room2)
    room1 = room2
    room2 = nil
  end
  --Bake the map with the corresponding rooms and textures
  --Map is prepared for population on the screen at this point
  ourMap:BakeTogether(width,height)

end

--[[MAP]]--
Map = {}
function Map:New(sizeX, sizeY)
  local map = {}
  setmetatable(map, self)
  self.__index = self
  map.rooms = {}
  map.paths = {}
  math.random()
  map.coords = {}
  local c = map.coords
  for x=0,sizeX do
    for y=0,sizeY do
      table.insert(c, Vision.hkvVec3(x,y,-1))
    end
  end
  
  return map
end

function Map:AddPathBetweenRooms(room1, room2)
  if(room1 and room2) then
    local r1Cent = Vision.hkvVec2(room1.data.x,room1.data.y)
    local r2Cent = Vision.hkvVec2(room2.data.x,room2.data.y)
    local vPath = 0
    local hPath = 0hel
    if(math.random(2) == 1) then
      vPath = Path.Vertical(r1Cent.y, r2Cent.y, r1Cent.x)
      hPath = Path.Horizontal(r1Cent.x, r2Cent.x, r2Cent.y)
    else
      hPath = Path.Vertical(r1Cent.y, r2Cent.y, r1Cent.x)
      vPath = Path.Horizontal(r1Cent.x, r2Cent.x, r2Cent.y)
    end
    
    if(vPath ~= 0 and hPath ~= 0) then
      self:AddPath(vPath)
      self:AddPath(hPath)
      room1.roadsTo = room1.roadsTo + 1
      room2.roadsTo = room2.roadsTo + 1
    end
    
  end
end

function Map:BakeTogether(maxX,maxY)
  local minX = maxX > maxY and 0 or 3
  local minY = maxX < maxY and 0 or 3
  
  for i,coord in pairs(self.coords) do  
    for _, path in pairs(self.paths) do --Bake paths
        local minX = math.min(path.sPoint.x, path.ePoint.x)
        local maxX = math.max(path.sPoint.x, path.ePoint.x)
        local minY = math.min(path.sPoint.y, path.ePoint.y)
        local maxY = math.max(path.sPoint.y, path.ePoint.y)
        local checkX = (coord.x >= minX and coord.x <= maxX)
        local checkY = (coord.y >= minY and coord.y <= maxY)
        local t = path.texture
        
        if( checkX and checkY ) then
            coord.z = t
        end
    end
    
    for _, room in pairs(self.rooms) do --Bake rooms
      if(room) then
        local c = Vision.hkvVec2(room.data.x, room.data.y)
        local s = room.data.z
        local t = room.texture
        
        local z = c.x * c.y
        --Check if coord is bigger or less
        local checkX = (coord.x >= c.x - s and coord.x <= c.x + s)
        local checkY = (coord.y >= c.y - s and coord.y <= c.y + s)
        
        if( checkX and checkY ) then
            coord.z = t
        end
        
      end
    end    
  end
end

function Map:AddRoom(room)
  --Console:PrintLine("Room added " .. room.data)
  table.insert(self.rooms, room)
end

function Map:AddPath(path)
  --Console:PrintLine("Path added " .. room.data)
  table.insert(self.paths, path)
end

--[[PATH]]--
Path = {}
function Path:New(startPoint, endPoint,texture)
  local new = {}
  setmetatable(new, self)
  self.__index = self
  
  new.sPoint = startPoint or Vision.hkvVec2(0,0)
  new.ePoint = endPoint or Vision.hkvVec2(0,0)
  new.texture = texture or Texture.Road
  return new
end

function Path.Vertical(startY,endY, X, texture)
  local start = Vision.hkvVec2(X,startY)
  local finish = Vision.hkvVec2(X,endY)
  return Path:New(start,finish,texture)
end

function Path.Horizontal(startX, endX, Y, texture)
  local start = Vision.hkvVec2(startX,Y)
  local finish = Vision.hkvVec2(endX,Y)
  return Path:New(start,finish,texture)
end

--[[ROOM]]--
Room = {}
function Room:New(vec3CenterAndSize, texture)
  local new = {}
  setmetatable(new, self)
  self.__index = self
  
  new.data = vec3CenterAndSize or Vision.hkvVec3(0,0,0)
  new.texture = texture or Texture.Neutral
  new.roadsTo = 0 --Every room should have a total of 2 roads leading to them except
                  --the roads that lead to the player room and enemy room, there can be 1 or 2
  return new
end

--If distance is less than the distance specified returns the var we passed with changed distance
--Else returns nil
function Room:ifCloser( var )
   local room = var.room
   local distanceTo = var.distance
   
   local rc1 = Vision.hkvVec2(self.data.x, self.data.y)
   local rc2 = Vision.hkvVec2(room.data.x, room.data.y)
   local distance = rc1:getDistanceToSquared(rc2)
   if(distance < distanceTo) then
     var.distance = distance
     return var
   end
   return nil
end

function Room:Intersects(room)
  local check1 = self.data.x - self.data.z - 1 <= room.data.x + room.data.z
  local check2 = self.data.x + self.data.z + 1 >= room.data.x - room.data.z
  local check3 = self.data.y - self.data.z - 1 <= room.data.y + room.data.z
  local check4 = self.data.y + self.data.z + 1 >= room.data.y - room.data.z
  return check1 and check2 and check3 and check4
end

function Room.MakeRandom(sPoint, ePoint)
  local p = { x = ePoint.x - sPoint.x, y = ePoint.y - sPoint.y }
  local x = math.random(p.x) + sPoint.x
  local y = math.random(p.y) + sPoint.y
  local z = math.random(math.max(math.min(p.x,p.y) / Settings.maxRooms,1))
  local data = Vision.hkvVec3(x,y,math.floor(z))
  return Room:New(data, Texture.Neutral)
end
