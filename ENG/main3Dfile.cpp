/*Compile with
 g++ main.cpp -omain -framework OpenGL -framework GLUT -lfreeimage
for debug
Compile with
    g++ main.cpp -omain -framework OpenGL -framework GLUT -lfreeimage -o2 
for release
*/
#include <GLUT/glut.h>  // GLUT, include glu.h and gl.h
#include <GL/freeglut.h>
#include <math.h>
#include <OpenGL/gl3.h>

#define GLFW_INCLUDE_GL3  /* don't drag in legacy GL headers. */
#define GLFW_NO_GLU
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "phyiscs.h"
#include <thread>
#include <FreeImage.h>
 
float pyramidplace;

//Camera
Vec3 campos;
//Clock
clock_t t;
//Texture init
//list of texture ids
int texID[2];


//Aniso init
GLuint g_samplers;
GLfloat maxAniso = 4.0f;
//Creating a cube with Physics
BoxPhysicsBody3d Cube;
//Creating a list of cubes for use with collision
BoxPhysicsBody3d Boxes[1] = {Cube};

void engTranslatef(float x, float y, float z, BoxPhysicsBody3d box) {
   x += box.velocity.veloctiy3d.x - campos.x;
   y += box.velocity.veloctiy3d.y - campos.y;
   z += box.velocity.veloctiy3d.z + campos.z;
   glTranslatef(x, y, z);
}

void PhysicsUpdate() {
   Cube.velocity.veloctiy3d.y = -sin(pyramidplace);
   Cube.velocity.veloctiy3d.x += Cube.vec3.x;
   Cube.velocity.veloctiy3d.y += Cube.vec3.y;
   Cube.velocity.veloctiy3d.z += Cube.vec3.z;
   for(int i = 0; i < 1; i++) {
      for(int j = 0; j < 1; j++) {
         if(i != j) {
            DetectBoxCollision(Boxes[i], Boxes[j]);
         }
      }
   } 
}

//filenames for textures
char* textureFileNames[2] = {
   "3dpapaya.jpg",
   "8514731789_86e9777367_z.jpg"
};

static unsigned int CompileShader(unsigned int type, const std::string& source) {
   unsigned int id = glCreateShader(type);
   const char* src = source.c_str();
   glShaderSource(id, 1, &src, nullptr);
   glCompileShader(id);

   //Error Handeling(TO DO)

   return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
   unsigned int program = glCreateProgram();
   //Creates a vertex and fragment shader
   unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
   unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

   glAttachShader(program, vs);
   glAttachShader(program, fs);
   glLinkProgram(program);
   glValidateProgram(program);

   //already attached shader to program
   glDeleteShader(vs);
   glDeleteShader(fs);

   return program;
}

void processNormalKeys(unsigned char key, int x, int y) {
	if (key == 119){
      pyramidplace += 0.1;
      campos.z += 0.1;
   }
   else if (key == 87) {
      pyramidplace += 0.3;
      campos.z += 0.3;
   }
   if (key == 's') {
      campos.z -= 0.1;
   }
   else if (key == 'S') {
      campos.z -= 0.3;
   }
   if (key == 'a') {
      campos.x -= 0.1;
   }
   else if (key == 'A')
   {
      campos.x -= 0.3;
   }
   if (key == 'd') {
      campos.x += 0.1;
   }
   else if (key == 'D')
   {
      campos.x += 0.3;
   }
   
}

/* Initialize OpenGL Graphics */
void initGL() {           
   glEnable( GL_MULTISAMPLE );
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   //glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, decal);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   if(maxAniso != 0.0) {
      //Aniso set init
      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
      glGenSamplers(5, &g_samplers);

      //Aniso set
      glSamplerParameteri(g_samplers, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glSamplerParameteri(g_samplers, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glSamplerParameterf(g_samplers, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
   }

   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel (GL_SMOOTH);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
   glMatrixMode(GL_TEXTURE);  // Matrix mode for manipulating the texture transform matrix.
   glLoadIdentity();
   glScalef(1,-1,1); 
   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
 
/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
   t = clock();
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers
   glMatrixMode(GL_PROJECTION);     // To operate on model-view matrix
   glMatrixMode(GL_MODELVIEW);
   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix
   glEnable(GL_TEXTURE_2D); 
   engTranslatef(1.5f, 0, -6.0f, Cube);
   glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glutSolidCube(2);
    glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
    glDisable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, texID[-1]);

 
   // Render a pyramid consists of 4 triangles
   glLoadIdentity();
   glTranslatef(-1.5f, sin(pyramidplace), -6.0f);  // Move left and into the screen
 
   glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
      // Front
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f( 0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(1.0f, -1.0f, 1.0f);
 
      // Right
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f(0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(1.0f, -1.0f, 1.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(1.0f, -1.0f, -1.0f);
 
      // Back
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f(0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(1.0f, -1.0f, -1.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-1.0f, -1.0f, -1.0f);
 
      // Left
      glColor3f(1.0f,0.0f,0.0f);       // Red
      glVertex3f( 0.0f, 1.0f, 0.0f);
      glColor3f(0.0f,0.0f,1.0f);       // Blue
      glVertex3f(-1.0f,-1.0f,-1.0f);
      glColor3f(0.0f,1.0f,0.0f);       // Green
      glVertex3f(-1.0f,-1.0f, 1.0f);
   glEnd();   // Done drawing the pyramid
   
   PhysicsUpdate();
   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
   float deltatime = clock() - t;
   printf("%f fps\n", (CLOCKS_PER_SEC / deltatime));
}

void loadTextures() {
    int i;
    glGenTextures( 2, (GLuint *) texID );  // Get the texture object IDs.
    for (i = 0; i < 2; i++) {
        void* imgData;  // Pointer to image color data read from the file.
        int imgWidth;   // The width of the image that was read.
        int imgHeight;  // The height.
        FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(textureFileNames[i]);
        if (format == FIF_UNKNOWN) {
            printf("Unknown file type for texture image file %s\n", textureFileNames[i]);
            continue;
        }
        FIBITMAP* bitmap = FreeImage_Load(format, textureFileNames[i], 0);  // Read image from file.
        if (!bitmap) {
            printf("Failed to load image %s\n", textureFileNames[i]);
            continue;
        }
        FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap);  // Convert to RGB or BGR format
        FreeImage_Unload(bitmap);
        imgData = FreeImage_GetBits(bitmap2);     // Grab the data we need from the bitmap.
        imgWidth = FreeImage_GetWidth(bitmap2);
        imgHeight = FreeImage_GetHeight(bitmap2);
        if (imgData) {
            printf("Texture image loaded from file %s, size %dx%d\n", 
                                     textureFileNames[i], imgWidth, imgHeight);
            int format; // The format of the color data in memory, depends on platform.
            if ( FI_RGBA_RED == 0 )
               format = GL_RGB;
            else
               format = GL_BGR;
            glBindTexture( GL_TEXTURE_2D, texID[i] );  // Will load image data into texture object #i
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, format,
                                GL_UNSIGNED_BYTE, imgData);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Required since there are no mipmaps.
        }
        else {
            printf("Failed to get texture data from %s\n", textureFileNames[i]);
        }
    }
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
   glutInitWindowSize(2560, 1600);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow("3D");          // Create window with the given title
   glutIdleFunc (display);
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   loadTextures();
   initGL();                       // Our own OpenGL initialization
   glutKeyboardFunc(processNormalKeys);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

   char* genericVertexShader = 
      "#version 330 core/n"
      "layout(location = 0) in vec4 position;/n"
      "void main() {/n"
      "  glPositionc = position;/n"
      "}/n";
   char* genericFramgmentShader = 
      "#version 330 core/n"
      "layout(location = 0) out vec4 color;/n"
      "void main() {/n"
      "  color = vec4(NULL, NULL, NULL, NULL);/n"
      "}/n";   
   unsigned int shader = CreateShader(genericVertexShader, genericFramgmentShader);
   glUseProgram(shader);

   glutMainLoop();
   glutPostRedisplay();

   return 0;
}
