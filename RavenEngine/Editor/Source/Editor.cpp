#include "EntryPoint.h"
#include "Editor.h"

#include "glm/glm.hpp"

void Editor::Initialize()
{
	Engine::Initialize();
	LOGE("this is a message,{0},{1},{2}", 1, 2, 3);
	LOGC("this is a message,{0},{1},{2}", 1, 2, 3);
	LOGW("this is a message,{0},{1},{2}", 1, 2, 3);
	LOGI("this is a message,{0},{1},{2}", 1, 2, 3);
	LOGV("this is a message,{0},{1},{2}", 1, 2, 3);

	glm::vec4 vec4;

}



Engine * CreateEngine() {
	return new Editor();
}