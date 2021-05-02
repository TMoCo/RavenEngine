
dofile("scripts/libs/Class.lua")

local ScriptBehavior = class("ScriptBehavior")

function ScriptBehavior:GetAnimator()
	return self.entity:GetAnimator()
end

function ScriptBehavior:GetRigidBody()
	return self.entity:GetRigidBody()
end


function ScriptBehavior:GetNameComponent()
	return self.entity:GetNameComponent()
end

function ScriptBehavior:GetActiveComponent()
	return self.entity:GetActiveComponent()
end

function ScriptBehavior:GetLight()
	return self.entity:GetLight()
end

function ScriptBehavior:GetTransform()
	return self.entity:GetTransform()
end

function ScriptBehavior:GetTransform()
	return self.entity:GetTransform()
end


return ScriptBehavior