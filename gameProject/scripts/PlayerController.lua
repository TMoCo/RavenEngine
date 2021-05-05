
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
	sprintAccel = 10
	jumpPower = 500
	mouseSens = 0.2

	---------------------------
	-- Keyboard Input Handling
	---------------------------

	if Input.IsKeyHeld(KeyCode.LeftShift) then
		moveAccel = sprintAccel
	else
		moveAccel = walkAccel
	end

	if Input.IsKeyHeld(KeyCode.A) then
		--LOGV("A is pressed")

		f = self:GetRigidBody():GetRightVector()
		f1 = glm.vec3(-moveAccel*f.x, 0, -moveAccel*f.z)

		totalLinear = totalLinear + f1
	end
	if Input.IsKeyHeld(KeyCode.D) then
		--LOGV("D is pressed")

		f = self:GetRigidBody():GetRightVector()
		f1 = glm.vec3(moveAccel*f.x, 0, moveAccel*f.z)

		totalLinear = totalLinear + f1
	end
	
	
	if Input.IsKeyHeld(KeyCode.W) then
		--LOGV("W is pressed")

		f = self:GetRigidBody():GetForwardVector()
		f1 = glm.vec3(-moveAccel*f.x, 0, -moveAccel*f.z)

		totalLinear = totalLinear + f1
	end
	
	if Input.IsKeyHeld(KeyCode.S) then
		--LOGV("S is pressed")

		f = self:GetRigidBody():GetForwardVector()
		f1 = glm.vec3(moveAccel*f.x, 0, moveAccel*f.z)

		totalLinear = totalLinear + f1
	end

	if Input.IsKeyPressed(KeyCode.Space) then
		f1 = glm.vec3(0,jumpPower, 0)

		totalLinear = totalLinear + f1
	end

	-- Apply all of the linear movement forces from the current frame
	self:GetRigidBody():ApplyForce(totalLinear)

	---------------------------
	-- Mouse Input Handling
	---------------------------

	if not self.lastMPos then
		self.lastMPos = Input.GetMousePosition()
	end

	mDiff = Input.GetMousePosition() - self.lastMPos

	-- Horizontal mouse movement
	if (mDiff.x ~= 0) then
		
		turnDelta = -(mDiff.x * mouseSens)
		self:GetRigidBody():RotateBody(glm.vec3(0,1,0), turnDelta)

	end

	-- Vertical mouse movement
	if (mDiff.y ~= 0) then
		
		--turnDelta = -(mDiff.x * mouseSens)
		--self:GetRigidBody():RotateBody(glm.vec3(0,1,0), turnDelta)

		--self.lastMPos = Input.GetMousePosition()
	end
	
	self.lastMPos = Input.GetMousePosition()
end

return PlayerController;