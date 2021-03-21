#pragma once






namespace Raven
{
	// Render Surface Info.
	struct RenderSurface
	{
		// OpenGL Major Version.
		int majorVersion;

		// OpenGL Minor Version;
		int minorVersion;

		// Requrie ore Profile.
		bool coreProfile;

		// Number of red bits.
		int redBits;

		// Number of green bits.
		int greenBits;

		// Number of blue bits.
		int blueBits;

		// Number of alpha bits.
		int alphaBits;

		// Require Double Buffer.
		bool doubleBuffer;

		// Number of samples for Multisampling.
		int samples;
	};
}


