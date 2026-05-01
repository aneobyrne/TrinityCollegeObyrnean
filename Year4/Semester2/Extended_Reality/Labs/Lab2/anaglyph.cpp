#include <glad/gl.h> // Loads OpenGL functions
#include <GLFW/glfw3.h> // Creates Window and handles keyboard/mouse

// Math library for graphics
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> // Gives vectors (vec3) and matrices (mat4)

// Classess
#include <render/shader.h> 
#include <render/texture.h>
#include <models/box.h>

// C++ Standard Library
#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

// Window set up
static GLFWwindow *window;
static int windowWidth = 1024; 
static int windowHeight = 768;

// Function declarations for keyboard and mouse input
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// OpenGL camera view parameters
static glm::vec3 originalEyeCenter(0, 0, 100);
static glm::vec3 eyeCenter = originalEyeCenter; // where the camera is
static glm::vec3 lookat(0, 0, 0); // what the camera looks at
static glm::vec3 up(0, 1, 0); // which way is up

// Perspective camera settings (objects outside this range get clipped)
static glm::float32 FoV = 45; // field of view (zoom)
static glm::float32 zNear = 0.1f;  // what distances are visible
static glm::float32 zFar = 1000.0f; // what distances are visible

// View control (camera movement)
static float viewAzimuth = M_PI / 2;
static float viewPolar = M_PI / 2;
static float viewDistance = 100.0f;
static bool rotating = false;

// Scene control 
static int numBoxes = 1;                // Debug: set numBoxes to 1.
std::vector<glm::mat4> boxTransforms;   // We represent the scene by a single box and a number of transforms for drawing the box at different locations.

// Anaglyph control 
static float ipd = 10.0f;                // Distance between left/right eye.

// After you implement the anaglyph, adjust the IPD value to control the red/cyan offsets and depth perception. 
enum AnaglyphMode {
    None,
    ToeIn, 
    Asymmetric, 
    AnaglyphModeCount,
};

static std::string strAnaglyphMode[] = {
    "None", 
    "Toe-in", 
    "Asymmetric view frustum", 
    "Invalid",
};
static AnaglyphMode anaglyphMode = AnaglyphMode::None; // current mode (None)

// Helper functions 
static void nextAnaglyphMode() { // cycles through modes when you press M
    anaglyphMode = (AnaglyphMode)(((int)anaglyphMode + 1) % (int)AnaglyphModeCount);
}

static int randomInt() { // scatter boxes randomly
    return rand();
}

static float randomFloat() { // scatter boxes randomly
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return r;
}

static glm::vec3 randomVec3() { // scatter boxes randomly
    return glm::vec3(randomFloat(), randomFloat(), randomFloat());
}

// Scene generation, one box is 1 else if 0 many boxes
static void generateScene() {
    boxTransforms.clear();
    if (numBoxes == 1) {
        // Use this for debugging
        glm::mat4 modelMatrix = glm::mat4();
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(16, 16, 16));
        boxTransforms.push_back(modelMatrix);
    } else {
        // Generate boxes based on random position, rotation, and scale. 
        // Store their transforms.
        for (int i = 0; i < numBoxes; ++i) {
            glm::vec3 position = 100.0f * (randomVec3() - 0.5f);
            float s = (1 + (randomInt() % 4)) * 1.0f;
            glm::vec3 scale(s, s, s);
            float angle = randomFloat() * M_PI * 2;
            glm::vec3 axis = glm::normalize(randomVec3() - 0.5f);
            glm::mat4 modelMatrix = glm::mat4();
            modelMatrix = glm::translate(modelMatrix, position);
            modelMatrix = glm::rotate(modelMatrix, angle, axis);
            modelMatrix = glm::scale(modelMatrix, scale);
            boxTransforms.push_back(modelMatrix);
        }
    }
}

// Debugging functions (prints in terminal)
static void printAnaglyphMode() {
    std::cout << "Anaglyph mode: " << strAnaglyphMode[(int)anaglyphMode] << std::endl;
}

static void printVec3(glm::vec3 v) {
    std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}

static void printMat4(glm::mat4 m) {
    // Column major
    std::cout << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << std::endl;
    std::cout << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << std::endl;
    std::cout << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << std::endl;
    std::cout << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << std::endl;
}

int main(void)
{
    // Initialise GLFW (sets OpenGL version 3.3 core)
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(windowWidth, windowHeight, "Anaglyph Rendering", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to open a GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(window, key_callback);

    // Ensure we can capture mouse cursor movement 
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cerr << "Failed to initialize OpenGL context." << std::endl;
        return -1;
    }
    srand(2024);

    // Background
    glClearColor(163 / 255.0f, 227 / 255.0f, 255 / 255.0f, 1.0f);
    glEnable(GL_DEPTH_TEST); // objects block each other correctly
    glEnable(GL_CULL_FACE); // dont draw back faces (performance)

    // Create a box
    Box box;
    box.initialize("../../src/box.vert", "../../src/box.frag", "../../src/facade4.jpg");

    // Create the scene with a set of boxes represented by their transforms
    generateScene();

    // Set a perspective camera (converts 3D to 2D screen)
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(FoV), (float)windowWidth / windowHeight, zNear, zFar);
    printAnaglyphMode();

    // MODES:
    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // TODO: Render anaglyph 
        // --------------------------------------------------------------------
        if (anaglyphMode == None) { // normal rendering (scene ones and all colors enables and single camera)

            // Clear the screen
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set camera view matrix 
            glm::mat4 viewMatrix = glm::lookAt(eyeCenter, lookat, up);
            glm::mat4 vp = projectionMatrix * viewMatrix;
            
            // Draw 
            for (int i = 0; i < numBoxes; ++i) {
                box.render(vp, boxTransforms[i]);
            }

        } else {
            glm::mat4 vpLeft;
            glm::mat4 vpRight;

            if (anaglyphMode == ToeIn) { // Moves eyes aprt by IPD, rotates each eye inward and same matrix for both
                // Camera basis
                glm::vec3 forward = glm::normalize(lookat - eyeCenter);
                glm::vec3 right = glm::normalize(glm::cross(forward, up));

                // Eye positions
                glm::vec3 leftEye  = eyeCenter - (0.5f * ipd * right);
                glm::vec3 rightEye = eyeCenter + (0.5f * ipd * right);

                // Toe-in angle (small!)
                float toeAngle = 0.02f;

                // Each eye looks inward
                glm::vec3 leftTarget  = lookat + toeAngle * right;
                glm::vec3 rightTarget = lookat - toeAngle * right;
                
                // View matrices
                glm::mat4 viewLeft  = glm::lookAt(leftEye,  leftTarget,  up);
                glm::mat4 viewRight = glm::lookAt(rightEye, rightTarget, up);
                
                vpLeft = projectionMatrix * viewLeft;
                vpRight = projectionMatrix * viewRight;
                // TODO: Implement the toe-in projection here
                // ------------------------------------------------------------
            
            } else if (anaglyphMode == Asymmetric) { // Moves eyes apart, uses parallel cameras and SHIFTS the frustum for each eye
                // TODO: Implement the asymmetric view frustum here
                // ------------------------------------------------------------
                glm::vec3 forward = glm::normalize(lookat - eyeCenter);
                glm::vec3 right = glm::normalize(glm::cross(forward, up));
                glm::vec3 leftEye = eyeCenter - (0.5f * ipd * right);
                glm::vec3 rightEye = eyeCenter + (0.5f * ipd * right);
                
                float aspect = (float)windowWidth / windowHeight;
                float top = zNear * tan(glm::radians(FoV * 0.5f));
                float bottom = -top;
                float rightF = top * aspect;
                float leftF = -rightF;
                float frustumShift = (ipd * 0.5f) * zNear / viewDistance;
                
                glm::mat4 projLeft = glm::frustum(
                    leftF + frustumShift,
                    rightF + frustumShift,
                    bottom,
                    top,
                    zNear,
                    zFar
                );

                glm::mat4 projRight = glm::frustum(
                    leftF - frustumShift,
                    rightF - frustumShift,
                    bottom,
                    top,
                    zNear,
                    zFar
                );

                glm::mat4 viewLeft = glm::lookAt(leftEye, leftEye + forward, up);
                glm::mat4 viewRight = glm::lookAt(rightEye, rightEye + forward, up);
                vpLeft = projLeft * viewLeft;
                vpRight = projRight * viewRight;
            }
            // TODO: Implement two-pass rendering to draw the anaglyph
            // ----------------------------------------------------------------
            // Two pass: Draws scene twice, giving each a complementary color
            // Left eye pass (red channel)
            glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
            glClear(GL_DEPTH_BUFFER_BIT); 
            for (int i = 0; i < numBoxes; ++i)
                box.render(vpLeft, boxTransforms[i]);

            // Right eye pass (cyan channel)
            glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
            glClear(GL_DEPTH_BUFFER_BIT);
            for (int i = 0; i < numBoxes; ++i)
                box.render(vpRight, boxTransforms[i]);
            
            // Restore channels
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            
        }
        // --------------------------------------------------------------------

        // Animation
        static double lastTime = glfwGetTime();
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        if (rotating) { // rotates camera smoothly over time
            viewAzimuth += 1.0f * deltaTime;
            eyeCenter.x = viewDistance * cos(viewAzimuth);
            eyeCenter.z = viewDistance * sin(viewAzimuth);
        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } 

    // Check if the ESC key was pressed or the window was closed
    while (!glfwWindowShouldClose(window));
    // Clean up
    box.cleanup();
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        std::cout << "Space key is pressed." << std::endl;
        rotating = !rotating;
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        std::cout << "Reset." << std::endl;
        rotating = false;
        eyeCenter = originalEyeCenter;
        viewAzimuth = M_PI / 2;
        viewPolar = M_PI / 2;
    }
    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        viewPolar -= 0.1f;
        eyeCenter.y = viewDistance * cos(viewPolar);
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        viewPolar += 0.1f;
        eyeCenter.y = viewDistance * cos(viewPolar);
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        viewAzimuth -= 0.1f;
        eyeCenter.x = viewDistance * cos(viewAzimuth);
        eyeCenter.z = viewDistance * sin(viewAzimuth);
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        viewAzimuth += 0.1f;
        eyeCenter.x = viewDistance * cos(viewAzimuth);
        eyeCenter.z = viewDistance * sin(viewAzimuth);
    }
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        nextAnaglyphMode(); 
        printAnaglyphMode();
    }
    // Adjust the IPD value to match your actual viewing distance
    // Special case: IPD == 0 means no 3D effect.
    if (key == GLFW_KEY_COMMA) {
        ipd -= 0.1f;
        ipd = std::max(ipd, 0.0f);
        std::cout << "IPD: " << ipd << std::endl;
    }
    if (key == GLFW_KEY_PERIOD) {
        ipd += 0.1f;
        std::cout << "IPD: " << ipd << std::endl;
    }
    if (key == GLFW_KEY_1) {
        numBoxes = 1;
        generateScene();
    }
    if (key == GLFW_KEY_0) {
        numBoxes = 100;
        generateScene();
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	
	//HEART
	if (key == GLFW_KEY_2){
		numBoxes = 100;
		boxTransforms.clear();

		float scale = 1.0f;
		float zDepth = 0.0f;

		for (int i = 0; i < numBoxes; ++i) {
			float t = i * 2.0f * M_PI / numBoxes;
			float x = scale * 16.0f * pow(sin(t), 3);
			float y = scale * (13.0f * cos(t) - 5.0f * cos(2*t) - 2.0f * cos(3*t) - cos(4*t));
            float z = zDepth; 

            glm::mat4 modelMatrix = glm::mat4();
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f)); 
            boxTransforms.push_back(modelMatrix);
		}
	}
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    // Optionally, you can implement your own mouse support.
}

