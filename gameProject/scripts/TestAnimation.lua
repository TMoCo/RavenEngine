local TestAnimation = {};

function TestAnimation:OnInit()

end


function TestAnimation:OnUpdate(dt)

	
	if Input.IsKeyPressed(KeyCode.A) then
		LOGV("A is pressed");
		
		self.parent:GetAnimator():SetValue("isSetting",true);
		self.parent:GetAnimator():SetValue("isWalking",false);
		
	end
	
	
	if Input.IsKeyPressed(KeyCode.W) then
		LOGV("W is pressed");
		
		self.parent:GetAnimator():SetValue("isSetting",false);
		self.parent:GetAnimator():SetValue("isWalking",true);

	end
	
	if Input.IsKeyPressed(KeyCode.S) then
		self.parent:GetAnimator():SetValue("isSetting",false);
		self.parent:GetAnimator():SetValue("isWalking",false);
	end
	
	if Input.IsKeyPressed(KeyCode.D) then
		LOGV("D is pressed");
	end

end

return TestAnimation;