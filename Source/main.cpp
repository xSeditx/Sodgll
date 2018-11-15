#include"WindowGLSDL.h"
#include"Renderer.h"
void Update()
{
//	f_Trace("Update")
}

void Render()
{
//	f_Trace("Render");
}
class MyApp : public Application
{

};

void Keydown(Uint16 Keycode, Uint16 ScanCode, Uint16 Modifier, Uint8 rep)
{
	switch (Keycode)
	{
	case 79:
		Window::SCREEN->World->Camera.MoveRight(.51);
		break;
	case 80:
		Window::SCREEN->World->Camera.MoveLeft(.51);
		break;
	case 81:
		Window::SCREEN->World->Camera.MoveForward(.51);
		break;
	case 82:
		Window::SCREEN->World->Camera.MoveBack(.51);
		break;
	}
//	Print(Keycode <<" : "<< ScanCode);
}

void  MouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle)
{
		Window::SCREEN->World->Camera.Rotate(-relX, relY);
}
#include<Chrono>
void main()
{


	Window MainWin(10, 30, 640, 480, "SDL OpenGL TestWindow");
	MainWin.Callbacks.SetOnKeyDown(Keydown);
	MainWin.Callbacks.SetOnMouseMove(MouseMove);
	MainWin.SetUpdate(Update);
	MainWin.SetRender(Render);
	MainWin.Framerate.Set(2500);

	//--------Gathering information about OpenGL state and Display it -----------------------------------------------

	const GLubyte *extensions = glGetString(GL_EXTENSIONS);
	Print(extensions);
	Print("OpenGL Version: "; Print(glGetString(GL_VERSION)));
	Print("Renderer: "; Print(glGetString(GL_RENDERER)));
	Print("Current Context: "; Print(wglGetCurrentContext()));

	//-------------------------------------------------------------------------------------------------------------


#if	_OPENGL_FIXED_FUNCTION
#else
	Material *Surface = new Material();
	Surface->Renderer = new Shader("Resources\\Basic.vert", "Resources\\Basic.frag");
#endif

	Block Cube(Vec3(0, 0, 10), 1);
	Block Cube1(Vec3(0, 0, -10), 1);
	Block Cube2(Vec3(0, 10, 0), 1);
	Block Cube3(Vec3(0, -10, 0), 1);
	Block Cube4(Vec3( 10, 0, 0), 1);
	Block Cube5(Vec3(-10, 0, 0), 1);	Block center(Vec3(0, 0, 0), 2);
	Sphere Ball(Vec3(-5, -5, 0), 1, 15);
	MainWin.World = new Scene();
	MainWin.World->Camera = Viewport(Vec3(0, 0, 0), Vec3(0, 0, 0));
	Viewport::Camera = &MainWin.World->Camera;
	MainWin.World->Groups[0].Add(Cube);
	MainWin.World->Groups[0].Add(Cube1);
	MainWin.World->Groups[0].Add(Cube2);
	MainWin.World->Groups[0].Add(Cube3);
	MainWin.World->Groups[0].Add(Cube4);
	MainWin.World->Groups[0].Add(Cube5);
	MainWin.World->Groups[0].Add(center);
	MainWin.World->Groups[0].Add(Ball);

	for_loop(Index, 400)
	{
		Sphere Ball(Vec3(rand() % 300 - 150, rand() % 300 - 150, rand() % 300 - 150), 1, 20);
		MainWin.World->Groups[0].Add(Ball);
	}

	while (MainWin.EventLoop())
	{
		{
			BenchMark Performance;

			MainWin.CLS();
			MainWin.World->Camera.Update();
			MainWin.World->Render();
			MainWin.Sync();
		}

		
//		Print(MainWin.Mouse.Position.x << " : " << MainWin.Mouse.Position.y);
//		Print(MainWin.Mouse.Velocity.x << " : " << MainWin.Mouse.Velocity.y);
// 		Print(MainWin.Framerate.Get());
	}
}






 
//  HWND ConsoleHandle = GetConsoleWindow();
//  LPMSG Msg = 0;
//  if (GetMessage(Msg, ConsoleHandle, 0, 0) != 0)
//  {
//  	if (Msg->message == WM_QUIT)
//  	{
//  
//  		Print("WindowExit");
//  	}
//  }