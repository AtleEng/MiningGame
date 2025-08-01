#include "gl_renderer.h"

// To load PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ################################     OpenGL Constants    ################################
const char *TEXTURE_PATH = "assets/textures/Texture_Atlas.png";

// ################################     OpenGL Structs    ################################
struct GLContext
{
    GLuint programID;
    GLuint textureID;
};
// ################################     OpenGL Globals    ################################
static GLContext glContext;

// ################################     OpenGL Functions    ################################
static void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                       GLsizei length, const GLchar *message, const void *user)
{
    if (severity == GL_DEBUG_SEVERITY_LOW ||
        severity == GL_DEBUG_SEVERITY_MEDIUM ||
        severity == GL_DEBUG_SEVERITY_HIGH)
    {
        SM_ASSERT(false, "OpenGL Error: %s", message);
    }
    else
    {
        SM_TRACE((char *)message);
    }
}

bool gl_init(BumpAllocator *transientStorage)
{
    load_gl_functions();

    glDebugMessageCallback(&gl_debug_callback, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);

    GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    int fileSize = 0;
    char *vertShader = read_file("assets/shaders/quad.vert", &fileSize, transientStorage);
    char *fragShader = read_file("assets/shaders/quad.frag", &fileSize, transientStorage);

    if (!vertShader || !fragShader)
    {
        SM_ASSERT(false, "Failed to load shaders");
        return false;
    }

    glShaderSource(vertShaderID, 1, &vertShader, 0);
    glShaderSource(fragShaderID, 1, &fragShader, 0);

    glCompileShader(vertShaderID);
    glCompileShader(fragShaderID);

    // Test vert shaders
    {
        int success;
        char shaderLog[2048] = {};

        glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertShaderID, 2048, 0, shaderLog);
            SM_ASSERT(false, "Failed to compile vertex shaders %s", shaderLog);
        }
    }
    // Test frag shaders
    {
        int success;
        char shaderLog[2048] = {};

        glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragShaderID, 2048, 0, shaderLog);
            SM_ASSERT(false, "Failed to compile fragment shaders %s", shaderLog);
        }
    }

    glContext.programID = glCreateProgram();
    glAttachShader(glContext.programID, vertShaderID);
    glAttachShader(glContext.programID, fragShaderID);
    glLinkProgram(glContext.programID);

    glDetachShader(glContext.programID, vertShaderID);
    glDetachShader(glContext.programID, fragShaderID);
    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);

    // This has to be done so GL will draw
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Texture Loading using STB image
    {
        int width, height, channels;
        char *data = (char *)stbi_load(TEXTURE_PATH, &width, &height, &channels, 4);
        if (!data)
        {
            SM_ASSERT(false, "Failed to load texture :(");
            return false;
        }

        glGenTextures(1, &glContext.textureID);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, glContext.textureID);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        // This setting only matters when using the GLSL texture() function
        // When you use texelFetch() this setting has no effect,
        // because texelFetch is designed for this purpose
        // See: https://interactiveimmersive.io/blog/glsl/glsl-data-tricks/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(0x809D);
    // Depth Testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);

    glUseProgram(glContext.programID);

    return true;
}

void gl_render()
{
    glClearColor(119.0f / 255.0f, 33.0f / 255.0f, 111.0f / 255.0f, 1.0f);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, input.screenSizeX, input.screenSizeY);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}