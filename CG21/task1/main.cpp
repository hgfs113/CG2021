#include "common.h"
#include "Image.h"
#include "Player.h"
#include "map.h"
#include <iostream>
#include <GL/glut.h>
#include <cstring>
#include <cstdlib>

#define GLFW_DLL
#include <GLFW/glfw3.h>

constexpr GLsizei WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int X_, Y_;

CellType* cm;
bool* collisionM;

void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

void processPlayerMovement(Player &player)
{
  if (Input.keys[GLFW_KEY_W])
    player.ProcessInput(MovementDir::UP, cm, collisionM);
  else if (Input.keys[GLFW_KEY_S])
    player.ProcessInput(MovementDir::DOWN, cm, collisionM);
  if (Input.keys[GLFW_KEY_A])
    player.ProcessInput(MovementDir::LEFT, cm, collisionM);
  else if (Input.keys[GLFW_KEY_D])
    player.ProcessInput(MovementDir::RIGHT, cm, collisionM);
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
  X_ = xpos;
  Y_ = ypos;
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "escape from a. samoilov", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 

	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
  glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	Point starting_pos{.x = WINDOW_WIDTH / 3, .y = WINDOW_HEIGHT / 3};
	Player player{starting_pos};

	Point starting_pos1{.x = 64, .y = 64};
  Point map_corner{ .x = 568, .y = 568};
	Enemy enemy_1{starting_pos1};
  Point starting_pos2{.x = 200, .y = 64};
	Enemy enemy_2{starting_pos2};
  Point starting_pos3{.x = 64, .y = 200};
	Enemy enemy_3{starting_pos3};
	//Point starting_pos3{.x = 64, .y = 434};
	//Enemy enemy_2{starting_pos3};

  Image ground_ ("../resources/ground_a.png");
  Image space_("../resources/space_tex.png");
  Image wall_("../resources/walls_a.png");
  Image exit_("../resources/exit_tex.png");
  

	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

  Image enemy("../resources/enemy.png");
  Image animation("../resources/attack.png");

  Image character("../resources/megu.png");

  Image avatar("../resources/alive.png");
  Image dead_avatar("../resources/dead.png");
  Image lives_bar("../resources/Live Bar.png");
  Image lives("../resources/SmallHeart.png");
  Image lives_anim("../resources/SmallHeartHit.png");
//  std::cout<<avatar.Width()<<avatar.Height() <<std::endl;

  Image *arr = new Image[10];
  for(int i = 0; i < 10; ++i)
  {
    std::string str_1 = "../resources/animations/00";
    if (i < 9)
      str_1 += "0";
    str_1 += std::to_string(i + 1) +".png";
    Image im(str_1);
    arr[i] = im;
  }
  
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

  cm = readMap("map.txt", &collisionM);
  backScene scene;

  for (int x = 0; x < WINDOW_WIDTH; x++)
    for (int y = 0; y < WINDOW_HEIGHT; y++)
      screenBuffer.PutPixel(x, y, backgroundColor);
  int anim = 0;
  float anim_time = 0;
  //game loop
	while (!glfwWindowShouldClose(window))
	{
    if (anim)
    {
      anim_time += deltaTime;
      if (anim_time > 0.1)
      {
        anim_time = 0;
        anim--;
      }
    }
    if (enemy_1.finish)
    {
      anim += 2;
      enemy_1.finish = false;
      player.touch_enemy();
    }
    if (enemy_2.finish)
    {
      anim += 2;
      enemy_2.finish = false;
      player.touch_enemy();
    }
    if (enemy_3.finish)
    {
      anim += 2;
      enemy_3.finish = false;
      player.touch_enemy();
    }
    if (player.get_lives() <= 0)
      avatar = dead_avatar;
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
    glfwPollEvents();
    //std::cout<< 1/ deltaTime <<std::endl;
    if ((Input.keys[GLFW_KEY_R]) && (!player.attack))
    {
      player.xpos_fixed = X_;
      player.ypos_fixed = WINDOW_HEIGHT - Y_;
      float l2 = sqrt(pow(player.get_x() + 16 - X_, 2) + pow(player.get_y() + 16  - WINDOW_HEIGHT + Y_, 2));
      std::cout<<l2<<std::endl;
      if (l2 < 80)
        player.attack =  true;
    }
    processPlayerMovement(player);
    scene.Draw(screenBuffer, ground_, wall_, space_, exit_, cm, 0, 0);
    player.Draw(character, screenBuffer, deltaTime);
    enemy_1.Draw(enemy, animation, screenBuffer, deltaTime, player.get_coords(), collisionM);
    enemy_2.Draw(enemy, animation, screenBuffer, deltaTime, player.get_coords(), collisionM);
    enemy_3.Draw(enemy, animation, screenBuffer, deltaTime, player.get_coords(), collisionM);
    player.ProcessAnimation(screenBuffer, arr, cm, collisionM, deltaTime, enemy_1, enemy_2, enemy_3);
  //  enemy_2.Draw(enemy, screenBuffer, deltaTime, player.get_coords(), collisionM);
    layer_Draw(screenBuffer, avatar, lives_bar, lives, lives_anim, anim, 568, 64, player.get_lives());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

    glFlush();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
  delete[]cm;
  delete[]collisionM;
	return 0;
}
