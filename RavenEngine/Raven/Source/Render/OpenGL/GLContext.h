#pragma once





namespace Raven
{

	// Forward Declare
	struct GLFWwindow;



	//
	struct GLSurface
	{
		int majorVersion;
		int minorVersion;
		bool coreProfile;

		int redBits;
		int greenBits;
		int blueBits;
		int alphaBits;

		bool doubleBuffer;
		int samples;
	};




	//
	//
	//
	class GLContext
	{
	public:
		//
		GLContext();

		//
		void SetGLFWContext(GLFWwindow* context);

		//
		static GLSurface GetSurface();


	private:
		//
		GLFWwindow* glfw_context;
	};


}
