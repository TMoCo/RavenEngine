
dofile("scripts/libs/Class.lua")
local Inventory = class("Inventory")

function Inventory:ctor()

end

function Inventory:Print()
    LOGV("Print from lua")
end

return Inventory