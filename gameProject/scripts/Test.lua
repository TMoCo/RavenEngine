

local Test = {};

Inventory = require("Inventory")


function Test:OnInit()

	self.x = 10
	self.y = 20

	self.pos = glm.vec2(101,10)
	self.pos2 = glm.vec2(102,10)
	
	self.gameEntity = Entity()

end


function Test:OnUpdate(dt)
	self.x = self.x + 1 * dt;
	
	if Input.IsKeyPressed(KeyCode.A) then
		LOGV("A is pressed");
	end
	
	
	if Input.IsKeyPressed(KeyCode.W) then
		LOGV("W is pressed");
	end
	
	if Input.IsKeyPressed(KeyCode.S) then
		LOGV("S is pressed");
	end
	
	if Input.IsKeyPressed(KeyCode.D) then
		LOGV("D is pressed");
	end

end

return Test;