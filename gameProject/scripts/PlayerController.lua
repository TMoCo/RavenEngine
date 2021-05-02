
dofile("scripts/libs/Class.lua")

-- declare BaseClass
local ScriptBehavior = require("ScriptBehavior")

-- extends from ScriptBehavior
local PlayerController = class("PlayerController",ScriptBehavior)

--constructor
function PlayerController:ctor()
	LOGV("PlayerController:ctor")
end

function PlayerController:OnInit()
-- self.entity is the Entity that current component bind to 
-- c++ class is Entity
end


function PlayerController:OnUpdate(dt)
	totalLinear = glm.vec3(0,0,0)

	walkAccel = 5
	jumpPower = 500
	if Input.IsKeyHeld(KeyCode.LeftShift) then
		walkAccel = 10
	end

	if Input.IsKeyHeld(KeyCode.A) then
		--LOGV("A is pressed")

		f = self:GetRigidBody():GetRightVector()
		f = f:scale(-1 * walkAccel)
		totalLinear = totalLinear + f
	end
	if Input.IsKeyHeld(KeyCode.D) then
		--LOGV("D is pressed")

		f = self:GetRigidBody():GetRightVector()
		f = f:scale(walkAccel)
		totalLinear = totalLinear + f
	end
	
	
	if Input.IsKeyHeld(KeyCode.W) then
		--LOGV("W is pressed")

		f = self:GetRigidBody():GetForwardVector()
		f = f:scale(-1 * walkAccel)
		totalLinear = totalLinear + f
	end
	
	if Input.IsKeyHeld(KeyCode.S) then
		--LOGV("S is pressed")

		f = self:GetRigidBody():GetForwardVector()
		f = f:scale(walkAccel)
		totalLinear = totalLinear + f
	end

	if Input.IsKeyPressed(KeyCode.Space) then
		f = self:GetRigidBody():GetUpVector()
		f = f:scale(jumpPower)
		totalLinear = totalLinear + f
	end

	if not self.lastMPos then
		self.lastMPos = Input.GetMousePosition()
	end

	-- If the mouse has moved horizontally last frame
	mDiff = Input.GetMousePosition() - self.lastMPos
	if (mDiff.x ~= 0) then
		
		f = glm.vec3(0,-1 * mDiff.x,0)
		self:GetRigidBody():ApplyTorque(f)

		self.lastMPos = Input.GetMousePosition()
	end

	self:GetRigidBody():ApplyForce(totalLinear)

end

return PlayerController;