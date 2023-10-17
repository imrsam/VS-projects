 #include <GL/glut.h>

// Global variables
float playerX = 0.0f;
float playerY = 0.0f;
float gravity = 0.01f;
float jumpForce = 0.5f;  // Increased jump force
float platformWidth = 0.4f;
float platformHeight = 0.05f;
float platforms[5][2] = { { -0.3f, -0.8f }, { 0.6f, -0.6f }, { -0.8f, -0.4f }, { 0.3f, -0.2f }, { -0.6f, 0.0f } };
float groundY = -1.4f;
bool isJumping = false;
bool isMovingLeft = false;
bool isMovingRight = false;

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the player
    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(playerX - 0.1f, playerY - 0.1f, playerX + 0.1f, playerY + 0.1f);

    // Draw the platforms
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < 5; i++)
    {
        glRectf(platforms[i][0] - platformWidth / 2, platforms[i][1] - platformHeight / 2,
            platforms[i][0] + platformWidth / 2, platforms[i][1] + platformHeight / 2);
    }

    // Draw the ground surface
    glColor3f(0.4f, 0.4f, 0.4f);
    glRectf(-1.0f, groundY, 1.0f, groundY - 0.1f);

    glutSwapBuffers();
}

void update(int value)
{
    // Apply gravity
    playerY -= gravity;

    // Check if player is below the ground
    if (playerY - 0.1f <= groundY)
    {
        playerY = groundY + 0.1f;
        gravity = 0.0f;
        isJumping = false;
    }
    else
    {
        gravity = 0.01f;
        isJumping = true;
    }

    // Move player left
    if (isMovingLeft)
        playerX -= 0.01f;

    // Move player right
    if (isMovingRight)
        playerX += 0.01f;

    // Check for collision with platforms only when jumping
    if (isJumping) {
        for (int i = 0; i < 5; i++)
        {
            if (playerY - 0.1f <= platforms[i][1] + platformHeight / 2 &&
                playerY + 0.1f >= platforms[i][1] - platformHeight / 2 &&
                playerX + 0.1f >= platforms[i][0] - platformWidth / 2 &&
                playerX - 0.1f <= platforms[i][0] + platformWidth / 2)
            {
                playerY = platforms[i][1] + platformHeight / 2 + 0.1f;
                gravity = 0.0f;
                isJumping = false;
                break;
            }
        }
    }

    // Redraw the scene
    glutPostRedisplay();

    // Call update function again after 16 milliseconds (about 60 frames per second)
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    // Space key for jumping
    if (key == 32 && !isJumping)
    {
        playerY += jumpForce;
        isJumping = true;
    }
}

void specialKeyboard(int key, int x, int y)
{
    // Arrow keys for movement
    switch (key)
    {
    case GLUT_KEY_LEFT:
        isMovingLeft = true;
        break;
    case GLUT_KEY_RIGHT:
        isMovingRight = true;
        break;
    }
}

void specialKeyboardUp(int key, int x, int y)
{
    // Arrow keys release event
    switch (key)
    {
    case GLUT_KEY_LEFT:
        isMovingLeft = false;
        break;
    case GLUT_KEY_RIGHT:
        isMovingRight = false;
        break;
    }
}

int main(int argc, char** argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 600);
    glutCreateWindow(" Game");

    // Set up the OpenGL viewport
    glViewport(0, 0, 400, 600);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.5, 1.5, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    // Register callback functions
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard); // Register special keyboard function for arrow keys
    glutSpecialUpFunc(specialKeyboardUp); // Register special keyboard release function for arrow keys
    glutTimerFunc(0, update, 0);

    // Start the main loop
    glutMainLoop();

    return 0;
}