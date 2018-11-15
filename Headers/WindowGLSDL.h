#pragma once
#define SDL_MAIN_HANDLED
#define _SDL_

#define FEATURES_JOYSTICK          OFF
#define FEATURES_GAMECONTROLLER    OFF

#define _OPENGL_FIXED_FUNCTION     ON //   OFF//

#include"SDL.h"
#include"SDL_syswm.h"

#include <iostream>
#include <vector>
#include <functional>
 
#ifdef _WIN32
#    define GLEW_STATIC
#    pragma comment(lib, "user32.lib")
#    pragma comment(lib, "gdi32.lib")
#    pragma comment(lib, "opengl32.lib")
#    pragma comment(lib, "gdiplus.lib")
#    include <GL/glew.h>
#    include <GL/gl.h>
#    include <GL/glu.h>
#    include"windows.h"
#endif

//#define _ITERATOR_DEBUG_LEVEL 0

#pragma warning(disable: 4244)	//  conversion from 'float' to 'int', possible loss of data
#pragma warning(disable: 4005)	
#pragma warning(disable: 4305)	

#pragma warning(disable: 4996)	// 'fopen': This function or variable may be unsafe
#pragma warning(disable: 4099)	
#pragma warning(disable: 4715)	
#pragma warning(disable: 4800)	
#pragma warning(disable: 4018)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         GLM COMPATIBILITY                                                                                                                       
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtc/type_ptr.hpp>
#include "gtc/matrix_transform.hpp"

typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;
typedef glm::mat4 Matrix;
typedef glm::vec3 RGBf;
typedef glm::vec4 RGBAf;




// Forward Declarations
class Callback;
class Shader;




#define TRUE   1
#define FALSE  0
#define ON     1
#define OFF    0

#define RADIANS(angle)           ((angle) * .0174532925199444)
#define GL_Color(c)              (c * (.0039215686627451))
#define Print(x)                 std::cout << x << std::endl
#define for_loop(iter, count)    for(int iter = 0;iter < (count); iter++)
#if _DEBUG
#    define f_Trace(x)    Print(x);
#    define _GL(x)           GLClearError();\
                                        (x);\
                                  _TRACE(x);\
                   ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#else
#    define f_Trace(x)
#    define _GL(x)  x
#endif

#define ASSERT(x) if(!(x)){  exit(EXIT_FAILURE);}

#ifdef TRACE
#    define  _TRACE(x)  std::cout << #x << std::endl
#else
#    define _TRACE(x)
#endif

/*===============================================================================================================================================================================================
//
//
//      APPLICATION SYSTEM MODULE
// INCOMPLETE
//
=================================================================================================================================================================================================*/
class Scene; class Viewport;
class Application
{
public:
	Application();
	~Application();
	virtual bool OnUserCreate();
	virtual bool OnUserUpdate();
	virtual bool OnUserRender();
	virtual bool OnUserDestroy();
 
	Scene *World;
	Viewport *Camera;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              USER DEFINED CALLBACK SYSTEM
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Callback {
public:
	Callback()
	{

		OnEvent = NULL;
		OnInputFocus = NULL;
		OnInputBlur = NULL;
		OnKeyDown = NULL;
		OnKeyUp = NULL;
		OnMouseFocus = NULL;
		OnMouseBlur = NULL;
		OnMouseMove = NULL;
		OnMouseWheel = NULL;

		OnLButtonDown = NULL;
		OnLButtonUp = NULL;
		OnRButtonDown = NULL;
		OnRButtonUp = NULL;
		OnMButtonDown = NULL;
		OnMButtonUp = NULL;

#if FEATURES_JOYSTICK
		OnJoyAxis = NULL;
		OnJoyButtonDown = NULL;
		OnJoyButtonUp = NULL;
		OnJoyHat = NULL;
		OnJoyBall = NULL;
#endif
		OnMinimize = NULL;
		OnRestore = NULL;
		OnResize = NULL;
		OnExpose = NULL;
		OnExit = NULL;
		OnUser = NULL;

	};
	~Callback() {};

	void(*OnEvent)                (SDL_Event* Event);

	void(*OnInputFocus)           ();
	void(*OnInputBlur)            ();
	void(*OnKeyDown)              (Uint16 Keycode, Uint16 ScanCode, Uint16 Modifier, Uint8 rep);                                                    //(SDL_Keysym sym, int mod, Uint16 unicode);
	void(*OnKeyUp)                (Uint16 Keycode, Uint16 ScanCode, Uint16 Modifier);                                                               //(SDL_Keysym sym, int mod, Uint16 unicode);
	void(*OnMouseFocus)           ();
	void(*OnMouseBlur)            ();
	void(*OnMouseMove)            (int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);
	void(*OnMouseWheel)           (bool Up, bool Down);

	void(*OnLButtonDown)         (int mX, int mY);
	void(*OnLButtonUp)           (int mX, int mY);
	void(*OnRButtonDown)         (int mX, int mY);
	void(*OnRButtonUp)           (int mX, int mY);
	void(*OnMButtonDown)         (int mX, int mY);
	void(*OnMButtonUp)           (int mX, int mY);

#if FEATURES_JOYSTICK
	void(*OnJoyAxis)              (Uint8 which, Uint8 axis, Sint16 value);
	void(*OnJoyButtonDown)        (Uint8 which, Uint8 button);
	void(*OnJoyButtonUp)          (Uint8 which, Uint8 button);
	void(*OnJoyHat)               (Uint8 which, Uint8 hat, Uint8 value);
	void(*OnJoyBall)              (Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);
#endif

	void(*OnMinimize)             ();
	void(*OnRestore)              ();
	void(*OnResize)               (int w, int h);
	void(*OnExpose)               ();
	void(*OnExit)                 ();
	void(*OnUser)                 (Uint8 type, int code, void* data1, void* data2);

	void SetOnEvent(SDL_Event* Event);

	void SetOnInputFocus();
	void SetOnInputBlur();

	void SetOnKeyDown(void(*f)   (Uint16 Keycode, Uint16 ScanCode, Uint16 Modifier, Uint8 rep)) { OnKeyDown = f; }
	void SetOnKeyUp(void(*f)   (Uint16 Keycode, Uint16 ScanCode, Uint16 Modifier)) { OnKeyUp = f; }

	void SetOnMouseFocus(void(*f)())                  { OnMouseFocus = f; }
	void SetOnMouseBlur(void(*f)())                   { OnMouseBlur = f; }
	void SetOnMouseMove(void(*f)  (int , int , int , int , bool , bool , bool )) { OnMouseMove = f; }
	void SetOnMouseWheel(void(*f)        (bool, bool)){ OnMouseWheel = f; };

	void SetOnLButtonDown(void(*f)        (int, int)) { OnLButtonDown = f; }
	void SetOnLButtonUp(void(*f)          (int, int)) { OnLButtonUp = f; }
	void SetOnRButtonDown(void(*f)        (int, int)) { OnRButtonDown = f; }
	void SetOnRButtonUp(void(*f)          (int, int)) { OnRButtonUp = f; }
	void SetOnMButtonDown(void(*f)        (int, int)) { OnMButtonDown = f; }
	void SetOnMButtonUp(void(*f)          (int, int)) { OnMButtonUp = f; }

#if FEATURES_JOYSTICK
	void SetOnJoyAxis(Uint8 which, Uint8 axis, Sint16 value);
	void SetOnJoyButtonDown(Uint8 which, Uint8 button);
	void SetOnJoyButtonUp(Uint8 which, Uint8 button);
	void SetOnJoyHat(Uint8 which, Uint8 hat, Uint8 value);
	void SetOnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);
#endif
	void SetOnMinimize(void(*f)())                    { OnMinimize = f; }
	void SetOnRestore(void(*f)())                     { OnRestore = f;}
	void SetOnResize(void(*f)             (int, int)) { OnResize = f; }
	void SetOnExpose(void(*f)                     ()) { OnExpose = f; }
	void SetOnExit(void(*f)                       ()) { OnExit = f;   }

	void SetOnUser(Uint8 type, int code, void* data1, void* data2);
};


/*===============================================================================================================================================================================================
//
//
//      WINDOW MANAGMENT MODULE
//
//
=================================================================================================================================================================================================*/


class Window
{
public:
	Window(int x, int y, int width, int height, const char *title);
	~Window()
	{
		SDL_GL_DeleteContext(OpenGLContext);
	}

	Vec2 Position;
	Vec2 Size;
	const char *Title;

	Callback Callbacks;
 
	Scene *World;

	struct 
	{
		Vec2 Position;
		Vec2 Velocity;
		Vec2 RealPosition;

		int Time;
		int Focus;

		struct
		{
			bool Left, Right, Center, X1, X2;
			short Clicks;
		}Button;
		void Show() { SDL_ShowCursor(SDL_ENABLE); }
		void Hide() { SDL_ShowCursor(SDL_DISABLE); }
    }Mouse;

	struct
	{
		struct keytype
		{
			int  Modifier;
			int  Code;
			int  ScanCode;
			bool State;
		}Key;
		const Uint8 *KeyState;
	}Keyboard;

	struct 
	{
		float Rate;
		void Set(float rate)
		{
			Coefficient = 1000.0 / rate;
		}
		float Get() { return Rate; }
		float PreviousTime = 0.0f;
		float Coefficient = 0.0f;
		float CurrentTime = 0.0f;
		float Tracker = 0.0f;
		unsigned int Counter = 0.0f;

		float LastFrame = 0.0;
	}Framerate;

	int EventLoop();

	void CLS();
	void Sync();

	inline void  SetRefreshRate(float rate) { RefreshRate = rate; }
	inline float GetRefreshRate() { return RefreshRate; }

	void SetUpdate(void(*f)()) { Update = f; }
	void SetRender(void(*f)()) { Render = f; }
	void SetIdle(void(*f)()) { Idle = f; }

	void SetActiveWindow(Window *sdlhwnd)
	{
		SCREEN = sdlhwnd;
	}


private:

#ifdef _WIN32
	HWND WIN_HWND;
#endif

	SDL_Window *SDL_HWND;
	SDL_Renderer *Renderer;
	SDL_GLContext OpenGLContext;

	float AspectRatio;
	Vec2 MouseCorrection;

	bool Alive, Active;

	double RefreshRate;

	void(*Update)();
	void(*Render)();
	void(*Idle)();

	void MessageHandler(SDL_Event msg);
public:
	static Window *SCREEN;
};





/*===============================================================================================================================================================================================
//
//
//       Utility Functions MODULE
//
//
=================================================================================================================================================================================================*/

#include<Chrono>
class BenchMark
{
public:
	BenchMark()
	{
	    PerformanceTimer = std::chrono::high_resolution_clock::now();
		ObjectCounter++;
	}
	~BenchMark()
	{
		AverageTimer += std::chrono::duration_cast  <std::chrono::nanoseconds>  (std::chrono::high_resolution_clock::now() - PerformanceTimer).count();
		if (ObjectCounter > 100)
		{
			Print("LastFrame: "
				<< std::chrono::duration_cast  <std::chrono::nanoseconds>  (std::chrono::high_resolution_clock::now() - PerformanceTimer).count()
				<< "n/s    "
				<< "AvgSpeed: "
				<< (AverageResult) << "n/s  "
				<< "FPS: "
				<< Window::SCREEN->Framerate.Get()
			);

			ObjectCounter = 0;
			AverageResult = AverageTimer / 100;
			AverageTimer = 0;
		}
	}
	std::chrono::time_point<std::chrono::steady_clock>  PerformanceTimer;
	static int ObjectCounter;
	static long long AverageTimer;
	static long long AverageResult;
};
float Squared(float x);
bool GLLogCall(const char *function, const char *file, int line);
void GLCheckError();
void GLClearError();
void SDLCheckError();
void EngineErrorResponse(int error, const int data, char *str);

// https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlseteventfilter.html
// SDL_SetEventFilter((SDL_EventFilter)&filter); // Called on window creation.
//TODO" Make a GLUT like Update, Input Draw etc...etc.. Register Mechanic
//n "update" function, an "input" function, a "draw" function, and an "idle
/// Look into GLUT based GLUI library for GUI Stuff








/*===============================================================================================================================================================================================
//
//
//      FILE AND ASSET MODULE
//
//
=================================================================================================================================================================================================*/




#include<string>
#include<fstream>
#include<vector>
#include<strstream>
#include"WindowGLSDL.h"

#define _ERROR_ 0x13
// tried changing the 3D settings in the display control panel, but there is no noticeable effect.the only immediate solution I can think of is to 
// use the Angle openGL replacement library, which converts opengl commands to directx, or to use a non - Intel video card.
class  FileUtils
{
public:
	static std::string read_file(const char* filepath)
	{
		FILE *file = fopen(filepath, "rt");

		fseek(file, 0, SEEK_END);

		unsigned long length = ftell(file);

		char *data = new char[length + 1];
		memset(data, 0, length + 1);

		fseek(file, 0, SEEK_SET);
		fread(data, 1, length, file);

		fclose(file);

		std::string result(data);
		delete[] data;
		return result;
	}


	char *FileData;

	static void Load_OBJ(const char *filename)
	{ // TODO: GET MY WORKING OBJ LOADER IN HERE AND REMOVE THIS GARBAGE

		std::ifstream file(filename);
		std::vector<Vec3> verts;
		while (!file.eof())
		{
			char line[128];
			file.getline(line, 128);

			std::strstream s;
			s << line;
			char junk;

			if (line[0] == 'v')
			{
				Vec3 v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				int file[4];
				s >> junk >> file[0] >> file[1] >> file[2] >> file[3];

				// meshShip.tris.push_back({ verts[f[0]-1], verts[f[1]-1], verts[f[2]-1], 0.0f, 0.0f, 0.0f, FG_YELLOW });
				//  meshShip.tris.push_back({ verts[f[0]-1], verts[f[2]-1], verts[f[3]-1], 0.0f, 0.0f, 0.0f, FG_YELLOW });
			}
		}
	}
};


template<typename T>
struct AssetManager
{
public:

	T *GetAsset(const char *name)
	{
		for_loop(Index, Count)
		{
			if ((List[Index]->Name != NULL) && strncmp(List[Index]->Name, name, sizeof(name)) == 0)
			{
				return List[Index];
			}
		}
		ErrorHandler(0x01, name);
	}
	T *GetAsset(int id)
	{
		return List[id];
	}

	int  Add(T *object)
	{
		ID = Count++;
		List.push_back(object);
		return ID;
	}
	void Delete(char *name)
	{
		if (GetAsset(name)->ID != nullptr)
		{
			Delete(GetAsset(name)->ID);
		}
		else
		{
			ErrorHandler(0x02, name);
		}
	}
	void Delete(int id)
	{
		delete(List[id]);
		Remove(id);
		//List.erase(List.begin() + id);
		//for(int Itr = id; Itr < List.size(); Itr++)
		//{
		//    List[Itr]->ID--;
		//}
	}
	void Remove(int id)
	{
		List.erase(List.begin() + (id));
		for (int Itr = id; Itr < List.size(); Itr++)
		{
			List[Itr]->ID--;
		}
	}
	void FreeAll()
	{
		for_loop(Index, List.size())
		{
			delete(List[Index]);
		}
	}
	bool IsExistant(char *name)
	{
		for (auto &L : List)
		{
			if (strncmp(List[Index]->Name, name, sizeof(name)) == 0)   //(L.Name == name) return true;
			{
				return true;
			}
		}
		return false;
	}

private:
	std::vector<T *> List;
	int ID;
	int Count;

	void ErrorHandler(int code, const char *name)
	{
		switch (code)
		{
		case 0x01:

			Print("Asset " << name << " Not Registered: Please check the description of ( " << name << " ) ");
			system("Pause");
			exit(_ERROR_);

			break;

		case 0x02:

			Print("Can not delete Asset ( " << name << " ): Asset has already been removed  ");
			system("Pause");
			exit(_ERROR_);

			break;

		case 0x03:
			break;

		case 0x04:
			break;
		}
	}
};
