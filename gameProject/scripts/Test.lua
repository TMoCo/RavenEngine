
LOGE("this message comes from lua");
LOGV("this message comes from lua");
LOGI("this message comes from lua");
LOGC("this message comes from lua");


Test = {};-- define a class 


function Test:OnInit()

end


function Test:OnTest()
	LOGC("this message comes Lua test function");
end


