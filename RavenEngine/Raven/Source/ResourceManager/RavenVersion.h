#pragma once




// The Current Raven Files Version.
#define RAVEN_VERSION 10002



// The First version in the engine.
#define RAVEN_FIRST_VERSION 10000


// Global variables are set by archive or loader for saving and load.
struct RavenVersionGlobals
{
	// Version of the scene that is currently being loaded.
	static unsigned int SCENE_ARCHIVE_VERSION;

};




// Versions:
//    - every time you increase the version please add comments this will help us detect the time
//			a file has been corrupted.
//
//    - FORAMT: NEW VERSION - DATE - WHAT THE NEW VERSION ADD.
//



// 10000 - 28/04/2021 - Initial Version.
// 10001 - 06/05/2021 - Start saving referenced material in Primitve Components.
// 10002 - 16/05/2021 - Cast Shadow boolean in in Primitve Components and Scene Global Settings.

