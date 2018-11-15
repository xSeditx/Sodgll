#include"Renderer.h"

std::vector<Shader*> Shader::ActiveShader;

// [ TIP: ]
// In case you don't have VAOs, you can still render pretty much in the same way. Just remove the VAO calls and remember
// to always reset the vertex format before drawing. I.e.: glVertexAttribPointer.


VAO::VAO() 
{
	glGenVertexArrays(1, &ID);
}
VAO::~VAO()
{
	//glDeleteVertexArrays(ID);
//	Print("Delete VAO");
}
void VAO::Bind()
{
	glBindVertexArray(ID);
}
void VAO::Unbind()
{
	glBindVertexArray(0);
}
void VAO::EnableAttribute(GLuint ind)
{
	glEnableVertexAttribArray(ind);
}
void VAO::DisableAttribute(GLuint ind)
{
	glDisableVertexAttribArray(ind);
}
int  VAO::MaxAttributes()
{
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &MaxAttrib);
	return MaxAttrib;
}


//==================================================================================================================================================
//__________________________________ VERTEX BUFFER CLASS ___________________________________________________________________________________________

VertexBuffer::~VertexBuffer()
{
//    glDeleteBuffers(1, &ID);
}
VertexBuffer::VertexBuffer(Vec3 *vertexdata,GLsizei vcount)
            : ElementCount(vcount),
              ID(0) 
{
    Data = new Vec3[vcount];
    memcpy(Data, vertexdata, sizeof(Vec3) * vcount);
   _GL( glGenBuffers(1 , &ID));
    glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, ElementCount * sizeof(Vec3), vertexdata, GL_DYNAMIC_DRAW) ; 
    glBindBuffer(GL_ARRAY_BUFFER,0);
}
void VertexBuffer::Bind()
{
#if _OPENGL_FIXED_FUNCTION
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	_GL(glVertexPointer(3, GL_FLOAT, 0, (char *)NULL));
	glEnableClientState(GL_VERTEX_ARRAY);
#else
    glBindBuffer(GL_ARRAY_BUFFER, ID);
	glEnableVertexAttribArray(Shader::VertexLocation);
	glVertexAttribPointer(Shader::VertexLocation, 3, GL_FLOAT, GL_FALSE, 0, (char *)NULL);
#endif
}
// GLuint Location = glGetAttribLocation(Shader::GetActiveShader()->Name(), "VertexColor");
// glEnableVertexAttribArray(Location);
// glBindBuffer(GL_ARRAY_BUFFER, ID);
// _GL(glVertexAttribPointer(Location, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), 0));

void VertexBuffer::Unbind()
{
#if _DEBUG
#if _OPENGL_FIXED_FUNCTION
          glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER,0);
#else
	glDisableVertexAttribArray(Shader::VertexLocation);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
#endif
}
void VertexBuffer::Lock(GLenum access)
{
        Data = (Vec3 *)glMapBuffer(ID, access);
}
void VertexBuffer::Unlock()
{
    glUnmapBuffer(ID);
}
void VertexBuffer::Rebuild()
{    
    glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferSubData(GL_ARRAY_BUFFER, 0,ElementCount * sizeof(Vec3), Data) ; 
    glBindBuffer(GL_ARRAY_BUFFER,0);
}
//  void VertexBuffer::Write(GLuint pos, GLfloat *data)
//  {
//  	glBindBuffer(GL_ARRAY_BUFFER, ID);
//  	glBufferSubData(GL_ARRAY_BUFFER, 0, ElementCount * sizeof(Vec3), Data);
//  	glBindBuffer(GL_ARRAY_BUFFER, 0);
//  }

//==================================================================================================================================================
//__________________________________ INDEX BUFFER CLASS ____________________________________________________________________________________________

IndexBuffer::~IndexBuffer()
{
//    glDeleteBuffers(1, &ID);
}
IndexBuffer::IndexBuffer(GLuint *data, GLsizei count)
           : ElementCount(count),
             ID(0)
{
        Data = new GLuint[count];
        memcpy(Data, data, sizeof(GLuint) * count);

        glGenBuffers(1,&ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
void IndexBuffer::Bind()
{
  //  glEnableVertexAttribArray();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ID);
        glIndexPointer(GL_FLOAT, 0, (void *) NULL);
        glEnableClientState(GL_INDEX_ARRAY);
}
void IndexBuffer::Unbind()
{
#if _DEBUG
#if _OPENGL_FIXED_FUNCTION
        glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
#else

#endif
#else
#endif
}

//==================================================================================================================================================
//__________________________________ COLOR BUFFER CLASS ____________________________________________________________________________________________

ColorBuffer::~ColorBuffer()
{
//        glDeleteBuffers(1, &ID);
}
ColorBuffer::ColorBuffer(Vec4 *ColorData, GLsizei count)
           : ElementCount(count),
             ID(0)
{
    Data = new Vec4[count];
	memcpy(Data, ColorData, sizeof(Vec4) * count);
    glGenBuffers(1 , &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, ElementCount * sizeof(Vec4), ColorData, GL_STATIC_DRAW) ; 
    glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind BUFFER_ID since its now bound to the ID;
}
void ColorBuffer::Bind()
{

#if _OPENGL_FIXED_FUNCTION
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glColorPointer(4, GL_FLOAT, 0, (char *) NULL);
	   glEnableClientState(GL_COLOR_ARRAY);
#else
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glEnableVertexAttribArray(Shader::ColorsLocation);
	glVertexAttribPointer(Shader::ColorsLocation, 4, GL_FLOAT, GL_FALSE, 0, (char *)NULL);
#endif

//"VertexPosition")
//"VertexNormals");
//"TextureCoords");
//"VertexColors");

}
void ColorBuffer::Unbind()
{
#if _DEBUG
#if _OPENGL_FIXED_FUNCTION
         glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER,0);
#else
	glDisableVertexAttribArray(Shader::ColorsLocation);
	glBindBuffer(GL_ARRAY_BUFFER,0);
#endif
#else
#endif
} 
 
//==================================================================================================================================================
//__________________________________ NORMAL BUFFER CLASS ___________________________________________________________________________________________
// TODO: Much work needs to be done here.
NormalBuffer::~NormalBuffer()
{
//    glDeleteBuffers(1, &ID);
}
NormalBuffer::NormalBuffer(Vec3 *NormalData, GLsizei count)
            : ElementCount(count),
              ID(0)
{
    Data = new Vec3[count];
	memcpy(Data, NormalData, sizeof(Vec3) * count);
    glGenBuffers(1 , &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, ElementCount * sizeof(Vec3), NormalData, GL_STATIC_DRAW) ; 
    glBindBuffer(GL_ARRAY_BUFFER,0); 
}
void NormalBuffer::Bind()
                
{
#if _OPENGL_FIXED_FUNCTION
    glBindBuffer(GL_ARRAY_BUFFER, ID); // 
        glNormalPointer(GL_FLOAT, 0, (void *) NULL);
		_GL(glEnableClientState(GL_NORMAL_ARRAY));
#else
	_GL(glBindBuffer(GL_ARRAY_BUFFER, ID));
	glEnableVertexAttribArray(Shader::NormalsLocation);
	glVertexAttribPointer(Shader::NormalsLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

#endif
}
void NormalBuffer::Unbind()
{
#if _DEBUG
#if _OPENGL_FIXED_FUNCTION
	       glDisableClientState(GL_NORMAL_ARRAY);
	   glBindBuffer(GL_ARRAY_BUFFER,0);
#else
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(Shader::NormalsLocation);
#endif
#endif
}


//==================================================================================================================================================
//__________________________________ FRAME BUFFER CLASS ____________________________________________________________________________________________
// There are two types of framebuffer-attachable images; texture images and renderbuffer images. If an image of a texture 
//     object is attached to a framebuffer, OpenGL performs "render to texture". And if an image of a renderbuffer object is
//     attached to a framebuffer, then OpenGL performs "offscreen rendering".
// Texture = Read
// RenderBuffer = Offscreen Rendering
// Attach different Textures or Render buffer with Different Bitmask to get different colors
// Depth and Stencil buffer is also an Option.
// Use: Multiple ColorBuffers   [ GL_COLOR_ATTACHMENT0,n....., ] 
// One Depth attachment point   [ GL_DEPTH_ATTACHMENT   ]
// One Stencil attachment point [ GL_STENCIL_ATTACHMENT ]
// Notice that the framebuffer object itself does not have any image storage(array) in it, 
// but it has only multiple attachment points.

FrameBuffer::~FrameBuffer()
{
 //   if(&ColorTexture != nullptr) glDeleteTextures(1, &ColorTexture.ID);
  //  if(&DepthTexture != nullptr) glDeleteTextures(1, &DepthTexture.ID);
 //   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
 //   glDeleteFramebuffersEXT(1, &ID);
}
FrameBuffer::FrameBuffer(GLuint  width, GLuint height)
    : Width(width),
      Height(height), DrawBufferCount(0)
{
 //   FBShader = shader;
    MakeTestQuad();

    glGenFramebuffersEXT(1,&ID);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ID);
        
        ColorTexture.GenColorTexture(width, height);
          DepthTexture.GenDepthTexture(width, height);
           // RenderBuffer RB(shader,width,height);
//           Attach Color texture to FBO  && Attach depth texture to FBO

          //  AttachRenderBuffer(&RB);
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, ColorTexture.ID, 0);
     const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0_EXT };
glDrawBuffers(sizeof(draw_buffers)/ sizeof(draw_buffers[0]), draw_buffers);       

            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT , GL_TEXTURE_2D, DepthTexture.ID, 0);

            if( glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
            {
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
                return;
            }
 
            DepthTexture.Unbind();
        ColorTexture.Unbind();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
}
void FrameBuffer::Bind()
{
 //   FBShader.Enable();

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ID);
        glClearColor(1,0,0,1.0f);
		glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
     glDrawBuffer(GL_FRONT_AND_BACK);
        
}
void FrameBuffer::Unbind()
{

  //  glViewport(0,0,WindowWidth, WindowHeight);

//    FBShader.Disable();
 //   glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

}
void FrameBuffer::Render()
{
#if 1
glPushMatrix();
glMatrixMode(GL_PROJECTION);
glPushMatrix();
glLoadIdentity();
glMatrixMode(GL_TEXTURE);
glPushMatrix();
glLoadIdentity();
glMatrixMode(GL_MODELVIEW);
glPushMatrix();
glLoadIdentity();


glEnable(GL_TEXTURE_2D);

glBindTexture(GL_TEXTURE_2D,  ColorTexture.ID);
glBegin(GL_QUADS);
glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, -1.0);
glTexCoord2f(0.0, 1.0); glVertex2f(-1.0,  -0.5);
glTexCoord2f(1.0, 1.0); glVertex2f( -0.5,  -0.5);
glTexCoord2f(1.0, 0.0); glVertex2f( -0.5, -1.0);
glEnd();

glBindTexture(GL_TEXTURE_2D, DepthTexture.ID);
glBegin(GL_QUADS);
glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -1.0);
glTexCoord2f(0.0, 1.0); glVertex2f(-0.5,  -0.5);
glTexCoord2f(1.0, 1.0); glVertex2f( -0.0,  -0.5);
glTexCoord2f(1.0, 0.0); glVertex2f( -0.0, -1.0);
glEnd();



glMatrixMode(GL_TEXTURE);
glPopMatrix(); 
glMatrixMode(GL_MODELVIEW);
glPopMatrix();
glMatrixMode(GL_PROJECTION);
glPopMatrix();
glMatrixMode(GL_MODELVIEW);
glPopMatrix();

glBindTexture(GL_TEXTURE_2D, 0);
#else
    glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
    
        glTranslatef(Position.x,
                     Position.y, 
                     Position.z);
        glRotatef(Rotation.x, 1.0f, 0.0f, 0.0f);
        glRotatef(Rotation.y, 0.0f, 1.0f, 0.0f);
        glRotatef(Rotation.z, 0.0f, 0.0f, 1.0f);
    
        TestQuad->Bind();
            TestQuadIBO->Bind();
            
         // glBindTexture(GL_TEXTURE_2D, DepthTexture.ID);
            glBindTexture(GL_TEXTURE_2D, ColorTexture.ID);           
                glBindBuffer(GL_ARRAY_BUFFER, TexCoordsID);
                glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    
                    glDrawElements(GL_TRIANGLES, TestQuadIBO->ElementCount , GL_UNSIGNED_INT, nullptr);
                    
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            TestQuadIBO->Unbind();
        TestQuadIBO->Unbind();       
    glPopMatrix();
#endif

}
void FrameBuffer::PositionQuad(Vec3 pos)
{
    Position = pos;
}
void FrameBuffer::RotateQuad(Vec3 rot)
{
    Rotation = rot;
}
void FrameBuffer::MakeTestQuad()
{
    Position = Vec3(0,0,0);
    Rotation = Vec3(0,0,0);

    float Size = 10;
    Vec3 quad[] = 
    {
        Vec3( Size,  -10.0f,  Size),
        Vec3(-Size,  -10.0f,  Size),
        Vec3(-Size,  -10.0f, -Size),
        Vec3( Size,  -10.0f, -Size),
    };

    Vec4 color[] = 
    {
        Vec4(  0.0f,  0.0f, 0.0f,   0.0f),
        Vec4(  0.0f,  0.0f, 0.0f,   0.0f),
        Vec4(  0.0f,  0.0f, 0.0f,   0.0f),
        Vec4(  0.0f,  0.0f, 0.0f,   0.0f),
    };

    GLuint quadIB[] = 
    {
        0,1,2,  0,2,3
    };
    Vec2 UVs[] = 
    {
        Vec2(0.0f, 1.0f),
        Vec2(0.0f, 0.0f),
        Vec2(1.0f, 0.0f),
        Vec2(1.0f, 1.0f)
    };

    TestQuad    = new VertexBuffer(quad,4);
    TestQuadIBO = new IndexBuffer(quadIB,6);

    glGenBuffers(1, &TexCoordsID);
        glBindBuffer(GL_ARRAY_BUFFER, TexCoordsID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * 4 , UVs, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void FrameBuffer::AttachRenderBuffer(RenderBuffer *rbuffer)
{
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbuffer->ID);
}
void FrameBuffer::AttachUVBuffer(Image *texture)
{
    glBindFramebufferEXT(GL_FRAMEBUFFER, ID);
    texture->Bind();
    if(texture->Width != Width || texture->Height != Height)
    {
        Print( "Error Line:" << __LINE__);
        Print( "Can not Attach Texture to FrameBuffer Object Number : " << ID);
        return;
    }
//    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, texture.Width, texture.Height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + DrawBufferCount, texture->ID, 0);
}

//==================================================================================================================================================
//__________________________________ RENDER BUFFER CLASS ___________________________________________________________________________________________
//                                                                                                                                    
//glRenderbufferStorageMultisample() creates a renderbuffer image for multisample anti-aliasing rendering mode.

RenderBuffer::~RenderBuffer()
{
//   glBindRenderbufferEXT(GL_RENDERBUFFER, 0);
//    glDeleteRenderbuffersEXT(1, &ID);
}
RenderBuffer::RenderBuffer( GLuint width, GLuint height)
    : Width(width),
      Height(height)
{
 //   RBShader = shader;
    MakeTestQuad();

    glGenRenderbuffersEXT(1,&ID);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, ID);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, Width, Height);        
//           Attach Color texture to FBO  && Attach depth texture to FBO
//            glRenderbufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, ColorTexture.ID, 0);
//            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT , GL_TEXTURE_2D, DepthTexture.ID, 0);
//            if( glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
//            {
//                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
//                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
//            }
        
  //  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0); 
}
void RenderBuffer::Bind()
{
//    RBShader.Enable();
    glBindRenderbufferEXT(GL_RENDERBUFFER, ID);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
void RenderBuffer::Unbind()
{
//    RBShader.Disable();
 //   glBindRenderbufferEXT(GL_RENDERBUFFER, 0);
}
void RenderBuffer::Render()
{
    glPushMatrix();
	_TRANSFORM();
    
        TestQuad->Bind();
            TestQuadIBO->Bind();
            
//           glBindTexture(GL_TEXTURE_2D, DepthTexture.ID);
            
                glBindBuffer(GL_ARRAY_BUFFER, TexCoordsID);
                glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    
                    glDrawElements(GL_TRIANGLES, TestQuadIBO->ElementCount , GL_UNSIGNED_INT, nullptr);
                    
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            TestQuadIBO->Unbind();
        TestQuadIBO->Unbind();       
    glPopMatrix();
}
void RenderBuffer::PositionQuad(Vec3 pos)
{
    Position = pos;
}
void RenderBuffer::RotateQuad(Vec3 rot)
{
    Rotation = rot;
}
void RenderBuffer::MakeTestQuad()
{
    Position = Vec3(0,0,0);
    Rotation = Vec3(0,0,0);

    float Size = 10;
    Vec3 quad[] = 
    {
        Vec3( Size,  -10.0f,  Size),
        Vec3(-Size,  -10.0f,  Size),
        Vec3(-Size,  -10.0f, -Size),
        Vec3( Size,  -10.0f, -Size),
    };

    Vec4 color[] = 
    {
        Vec4(  0.0f,  0.0f, 0.0f,   0.0f),
        Vec4(  0.0f,  0.0f, 0.0f,   0.0f),
        Vec4(  0.0f,  0.0f, 0.0f,   0.0f),
        Vec4(  0.0f,  0.0f, 0.0f,   0.0f),
    };

    GLuint quadIB[] = 
    {
        0,1,2,  0,2,3
    };
    Vec2 UVs[] = 
    {
        Vec2(0.0f, 1.0f),
        Vec2(0.0f, 0.0f),
        Vec2(1.0f, 0.0f),
        Vec2(1.0f, 1.0f)
    };

    TestQuad    = new VertexBuffer(quad,4);
    TestQuadIBO = new IndexBuffer(quadIB,6);

    glGenBuffers(1, &TexCoordsID);
        glBindBuffer(GL_ARRAY_BUFFER, TexCoordsID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * 4 , UVs, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Obsolete but still might make a class to manage it just incase
//1. pbuffer extension initialization
//2. pbuffer creation
//3. pbuffer binding
//4. pbuffer destruction. 


//==================================================================================================================================================
//__________________________________ SIMULATED VAO CLASS ___________________________________________________________________________________________

VAOBuffer::VAOBuffer()
{
        Vertices = nullptr;
        Normals  = nullptr;
        Textures = nullptr;
        Indices  = nullptr;
        Colors   = nullptr;
}
void VAOBuffer::Attach(VertexBuffer  *vertices)
{
    Vertices = vertices;
}
void VAOBuffer::Attach(IndexBuffer   *indices)
{

    Indices = indices;
}
void VAOBuffer::Attach(NormalBuffer  *normals)
{
    Normals = normals;
}
void VAOBuffer::Attach(UVBuffer *texture)
{
    Textures = texture;
}
void VAOBuffer::Attach(ColorBuffer   *color)
{
    Colors = color;
}
void VAOBuffer::Bind()
{
    if (Vertices)  Vertices->Bind();
    if (Indices)    Indices->Bind();
    if (Textures)  Textures->Bind();
	if (Colors)      Colors->Bind();
	if (Normals)    Normals->Bind();
}
void VAOBuffer::Unbind()
{
#if _DEBUG
    if (Vertices)  Vertices->Unbind();
	if (Indices)    Indices->Unbind();
	if (Textures)  Textures->Unbind();
	if (Colors)      Colors->Unbind();
	if (Normals)    Normals->Unbind();
#endif
}

//__________________________________________________________________________________________________________________________________________________
//==================================================================================================================================================

//======================================================================================================================================================================================
//                                                    IMAGE CLASS                                                                                                                                   
//======================================================================================================================================================================================

Image::~Image()
{
#if _TEXTURE_DEBUG
	Print("Deleting Image: " << ID);
#endif
	glDeleteTextures(1, &ID);
	if (Data != nullptr)
	{
		//    delete(Data);
		//     Data = nullptr;
	}
}
Image::Image()
       : ID(0), 
         Data(nullptr),
         Width(0),
         Height(0)
{
    
  //  glEnable(GL_TEXTURE_2D);  
}
Image::Image(const char *filename)
    : ID(0)
{   
 //   glEnable(GL_TEXTURE_2D);
    Data = LoadBMP(filename);

    glGenTextures(1,&ID); 
    glBindTexture(GL_TEXTURE_2D, ID); // Or maybe GL_TEXTURE_BUFFER???
 // glActiveTexture(GL_TEXTURE0 + ID);
// Sampling mode for blending textals
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

// Texture Layout when sampling past texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, Data);
    glBindTexture(GL_TEXTURE_2D,0);
}
Image::Image(GLenum param, const char *filename)
    : ID(0)
	// This is to load images for a Cube map. Allowing Params to be added as well.
{   
   // glEnable(GL_TEXTURE_2D);
   Data =LoadBMP((char *)filename);
  //Data = readBMP((char *)filename);
  //  VerticalFlip();
    if (param == GL_TEXTURE_CUBE_MAP)
    {
        glGenTextures(1,&ID); 
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID); 
//      glActiveTexture(GL_TEXTURE0 + ID);
//      Sampling mode for blending textals
        
        glTexParameteri(param, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(param, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
//      Texture Layout when sampling past texture
        glTexParameteri(param, GL_TEXTURE_WRAP_S, GL_REPEAT);   
        glTexParameteri(param, GL_TEXTURE_WRAP_T, GL_REPEAT);   
        glTexParameteri(param, GL_GENERATE_MIPMAP, GL_TRUE);
        
        glTexImage2D(param, 0, GL_RGB, Width, Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, Data);
        glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    }

    if (param == GL_TEXTURE_2D) // Currently for transparency. 
    {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);
        //      glActiveTexture(GL_TEXTURE0 + ID);
        //      Sampling mode for blending textals
//    glPixelStorei(GL_PACK_ALIGNMENT,2 );
        glTexParameteri(param, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(param, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        //      Texture Layout when sampling past texture
        glTexParameteri(param, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(param, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(param, GL_GENERATE_MIPMAP, GL_TRUE);
  
        glTexImage2D(param, 0, GL_RGBA, Width , Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, Data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
void Image::Bind()
{
    glBindTexture(GL_TEXTURE_2D, ID);
}
void Image::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLubyte* Image::LoadBMP(const char *filename)
{
    FILE *File = fopen(filename,"rb");
    if(!File)
    {
        Print("Image Could Not Be Loaded");
        return 0;
    }

    if(fread(header,1,54,File)!=54)
    {
        Print("Not the Correct BMP format");
        return 0;
    }
//    BitMapHeader Head;

    dataPos   = *(int*)&(header[0x0A]);
    ImageSize = *(int*)&(header[0x22]);
    Width     = *(int*)&(header[0x12]);
    Height    = *(int*)&(header[0x16]);
//    memcpy(&Head, &header, sizeof(Head));
//    ElementCount = ImageSize;
    if(ImageSize == 0) ImageSize = Width * Height ;
    if(dataPos   == 0) dataPos = 54;
    fseek(File, dataPos, SEEK_SET);
    GLubyte *imagedata  = new GLubyte[ImageSize];
  
    // n texture coor-dinates (0, 0), (1, 0), (1, 1), and (0, 1) 
    fread(imagedata,1,ImageSize,File);
    fclose(File);

#if _FLIPIMAGE
    GLubyte *imageflip = new GLubyte[ImageSize];
    int Count2 = 0;
    for(int Counter = ImageSize; Counter > 0;Counter-=3)
    {
        imageflip[Counter]     = imagedata[Count2];
        imageflip[Counter + 1] = imagedata[Count2 + 1]; 
        imageflip[Counter + 2] = imagedata[Count2 + 2];
        Count2+=3;
    }
    return imageflip;
#endif

    return imagedata;
}
inline void Image::SetSize(float width, float height)
{
    Width  = width;
    Height = height;
}

void Image::GenColorTexture(float W, float H)
{
    Width  = W; 
    Height = H;

    if(Width && Height == 0)
    {
        Print("Texture Size not Initialized");
        exit(0);
    }
       glGenTextures(1, &ID);
       glBindTexture(GL_TEXTURE_2D, ID);
           glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA , Width, Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
           
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void Image::GenDepthTexture(float W, float H)
{
    Width  = W;
    Height = H;

    if(Width && Height == 0)
    {
        Print("Texture Size not Initialized");
        exit(0);
    }

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
    //    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    
        glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
}

GLint Image::MaxTextureSize()
{
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxTexture);
	return MaxTexture;
}
void Image::VerticalFlip()
{
	int row;
	size_t bytes_per_row = (size_t)Width * 4;
	unsigned char temp[2048];
	unsigned char *bytes = (unsigned char *)Data;

	for (row = 0; row < (Height >> 1); row++) {
		unsigned char *row0 = bytes + row*bytes_per_row;
		unsigned char *row1 = bytes + (Height - row - 1)*bytes_per_row;
		// swap row0 with row1
		size_t bytes_left = bytes_per_row;

		while (bytes_left) {
			size_t bytes_copy = (bytes_left < sizeof(temp)) ? bytes_left : sizeof(temp);
			memcpy(temp, row0, bytes_copy);
			memcpy(row0, row1, bytes_copy);
			memcpy(row1, temp, bytes_copy);
			row0 += bytes_copy;
			row1 += bytes_copy;
			bytes_left -= bytes_copy;
		}
	}
}

//==================================================================================================================================================
//__________________________________ TEXTURE BUFFER CLASS __________________________________________________________________________________________

UVBuffer::~UVBuffer()
{
#if _UV_DEBUG
    Print("Deleting UV Coords: ~ You have not set it up to delete it yet ~ " << ID);
#endif
//    delete(Image);
//    glDeleteBuffers(1, &ID);   
}
UVBuffer::UVBuffer(Vec2 *data,  GLsizei count)   ///UVBuffer::UVBuffer(Image &img, Vec2 *data,  GLsizei count) remove Image from UV buffer object as its seperate entity
             : ElementCount(count) 
{
    Data = new Vec2[count];
    memcpy(Data, data, sizeof(Vec2) * count);
 
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * count , data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void UVBuffer::Bind()
{
//    Picture.Bind();
//glEnableVertexAttribArray(TEXTURE_ATTRIB);
      glBindBuffer(GL_ARRAY_BUFFER, ID);      
//glVertexAttribPointer(TEXTURE_ATTRIB, 2, GL_FLOAT, GL_FALSE, 0,  (char *) NULL);
        glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}
void UVBuffer::Unbind()
{
      //  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableVertexAttribArray(TEXTURE_ATTRIB);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}


//   __forceinline void myInlineFunc() __attribute__((always_inline));









//======================================================================================================================================================================================
//                                         SHADER CLASS                                                                                                                                                   
//======================================================================================================================================================================================

// TODO: NEED A SHADER PARSER SO FRAGMENT AND VERTEX SHADER ARE THE SAME FILE
// Merge the two after ALL debugging is handled

GLuint Shader::VertexLocation  = 0;
GLuint Shader::ColorsLocation  = 0;
GLuint Shader::NormalsLocation = 0;


Shader::~Shader() {}
Shader::Shader()
{
	Print("Default Shader Constructor Called");
}
Shader::Shader(const char* vertpath, const char* fragpath)
	: m_Vertpath(vertpath), m_Fragpath(fragpath)
{
	ID = Load();
	ActiveShader.push_back(this);

	_GL(glUseProgram(ID));

	VertexLocation = glGetAttribLocation(Shader::GetActiveShader()->Name(), "VertexPosition");
	glEnableVertexAttribArray(VertexLocation);
	glVertexAttribPointer(VertexLocation, 3, GL_FLOAT, GL_FALSE, 0, (char *)NULL);

	NormalsLocation = glGetAttribLocation(Shader::GetActiveShader()->Name(), "VertexNormal");
	glEnableVertexAttribArray(NormalsLocation);
	glVertexAttribPointer(NormalsLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	ColorsLocation = glGetAttribLocation(Shader::GetActiveShader()->Name(), "VertexColor");
	glEnableVertexAttribArray(ColorsLocation);
    glVertexAttribPointer(ColorsLocation, 4, GL_FLOAT, GL_FALSE, 0, (char *)NULL);

	Enable();
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}
void Shader::Enable()
{
	if (GetActiveShader()->ID != this->ID)
	{
		glUseProgram(ID);
		for (auto &Uni : Uniforms)
		{
			switch (Uni.Type)
			{

			case(Float):
				SetUniform1f(Uni.Name, *(float*)Uni.Value);
				break;
			case(Integer):
				SetUniform1Int(Uni.Name, *(int*)Uni.Value);
				break;
			case(Vector2):
				SetUniform2f(Uni.Name, *(Vec2*)Uni.Value);
				break;
			case(Vector3):
				SetUniform3f(Uni.Name, *(Vec3*)Uni.Value);
				break;
			case(Vector4):
				SetUniform4f(Uni.Name, *(Vec4*)Uni.Value);
				break;
			case(Matrix4):
				SetUniformMat4(Uni.Name, *(Matrix*)Uni.Value);
				break;
			}
		}
	}
}
void Shader::Disable()
{
	glUseProgram(0);
}

GLuint Shader::Load()
{
	GLuint program = glCreateProgramObjectARB();
	GLuint vertex = glCreateShaderObjectARB(GL_VERTEX_SHADER);
	GLuint fragment = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

	std::string vertSourceString = FileUtils::read_file(m_Vertpath);
	std::string fragSourceString = FileUtils::read_file(m_Fragpath);

	const char* vertSource = vertSourceString.c_str();
	const char* fragSource = fragSourceString.c_str();

	glShaderSourceARB(vertex, 1, &vertSource, NULL);
	glCompileShaderARB(vertex);

	GLint result;
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetShaderInfoLog(vertex, length, &length, &error[0]);
		std::cout << "Failed to compile VertexShader: " << &error[0] << std::endl;
		glDeleteShader(vertex);
		EngineErrorResponse(0x10, fragment, (char*)m_Fragpath);
		return 0;
	}

	glShaderSourceARB(fragment, 1, &fragSource, NULL);
	glCompileShaderARB(fragment);
	_GL(glGetShaderiv(fragment, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetShaderInfoLog(fragment, length, &length, &error[0]);
		std::cout << "Failed to compile Fragment Shader:" << &error[0] << std::endl;
		glDeleteShader(fragment);
		EngineErrorResponse(0x11, fragment, (char*)m_Fragpath);
		return 0;
	}
	
#if	_OPENGL_FIXED_FUNCTION
#else
	//glBindAttribLocation(program, 0, "VertexPosition"); // The index passed into glBindAttribLocation is
	//glBindAttribLocation(program, 1, "VertexNormals");              
	//glBindAttribLocation(program, 2, "TextureCoords"); // used by glEnableVertexAttribArray. "position"   
	//glBindAttribLocation(program, 3, "VertexColor");
#endif
	glAttachObjectARB(program, vertex);
	glAttachObjectARB(program, fragment);

	_GL(glLinkProgramARB(program));
	_GL(glGetProgramiv(program, GL_LINK_STATUS, &result));
	if (result == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetProgramInfoLog(program, length, &length, &error[0]);
		std::cout << "Link Fail " << &error[0] << std::endl;
		glDeleteShader(program);
		EngineErrorResponse(0x12, program, (char*)m_Vertpath);
		return 0;
	}

	_GL(glValidateProgramARB(program));


	int param;
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &param);
	Print(param);
	glDetachShader(program, vertex);
	glDetachShader(program, fragment);
	glDeleteShader(vertex);
	_GL(glDeleteShader(fragment));
	return program;
}

void Shader::SetUniform1f(GLchar *name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}
void Shader::SetUniform1Int(GLchar *name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}
void Shader::SetUniform2f(GLchar *name, Vec2 &vector)
{
	glUniform2f(GetUniformLocation(name), vector.x, vector.y);
}
void Shader::SetUniform3f(GLchar *name, Vec3 &vector)
{
	_GL(glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z));
}
void Shader::SetUniform4f(GLchar *name, Vec4 &vector)
{
	glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetUniformMat4(GLchar *name, Matrix &matrix)
{
	//TODO: Find out what GLM equivalent to my elements in my Matrix class is.

	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}
void  Shader::SetTexture(GLchar *name, int slot)
{
	glUniform1i(GetUniformLocation(name), slot);
}
void Shader::AttachUniform(GLchar *name, Uniformtype type, void *variable)
{
	Uniforms.push_back(Uniform(type, name, variable));
}

GLint Shader::GetUniformLocation(GLchar *name)
{
	return glGetUniformLocation(ID, name);
}


//=======================================================================================================================================================
//======================================================================================================================================================================================
//                                                 CAMERA CLASS                                                                                                                                                 
//======================================================================================================================================================================================

Viewport *Viewport::Camera;

#if _OPENGL_FIXED_FUNCTION

Viewport::Viewport(Vec3 position, Vec3 rotation)
{
	FOV = 90.0;
	AspectRatio = 640.0f / 480.0f;
	Near = 0.1f;
	Far = 1000.0f;
	Speed = 2.0;

	Position = position;
	Rotation = rotation;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, AspectRatio, Near, Far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(Rotation.x, 1.0, 0, 0);
	glRotatef(Rotation.y, 0.0, 1.0, 0);
	glRotatef(Rotation.z, 0.0, 0, 1.0);
	glTranslatef(Position.x, Position.y, Position.z);
	Camera = this;

}
Matrix Viewport::GetModelViewMatrix()
{
	glGetFloatv(GL_MODELVIEW_MATRIX, &ViewMatrix[0][0]);
	return ViewMatrix;
}

void Viewport::MoveForward(float speed)
{
	Speed = speed;
	float  SIN_X = sin(RADIANS(Rotation.x - 90));
	Position.x -= (cos(RADIANS(Rotation.y - 90)) * SIN_X) * Speed;
	Position.y -= cos(RADIANS(Rotation.x - 90)) * speed;
	Position.z -= (sin(RADIANS(Rotation.y - 90)) * SIN_X) * Speed;
}
void Viewport::MoveBack(float speed)
{
	Speed = speed;
	float  SIN_X = sin(RADIANS(Rotation.x - 90));
	Position.x += (cos(RADIANS(Rotation.y - 90))* SIN_X) * Speed;
	Position.y += cos(RADIANS(Rotation.x - 90))* speed;
	Position.z += (sin(RADIANS(Rotation.y - 90))* SIN_X) * Speed;
}
void Viewport::MoveLeft(float speed)
{
	Speed = speed;
	Position.x += cos(RADIANS(Rotation.y - 0)) * Speed;
	Position.z += sin(RADIANS(Rotation.y - 0)) * Speed;
}
void Viewport::MoveRight(float speed)
{
	Speed = speed;
	Position.x += cos(RADIANS(Rotation.y - 180)) * Speed;
	Position.z += sin(RADIANS(Rotation.y - 180)) * Speed;
}
void Viewport::Rotate(float x, float y)
{
	Rotation.y += (x * .8);
	Rotation.x += (y * .8);
}
void Viewport::Update()
{
	//_GL(glMatrixMode(GL_MODELVIEW));
	glLoadIdentity();
	//  ROTATE the Camera  
	glRotatef(Rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(Rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(Rotation.z, 0.0f, 0.0f, 1.0f);
	//  Move Camera into Position       
	glTranslatef(Position.x, Position.y, Position.z);
}


#else  //  PROGRAMABLE PIPELINE

Viewport::Viewport(Vec3 position, Vec3 rotation)
{
	FOV = 45.0;
	AspectRatio = 640.0f / 480.0f;
	Near = 0.1f;
	Far = 1000.0f;

	Speed = 1.0;

	Position = position;
	Rotation = rotation;

	Forward = Vec3(0.0f, 0.0f, 1.0f);
	Right = Vec3(0.1f, 0.0f, 0.0f);
	Up = Vec3(0.0f, 1.0f, 0.0f);

	ProjectionMatrix = glm::perspective(glm::radians(FOV), AspectRatio, Near, Far);
	ViewMatrix = Matrix(1.0f);
	Camera = this;
}

void Viewport::Update()
{

	glm::mat4 Pitch = glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 Yaw = glm::rotate(glm::mat4(1.0f), Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Roll = glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    ViewMatrix = (Roll * Pitch *  Yaw) * (glm::translate(glm::mat4(1.0f), -Position));

	Forward = glm::normalize(Vec3(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]));
	Right = glm::normalize(Vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]));
}
Matrix Viewport::GetViewMatrix()
{
	ViewMatrix = glm::lookAt(Position, Position + Forward, Up);
	return ProjectionMatrix * ViewMatrix;
}

void Viewport::Rotate(float pitch, float yaw)
{
	Rotation.x -= yaw * .005;
	Rotation.y -= pitch * .008;
}
void Viewport::MoveForward(float speed)
{
	Position -= (Speed * Forward);
}
void Viewport::MoveBack(float speed)
{
	Position += (Speed * Forward);
}
void Viewport::MoveLeft(float speed)
{
	Position -= (speed)* Right;
}
void Viewport::MoveRight(float speed)
{
	Position += (speed)* Right;
}

void Viewport::ClampCamera()
{
	if (Rotation.x > 90)  Rotation.x = 90;
	if (Rotation.x < -90) Rotation.x = -90;
	if (Rotation.y < .0)    Rotation.y += 360.0f;
	if (Rotation.y> 360.0f)  Rotation.y -= 360.0f;
}
  #endif





//======================================================================================================================================================================================
//                                         PRIMATIVES TO TEST WITH                                                                                                                                                   
//======================================================================================================================================================================================



Block::Block(Vec3 pos, float size)
	:
	VertexCount(0)
{
	//f_TRACE(Print("Block Constructor"));
	Position = pos;

	Vec3 V[] =
	{
		Vec3(-size,-size, size), // FRONT    // 0                                             
		Vec3(size,-size, size),             // 1                                    
		Vec3(size, size, size),             // 2                                    
		Vec3(-size, size, size),             // 3                                    

		Vec3(size, size, size), // RIGHT    // 4    2                                        
		Vec3(size, size,-size),             // 5                                    
		Vec3(size,-size,-size),             // 6                                    
		Vec3(size,-size, size),             // 7    1                                

		Vec3(-size,-size,-size), // BACK     // 8                                            
		Vec3(size,-size,-size),             // 9    6                               
		Vec3(size, size,-size),             // 10   5                                
		Vec3(-size, size,-size),             // 11

		Vec3(-size,-size,-size), // LEFT     // 12   8                                        
		Vec3(-size,-size, size),             // 13   7                              
		Vec3(-size, size, size),             // 14   3                                
		Vec3(-size, size,-size),             // 15  11                                

		Vec3(size, size, size), // TOP      // 16   2                                      
		Vec3(-size, size, size),             // 17   3                                
		Vec3(-size, size,-size),             // 18   11                                
		Vec3(size, size,-size),             // 19   5                                

		Vec3(-size,-size,-size), // BOTTOM   // 20   8                                   
		Vec3(size,-size,-size),             // 21   9                          
		Vec3(size,-size, size),             // 22   1                          
		Vec3(-size,-size, size),             // 23   0                               
	};

	Vec2 UVs[] =
	{
		Vec2(0.00f, 0.66f),  // FRONT  // 0                                             
		Vec2(0.00f, 0.33f),            // 1                                             
		Vec2(0.25f, 0.33f),            // 2                                             
		Vec2(0.25f, 0.66f),            // 3                                             

		Vec2(0.25f, 0.33f),  // RIGHT  // 4                                             
		Vec2(0.50f, 0.33f),            // 5                                             
		Vec2(0.50f, 0.00f),            // 6                                             
		Vec2(0.25f, 0.00f),            // 7                                             

		Vec2(0.75f, 0.66f),  // BACK   // 8                                             
		Vec2(0.75f, 0.33f),            // 9                                             
		Vec2(0.50f, 0.33f),            // 10                                             
		Vec2(0.50f, 0.66f),            // 11

		Vec2(0.50f, 1.00f),  // LEFT   // 12                                             
		Vec2(0.25f, 1.00f),            // 13                                             
		Vec2(0.25f, 0.66f),            // 14                                             
		Vec2(0.50f, 0.66f),            // 15                                             

		Vec2(0.25f, 0.33f),  // TOP    // 16                                             
		Vec2(0.25f, 0.66f),            // 17                                             
		Vec2(0.50f, 0.66f),            // 18                                             
		Vec2(0.50f, 0.33f),            // 19     

		Vec2(0.75f, 0.66f),  // BOTTOM // 20                                             
		Vec2(0.75f, 0.33f),            // 21                                             
		Vec2(1.00f, 0.33f),            // 22                                             
		Vec2(1.00f, 0.66f),            // 23                 
	};
	Vec4 Cols[] =
	{
		Vec4(1.0, 1.0, 1.0, 1.0),  // FRONT  // 0         
		Vec4(0.0, 1.0, 1.0, 1.0),            // 1         
		Vec4(0.0, 0.5, 1.0, 1.0),            // 2         
		Vec4(0.0, 0.0, 0.0, 1.0),            // 3         

		Vec4(0.0, 0.0, 0.0, 1.0),  // RIGHT  // 4         
		Vec4(0.0, 0.0, 1.0, 1.0),            // 5         
		Vec4(0.0, 1.0, 1.0, 1.0),            // 6         
		Vec4(1.0, 1.0, 1.0, 1.0),            // 7         

		Vec4(1.0, 0.0, 1.0, 1.0),  // BACK   // 8         
		Vec4(0.0, 1.0, 0.0, 1.0),            // 9         
		Vec4(1.0, 1.0, 0.0, 1.0),            // 10        
		Vec4(1.0, 0.0, 0.0, 1.0),            // 11

		Vec4(0.50f, 1.00f, 1.00f, 1.0),  // LEFT   // 12        
		Vec4(0.25f, 1.00f, 1.00f, 1.0),            // 13        
		Vec4(0.25f, 0.66f, 0.66f, 1.0),            // 14        
		Vec4(0.50f, 0.66f, 0.66f, 1.0),            // 15        

		Vec4(0.25f, 0.33f, 0.33f, 1.0),  // TOP    // 16        
		Vec4(0.25f, 0.66f, 0.66f, 1.0),            // 17        
		Vec4(0.50f, 0.66f, 0.66f, 1.0),            // 18        
		Vec4(0.50f, 0.33f, 0.33f, 1.0),            // 19     

		Vec4(0.75f, 0.66f, 0.66f, 1.0),  // BOTTOM // 20        
		Vec4(0.75f, 0.33f, 0.33f, 1.0),            // 21        
		Vec4(1.00f, 0.33f, 0.33f, 1.0),            // 22        
		Vec4(1.00f, 0.66f, 0.66f, 1.0),            // 23        
	};
	GLuint Indexdata[] =
	{
		0,1,2,    0,2,3,     // FRONT
		6,5,4,    4,7,6,     // RIGHT 
		10,9,8,   8,11,10,   // BACK
		12,13,14, 12,14,15,  // LEFT
		18,17,16, 16,19,18,  // TOP
		20,21,22, 20,22,23   // BOTTOM
	};

	Vec3 NormalData[24];

	for_loop(Index, 24)
	{
		float x = V[Index].x;
		float y = V[Index].y;
		float z = V[Index].z;

		float  magnitude = sqrt(Squared(x) + Squared(y) + Squared(z));

		if (magnitude != 0)
		{
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}
		NormalData[Index] = Vec3(x, y, z); //  Vec3(RANDOM(2)-1,RANDOM(2)-1,RANDOM(2)-1);//
	}
	for_loop(Index, 24)
	{
		VertexList.push_back(V[Index]);
		//	TextureCoords.push_back(UVs[Index]);
		NormalList.push_back(NormalData[Index]);
	}
	for_loop(Index, 36)
	{
		IndexList.push_back(Indexdata[Index]);
	}

	// Averages all the normals so Each Faces Vertices have normals facing straight out to prevent interpolation of the light
	// Across the block

	// for(int Index = 0; Index < 24 ; Index += 4)
	// {
	//     Vec3 Norm =  NormalData[Index]   +
	//                  NormalData[Index+1] +
	//                  NormalData[Index+2] +
	//                  NormalData[Index+3];
	//
	//     Norm /= 4; 
	//
	//     NormalData[Index]   = Norm;
	//     NormalData[Index+1] = Norm;
	//     NormalData[Index+2] = Norm;
	//     NormalData[Index+3] = Norm;
	//
	// }
	//


	Vertices = new VertexBuffer(&VertexList[0], 24);
	Indices = new IndexBuffer(&IndexList[0], 36);
	Colors = new ColorBuffer(&Cols[0], 24);
	Normals = new NormalBuffer(&NormalList[0], 24);

	Polygons = new VAOBuffer();

	Polygons->Attach(Vertices);
	Polygons->Attach(Indices);
	Polygons->Attach(Normals);
	Polygons->Attach(Colors);

//	std::memcpy(&UV, &UVs, sizeof(UVs));
//	std::memcpy(&Indice, Indexdata, sizeof(Indexdata));
//	std::memcpy(&Norms, &NormalData, sizeof(NormalData));
//	std::memcpy(&Verts, &V, sizeof(V));


	Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
	Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
	Transform = glm::translate(Transform, Position);
	Rotation = Vec3(rand() % 360, rand() % 360, rand() % 360);

	// model_matrix = glm::translate(glm::rotate(glm::scale( mat4(1.0f), scaling), rotation_angle, rotation_axis), translation);
	// glm::mat4 myModelMatrix = myTranslationMatrix * myRotationMatrix * myScaleMatrix;
	// glm::vec4 myTransformedVector = myModelMatrix * myOriginalVector;
}
Sphere::Sphere(Vec3 pos, float radius, int sectors)
	:
	VertexCount(0),
	ColorCount(0),
	Radius(radius)
 
{
	Position = pos;
	Rotation = Vec3(rand() % 360, rand() % 360, rand() % 360);
	float size = 20;
	int IndexCount = 0;
	float  x = 0, y = 0, z = 0;
	float x1 = 0, y1 = 0, z1 = 0;
	float x2 = 0, y2 = 0, z2 = 0;
	float x3 = 0, y3 = 0, z3 = 0;
	std::vector<GLuint> Ind;
	for (float Long = 0;Long < 360;Long += size) {
		for (float Lat = 0;Lat < 180;Lat += size) {
			x = radius * (sin(RADIANS(Lat)) * cos(RADIANS(Long)));
			y = radius * (sin(RADIANS(Lat)) * sin(RADIANS(Long)));
			z = radius *  cos(RADIANS(Lat));

			x1 = radius * (sin(RADIANS(Lat + size)) * cos(RADIANS(Long)));
			y1 = radius * (sin(RADIANS(Lat + size)) * sin(RADIANS(Long)));
			z1 = radius *  cos(RADIANS(Lat + size));

			x2 = radius * (sin(RADIANS(Lat)) * cos(RADIANS(Long + size)));
			y2 = radius * (sin(RADIANS(Lat)) * sin(RADIANS(Long + size)));
			z2 = radius *  cos(RADIANS(Lat));

			x3 = radius * (sin(RADIANS(Lat + size)) * cos(RADIANS(Long + size)));
			y3 = radius * (sin(RADIANS(Lat + size)) * sin(RADIANS(Long + size)));
			z3 = radius *  cos(RADIANS(Lat + size));

			Colors[ColorCount].r = GL_Color(x * 255);
			Colors[ColorCount].g = GL_Color(y * 255);
			Colors[ColorCount].b = GL_Color(z * 255);

			Vertices[VertexCount].x = x;
			Vertices[VertexCount].y = y;
			Vertices[VertexCount].z = z;


			Colors[ColorCount + 1].r = GL_Color(x * 255);
			Colors[ColorCount + 1].g = GL_Color(y * 255);
			Colors[ColorCount + 1].b = GL_Color(z * 255);
			Vertices[VertexCount + 1].x = x1;
			Vertices[VertexCount + 1].y = y1;
			Vertices[VertexCount + 1].z = z1;

			Colors[ColorCount + 2].r = GL_Color(x * 255);
			Colors[ColorCount + 2].g = GL_Color(y * 255);
			Colors[ColorCount + 2].b = GL_Color(z * 255);
			Vertices[VertexCount + 2].x = x2;
			Vertices[VertexCount + 2].y = y2;
			Vertices[VertexCount + 2].z = z2;

			Colors[ColorCount + 3].r = GL_Color(x * 255);
			Colors[ColorCount + 3].g = GL_Color(y * 255);
			Colors[ColorCount + 3].b = GL_Color(z * 255);
			Vertices[VertexCount + 3].x = x3;
			Vertices[VertexCount + 3].y = y3;
			Vertices[VertexCount + 3].z = z3;


			Ind.push_back(VertexCount);
			Ind.push_back(VertexCount + 1);
			Ind.push_back(VertexCount + 2);

			Ind.push_back(VertexCount + 1);
			Ind.push_back(VertexCount + 3);
			Ind.push_back(VertexCount + 2);

			Indices[IndexCount] = VertexCount;
			Indices[IndexCount + 1] = VertexCount + 1;
			Indices[IndexCount + 2] = VertexCount + 2;

			Indices[IndexCount + 3] = VertexCount + 1;
			Indices[IndexCount + 4] = VertexCount + 3;
			Indices[IndexCount + 5] = VertexCount + 2;


			float  magnitude = sqrt(Squared(x) + Squared(y) + Squared(z));
			if (magnitude != 0)
			{
				x /= magnitude;
				y /= magnitude;
				z /= magnitude;
			}
			Normals[VertexCount].x = x;
			Normals[VertexCount].y = y;
			Normals[VertexCount].z = z;

			magnitude = sqrt(Squared(x1) + Squared(y1) + Squared(z1));
			if (magnitude != 0)
			{
				x1 /= magnitude;
				y1 /= magnitude;
				z1 /= magnitude;
			}
			Normals[VertexCount + 1].x = x1;
			Normals[VertexCount + 1].y = y1;
			Normals[VertexCount + 1].z = z1;

			magnitude = sqrt(Squared(x2) + Squared(y2) + Squared(z2));
			if (magnitude != 0)
			{
				x2 /= magnitude;
				y2 /= magnitude;
				z2 /= magnitude;
			}
			Normals[VertexCount + 2].x = x2;
			Normals[VertexCount + 2].y = y2;
			Normals[VertexCount + 2].z = z2;

			magnitude = sqrt(Squared(x3) + Squared(y3) + Squared(z3));
			if (magnitude != 0)
			{
				x3 /= magnitude;
				y3 /= magnitude;
				z3 /= magnitude;
			}
			Normals[VertexCount + 3].x = x3;
			Normals[VertexCount + 3].y = y3;
			Normals[VertexCount + 3].z = z3;


			VertexCount += 4;
			ColorCount += 4;
			IndexCount += 6;
		}
	}
	Polygons = new VAOBuffer();
	Polygons->Vertices = new VertexBuffer(Vertices, VertexCount);
	Polygons->Indices = new IndexBuffer(Indices, IndexCount);
	Polygons->Colors = new ColorBuffer(Colors, ColorCount);
	Polygons->Normals = new NormalBuffer(Normals, VertexCount);
 }


