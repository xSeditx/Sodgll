#pragma once

#include<vector>
#include"WindowGLSDL.h"
/*===============================================================================================================================================================

TODO: Make a Renderer class, this now handles all the buffers and what not, Possibly move the Shader class here instead of Window.

This handles buffers but not all:

Scene->Groups->Mesh->Polygon->Buffers;

Mesh
=====
--Material
_____________
----Shader
----Lighting
----Normals
----Texture
-------- UVBuffer
-------- Image
--Polygon
_____________
---- Indices
---- Vertices
---- Normals
---- Colors 

=================================================================================================================================================================
*/ 

#define _TRANSFORM() glPushMatrix();\
_GL(glTranslatef(Position.x, Position.y, Position.z));\
glRotatef(Rotation.x, 1.0, 0.0, 0.0);\
glRotatef(Rotation.y, 0.0, 1.0, 0.0);\
glRotatef(Rotation.z, 0.0, 0.0, 1.0);


//=========================================== DEBUG SWITCHES ==========================================================
#define _TEXTURE_DEBUG    0
#define      _UV_DEBUG    0
 //====================================================================================================================

#define VERTEX_ATTRIB     0
#define NORMAL_ATTRIB     1
#define TEXTURE_ATTRIB    2
#define COLOR_ATTRIB      3

#define BUFFER_OFFSET(i)   ((char *)NULL + (i))

using namespace std;
using namespace glm;


struct Vertex
{
    Vec3 Position;
    Vec3 Normals;
    Vec2 Uv;
};


///     OpenGL BUFFER ABSTRACTIONS
/*
================================================================================================================================================================
//
//
//      BUFFERs MODULE
//
//
=================================================================================================================================================================
*/

class VAO
{
public:
	VAO();
	~VAO();

	GLuint ID;
	
	void EnableAttribute(GLuint index);
	void DisableAttribute(GLuint index);
	void Bind();
	void Unbind();
	int MaxAttributes();
private:
	GLint MaxAttrib;
};
//_______________________________________________________________________________________________________________________________________________________________
class VertexBuffer
{
public:
    VertexBuffer() {}
   ~VertexBuffer();
    VertexBuffer(Vec3 *Vertexdata, GLsizei count);

	GLuint ID;
    GLint  ElementCount;

    void Bind();
    void Unbind();

    void Lock(GLenum access);
    void Unlock();
	Vec3  *Data;
    GLfloat *Read();
    void Write(GLuint pos, GLfloat *data);
    void Rebuild();
private:

};
//_______________________________________________________________________________________________________________________________________________________________
class ColorBuffer
{
public:
    ColorBuffer() {}
    ~ColorBuffer();
	ColorBuffer(Vec3 *ColorData, GLsizei count)
	{
		Vec4 *tempdata = new Vec4[count];

		for_loop(Index, count)
		{
			tempdata[Index] = Vec4(ColorData[Index].r, ColorData[Index].g, ColorData[Index].b, 1.0f);
		}
		*this = ColorBuffer(tempdata, count);
		delete[]tempdata;
	}
    ColorBuffer(Vec4 *ColorData, GLsizei count);

    GLuint ID;
    GLint  ElementCount;

public:
    void Bind();
    void Unbind();
	Vec4 *Data;
private:
	
};
//_______________________________________________________________________________________________________________________________________________________________
class NormalBuffer
{
public:
    NormalBuffer() {}
    ~NormalBuffer();
    NormalBuffer(Vec3 *NormalData, GLsizei count);

    GLuint ID;
    GLint  ElementCount;

public:
    void Bind();
    void Unbind();
	Vec3  *Data;

private:
};
//_______________________________________________________________________________________________________________________________________________________________
class IndexBuffer
{
public:
    IndexBuffer() {}
    ~IndexBuffer();
    IndexBuffer(GLuint *data, GLsizei count);

    GLuint ID;
    GLuint ElementCount;

public:
    void Bind();
    void Unbind();
private:
	GLuint *Data;
};
//_______________________________________________________________________________________________________________________________________________________________
class Image
{
public:
    Image();
    ~Image();
    Image(const char *filename);
    Image(GLenum param, const char *filename);

    GLuint ID;

	GLuint Width,
		   Height,
		   ImageSize;

    GLubyte* LoadBMP(const char *filename); // Possibly make this Static and return an entire Image object

    inline void SetSize(float width, float height);

    void GenColorTexture(float width, float height);
    void GenDepthTexture(float width, float height);
    void Bind();
    void Unbind();

    void VerticalFlip();// void *image, int w, int h, int bytes_per_pixel)

	GLint MaxTextureSize();
private:

	GLubyte header[54];
	GLuint  dataPos;

	GLubyte *Data;
	GLint MaxTexture;

};
//_______________________________________________________________________________________________________________________________________________________________
class UVBuffer
    {
    public:
        UVBuffer();
        ~UVBuffer();
		UVBuffer(Vec2 *UVdata, GLsizei count); //UVBuffer(Image &img, Vec2 *UVdata, GLsizei count);

        GLuint ID;
        GLuint ElementCount;

        Vec2    *Data;
        //Image Picture;
    public:

        void Bind();
        void Unbind();
    };
//_______________________________________________________________________________________________________________________________________________________________
class TextureBuffer
{
	TextureBuffer();
	~TextureBuffer();


	Image    *Texture;
	UVBuffer *TextureCoords;

	void Bind();
	void Unbind();
};
//_______________________________________________________________________________________________________________________________________________________________

///==============================================================================================================================================================
// TODO: Attach the Framebuffer, RenderBuffer, PixelBuffer and the Image and Texture stuff together to work as abstracted Factory 
///== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
class RenderBuffer
    {
    public:
        RenderBuffer() {}
        ~RenderBuffer();
        RenderBuffer( GLuint Width, GLuint Height);
  
//        Shader RBShader;

        GLuint ID,
            TexCoordsID;

        void Bind();
        void Unbind();
        void Render();

        void PositionQuad(Vec3 pos);
        void RotateQuad(Vec3 rot);

    private:
        void MakeTestQuad();

        GLsizei Width, Height;
        GLsizei WindowWidth, WindowHeight;

        VertexBuffer  *TestQuad;
        IndexBuffer   *TestQuadIBO;

        Vec3 Position,
            Rotation;
    };
//_______________________________________________________________________________________________________________________________________________________________
//_______________________________________________________________________________________________________________________________________________________________
class PBuffer
{
	PBuffer() {}
	~PBuffer() {}
	PBuffer(GLuint width, GLuint height);

	GLuint width;
	GLuint height;
};
///== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
//_______________________________________________________________________________________________________________________________________________________________
class FrameBuffer
    {
    public:

        ~FrameBuffer();
        FrameBuffer( GLuint Width, GLuint Height);
        FrameBuffer( RenderBuffer rendernb); // FUTURE AFTER RB CLASS IS FINISHED


//        Shader FBShader;

        GLuint ID;

        Image ColorTexture;
        Image DepthTexture;

        void Bind();
        void Unbind();
        void Render();

        void PositionQuad(Vec3 pos);
        void RotateQuad(Vec3 rot);

        void AttachRenderBuffer(RenderBuffer *rbuffer);
        void AttachUVBuffer(Image *texture);
    private:
        void MakeTestQuad();

        GLsizei Width, Height;
        GLsizei WindowWidth, WindowHeight;

        VertexBuffer  *TestQuad;
        IndexBuffer   *TestQuadIBO;

        GLuint TexCoordsID;

        Vec3 Position,
            Rotation;

        GLubyte DrawBufferCount;
    };
//_______________________________________________________________________________________________________________________________________________________________




//_______________________________________________________________________________________________________________________________________________________________
class VAOBuffer
{
public:
    VAOBuffer();
    ~VAOBuffer() {}

    VertexBuffer *Vertices;
    IndexBuffer  *Indices;
    NormalBuffer *Normals;
    UVBuffer     *Textures;
    ColorBuffer  *Colors;

	GLuint BatchID = 0;
 
	int NormalsOffset;
	int UVOffset;
	int ColorsOffset;

    void Attach(VertexBuffer *vertices);
    void Attach(IndexBuffer  *indices);
    void Attach(NormalBuffer *normals);
    void Attach(UVBuffer     *textures);
    void Attach(ColorBuffer  *color);

    void Bind();
    void Unbind();
};




///         SHADER MODULE
/*===============================================================================================================================================================
//
//
//      SHADER MODULE
//
//
=================================================================================================================================================================
*/

#include<unordered_map>

enum Uniformtype
{
	Float,
	Integer,
	Vector2,
	Vector3,
	Vector4,
	Matrix4
};
struct Uniform
{

	Uniform(Uniformtype type, char *name, void *value)
		:Type(type), Name(name), Value(value) {}
	Uniform() {}

	Uniformtype Type;
	char *Name;
	void *Value;
};
class Shader
{

private:

	const char* m_Vertpath;
	const char* m_Fragpath;
	GLuint ID;
public:
	Shader(const char* vertPath, const char* fragPath);
	Shader();

	~Shader();

	void Enable();
	void Disable();

static GLuint VertexLocation;
static GLuint ColorsLocation;
static GLuint NormalsLocation;


	std::vector<Uniform> Uniforms;

	GLint GetUniformLocation(GLchar *);

	void  SetUniform1f(GLchar *name, float value);
	void  SetUniform1Int(GLchar *name, int value);

	void  SetUniform2f(GLchar *name, Vec2 &vector);
	void  SetUniform3f(GLchar *name, Vec3 &vector);
	void  SetUniform4f(GLchar *name, Vec4 &vector);
	void  SetUniformMat4(GLchar *name, Matrix &matrix);
	void  SetTexture(GLchar *name, int slot);
	void  Delete();

	void AttachUniform(GLchar *name, Uniformtype type, void *variable);

	GLuint Name() { return ID; }
static std::vector<Shader *> ActiveShader;
static Shader* GetActiveShader() { return ActiveShader.back(); }
private:
	GLuint Load();
	// FOR CACHING THE SHADER LOCATIONS.
	////	   std::unordered_map<std::string, int> LocationCache;

	//	   if (LocationCache.find(name) != LocationCache.end()) return LocationCache[name];
	//	   if (loc == -1) Print("Uniform does not exist")
	//	   else LocationCache[name] = location;
};

//== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == 




///        CAMERA 
/*===============================================================================================================================================================
//
//
//      CAMERA MODULE
//
//
=================================================================================================================================================================
*/

class Viewport
{
#if _OPENGL_FIXED_FUNCTION
public:
	Viewport() :
		Position(0, 0, 0),
		Rotation(0, 0, 0),
		Speed(1.0f)
	{}

	Viewport(Vec3 position, Vec3 Rotation);
 
	float
		FOV,
		AspectRatio,
		Near,
		Far;

	Vec3
		Position,
		Rotation,
		Forward,
		Right,
		Up;

	Matrix   ProjectionMatrix;
	Matrix   ViewMatrix;

	float Speed;
	static Viewport *Camera;

	std::vector<Matrix> MatrixStack;
	//=======================================================================================================================================================
	// FUNCTIONS FOR THE CAMERA CLASS
	//=======================================================================================================================================================

	void Update();
	void Render();

	Matrix GetModelViewMatrix();

	void Rotate(float pitch, float yaw);
	void MoveForward(float speed);
	void MoveBack(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);
	//=======================================================================================================================================================

	void ClampCamera();

	std::vector<Matrix> View;
	std::vector<Matrix> Projection;
	
	Matrix GetViewMatrix() { return View.back(); }
	Matrix GetProjectionMatrix() { return Projection.back(); }


#else
	
public:
	Viewport() :
		Position(0, 0, 0),
		Rotation(0, 0, 0),
		Speed(1.0f)
	{}

	Viewport(Vec3 position, Vec3 Rotation);

	float
		FOV,
		AspectRatio,
		Near,
		Far;



	Matrix   ProjectionMatrix;
	Matrix   ViewMatrix;


	Vec2 Orientation;

	float Speed;

	//=======================================================================================================================================================
	// FUNCTIONS FOR THE CAMERA CLASS
	//=======================================================================================================================================================

	void Update();
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	//glm::vec3 direction(
	//    cos(verticalAngle) * sin(horizontalAngle),
	//    sin(verticalAngle),
	//    cos(verticalAngle) * cos(horizontalAngle)
	//);
	//   
	void Render();
	Matrix GetViewMatrix();

	void Rotate(float pitch, float yaw);
	void MoveForward(float speed);
	void MoveBack(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);
	//=======================================================================================================================================================

	void ClampCamera();

	Matrix RotateX(GLfloat Angle);
	Matrix RotateY(GLfloat Angle);
	Matrix RotateZ(GLfloat Angle);

	static Viewport *Camera;
	private:
		Vec3
			Position,
			Rotation,
			Forward,
			Right,
			Up;
#endif
};

//== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==



///      ENGINE 
/*
=============================================================================================================================================================
//
//      ENGINE MODULE
//
=============================================================================================================================================================
*/


class Material
{
public:
	Material()
	{
		Renderer = NULL;
		Skin = NULL;
	}
	struct
	{
		Vec3 
			Ambient = Vec3(.5),
			Diffuse = Vec3(.5),
			Specular = Vec3(1.0f);
	}SurfaceColor;
	void SetSurfaceColor(Vec3 A, Vec3 D, Vec3 S)
	{
		SurfaceColor.Ambient = A;
		SurfaceColor.Diffuse = D;
		SurfaceColor.Specular = S;
	}

	float Shine = .2f;
	Shader *Renderer = NULL;

	struct
	{
		Image Texture;
		Image NormalsTexture;
		Image GlowTexture;
		Image SpecularTexture;
		UVBuffer TextureCoords;
	}*Skin;

	void Bind()
	{
		if (Renderer != NULL)
		{
			Shader::ActiveShader.push_back(Renderer);
			Renderer->Enable();

			Shader::GetActiveShader()->SetUniform3f("MaterialAmbient", SurfaceColor.Ambient );
			Shader::GetActiveShader()->SetUniform3f("MaterialDiffuse", SurfaceColor.Diffuse );
			Shader::GetActiveShader()->SetUniform3f("MaterialSpecular", SurfaceColor.Specular );

		}
		if (Skin != NULL)
		{
			Skin->Texture.Bind();
			Skin->TextureCoords.Bind();
		}
	}
	void Unbind()
	{
#if _DEBUG
		if (Skin != NULL)
		{
			Skin->Texture.Unbind();
			Skin->TextureCoords.Unbind();
		}
		if (Renderer != NULL)
		{
			Shader::GetActiveShader()->Enable();
			Shader::ActiveShader.pop_back();
		}
#endif
	}
};
class Light
{
public:
	Light(){}
	Light(Vec3 pos)
	{
		Position = pos;
	}
	Light(Vec3 pos, Vec3 ambient, Vec3 diffuse, Vec3 specular)
	{
		Position = pos;
		Vec3 AmbientColor = ambient;
		Vec3 DiffuseColor = diffuse;
		Vec3 SpecularColor = specular;
	}


	enum Lighttype
	{
		Ambient, Diffuse, Specular 
	};
	void SetRotation(Vec3 rot)
	{
		Rotation = rot;
	}
	void SetPosition(Vec3 pos)
	{
		Position = pos;
	}
	void Set(Lighttype which, Vec3 value)
	{
		switch (which)
		{
		case Ambient:
			AmbientColor = value;
			break;

		case Diffuse:
			DiffuseColor = value;
			break;

		case Specular:
			SpecularColor = value;
			break;
		}
	}
	void Set(Vec3 ambient, Vec3 diffuse, Vec3 specular)
	{
		AmbientColor = ambient;
		DiffuseColor = diffuse;
		SpecularColor = specular;
	}

	void Bind()
	{
		Shader::GetActiveShader()->SetUniform3f("AmbientLight", AmbientColor);
		Shader::GetActiveShader()->SetUniform3f("DiffuseLight", DiffuseColor);
		Shader::GetActiveShader()->SetUniform3f("SpecularLight", SpecularColor);
	}
	void Unbind()
	{
	}
private:
	vec3 AmbientColor = Vec3(0.2f);
	vec3 DiffuseColor = Vec3(0.5f);
	vec3 SpecularColor = Vec3(1.0f);

	Vec3 Position;
	Vec3 Rotation;


	// vec3 ambient = light.ambient * material.ambient;
	// vec3 diffuse = light.diffuse * (diff * material.diffuse);
	// vec3 specular = light.specular * (spec * material.specular);
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
class Mesh
{
public:
	Mesh() 
	{
		Surface = NULL;
	}

	Vec3 Position = Vec3(1.0f);
	Vec3 Rotation = Vec3(1.0f);
	Vec3 Scale = Vec3(1.0f);

	Matrix Transform;

	VAOBuffer *Polygons;
	Material *Surface;
	virtual void Bind()
	{
		if (Surface != NULL)
		{
			Surface->Bind(); // This seems like its going to get called to much. 
			// If there is multiple levels The Uniforms are going to get passed way to much and I will have to call
			// Set uniform entirely to much unless I change something.
			// Make this the first stop if more performance is needed
		}
#if _OPENGL_FIXED_FUNCTION
		_TRANSFORM();
#else
		Polygons->Bind();
		Matrix MVP = Viewport::Camera->GetViewMatrix();
		Shader::GetActiveShader()->SetUniformMat4("ModelViewProjectionMatrix", MVP * Transform);
#endif
	}
	virtual void Unbind()
	{
#if _DEBUG
		Polygons->Unbind();
		if (Surface != NULL)
		{
			Surface->Unbind();
		}
#endif
		glPopMatrix();
	}
	virtual void Render()
	{
		Bind();
		Update();
		_GL(glDrawElements(GL_TRIANGLES, Polygons->Indices->ElementCount, GL_UNSIGNED_INT, nullptr));
		Unbind();

	}
	virtual void Update()
	{
		Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
		Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
		Transform = glm::translate(Transform, Position);
	}

	void Add(VAOBuffer *vao) { Polygons = vao; }
	void Add(Material *surface) { Surface = surface; }
};
class Group
{
public:
	Group()
	{
		Surface = NULL;
	}

	Vec3 Position = Vec3(0.0f);
	Vec3 Rotation = Vec3(0.0f);

	std::vector<Mesh> Objects;
	Material *Surface;

	int ObjectCount = 0;

	void Add(Mesh object)
	{
		Objects.push_back(object);
		ObjectCount++;
	}
	void Attach(Material *surface)
	{
		Surface = surface;
	}

	void Bind()
	{
		if (Surface != NULL)
		{
			Surface->Bind();
		}
//		_TRANSFORM();
	}
	void Unbind()
	{
		if (Surface != NULL)
		{
			Surface->Unbind();
		}
		glPopMatrix();
	}
	void Update()
	{
		for (auto& M : Objects)
		{
			M.Update();
		}
	}
	void Render()
	{
	    Bind();
		for (auto& M : Objects)
		{
			M.Render();
		}
		Unbind();
	}
};
class Scene
{
public:
	Scene()
	{
		Surface = NULL;
		Groups.push_back(Group());
	}
	Scene(Material *renderer)
	{
		Surface = renderer;
		Groups.push_back(Group());
	}

	std::vector<Group> Groups;
	std::vector<Light> Lights;
	int LightCount = 0;
	int GroupCount = 0;

	Viewport Camera;
	Material *Surface;

	void Update()
	{
		//Surface->Renderer->SetUniformMat4("ProjectionMatrix",Camera.ProjectionMatrix);
		//Surface->Renderer->SetUniformMat4("ViewMatrix",Camera.ViewMatrix);
		for (auto& G : Groups)
		{
			G.Update();
		}
	}
	void Render()
	{
//		Camera.Bind();
		if (Surface != NULL)
		{
			Surface->Bind();
		}
		for (auto& G : Groups)
		{
			G.Render();
		}
		if (Surface != NULL)
		{
			Surface->Unbind();
		}
		glFlush();
	}

	void AddGroup(const Group& group)
	{
		Groups.push_back(group);
		GroupCount++;
	}
	void AddLight(const Light& light)
	{
		Lights.push_back(light);
		LightCount++;
	}

	void Attach(Material *renderer)
	{
		Surface = Surface;
	}
	void Attach(Viewport camera)
	{
		Camera = camera;
	}
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------------








/// look into this https://www.opengl.org/discussion_boards/showthread.php/155504-Render-to-Texture-vs-CopyTexSubImage
// CONVERT THIS NAME TO SOMETHING LIKE IMAGE SINCE IT DOES NOT MAKE A TEXTURE BUT INSTEAD LOADS AN IMAGE
// THE TEXTUREBUFFER SHOULD EITHER BECOME TEXTURE OR PROB EVEN BETTER CALLED UV_BUFFER OR SOMETHING AND HAVE 
// A TEXTURE CLASS WHICH IS ITSELF A COMBINATION OF THE IMAGE AND THE TEXTURE COORDS SO THAT THEY CAN BE LOADED
// AND UNLOADED AT THE SAME TIME OR SEPERATELY AS NEEDED.
//-------------------------------------------------------------
// ADD ALL VARIOUS BUFFERS TO A SINGLE BUFFER VERTEX, INDEX, COLOR, TEXTURE, NORMALS etc... 
// Into a single Buffer for their type, Bind each buffer than call  Draw Instances
//-----------------------
//1.	Uniform Buffer Objects
//2.	Texture Buffer Objects
//3.	Instanced_arrays_ARB
//=================================================================================================================== 
//___________________________________________________________________________________________________________________ 
//1. pbuffer extension initialization
//2. pbuffer creation
//3. pbuffer binding
//4. pbuffer destruction. 
class Block :public Mesh
{
public:

	~Block() {}
	Block() {}
	Block(Vec3 pos, float size);

	int ID;

	GLuint VertexCount;

	std::vector<Vec3>    VertexList;
	std::vector<Vec3>    NormalList;
	std::vector<Vec2>    TextureCoords;
	std::vector<Vec4>    ColorList;
	std::vector<GLuint>  IndexList;

// Vec3     Verts[24];
// Vec3     Norms[24];
// Vec2        UV[24];
//	GLuint               Indice[36];

	IndexBuffer    *Indices;
	VertexBuffer   *Vertices;
	NormalBuffer   *Normals;

//	Vec3                Color[24];
	ColorBuffer    *Colors;
};
class Sphere : public Mesh
{
public:
	// Vec3 Position;
	Vec3 Rotation;
	Vec3 Scale;

	int   Mesh_ID;

	float Radius;

public:// OpenGL Stuff

	Sphere() {}
	Sphere(Vec3 pos, float radius, int sectors);

	Vec3  Vertices[648];
	Vec3  Normals[648];
	GLuint Indices[972];
	RGBf  Colors[648];

	GLuint VertexCount;
	GLuint ColorCount;

	void Set_Position(float x, float y, float z) { Position = Vec3(x, y, z); }
	void Set_Position(Vec3 pos) { Position = pos; }
	void Set_Rotation(float x, float y, float z) { Rotation = Vec3(x, y, z); }
	void Rotate(float x, float y, float z);
	void SetRotation(Vec3 rot) { Rotation = rot; }

	Vec3 Get_Position() { return Position; }
	Vec3 Get_Rotation() { return Rotation; }
};