#include "common.h"
#include "global.h"
#include "game_map.h"
#include "Player.h"

constexpr GLsizei WINDOW_WIDTH = 640, WINDOW_HEIGHT = 640;
Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

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

void processPlayerMovement(Player &player, GameMap &map, Game &game)
{
  //std::cout << map.screen_next_lvl << std::endl;
  if (Input.keys[GLFW_KEY_W])
    player.ProcessInput(MovementDir::UP, map);
  else if (Input.keys[GLFW_KEY_S])
    player.ProcessInput(MovementDir::DOWN, map);
  if (Input.keys[GLFW_KEY_A])
    player.ProcessInput(MovementDir::LEFT, map);
  else if (Input.keys[GLFW_KEY_D])
    player.ProcessInput(MovementDir::RIGHT, map);
  if (Input.keys[GLFW_KEY_ENTER] && game.level != 1)
    map.screen_next_lvl = false;
  if (Input.keys[GLFW_KEY_R])
  {
    game.screen_load = true;
    map.screen_dead = false;
    map.screen_next_lvl = false;
  }
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

	//std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	//std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	//std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	//std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "R - to retry level" << std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

void ScreenHandler(GLFWwindow *window_main, Game &game, Image &screen, Player &player, bool &cond)
{
  while (cond && !glfwWindowShouldClose(window_main))
  {
    glfwPollEvents();
    processPlayerMovement(player, game.Maps[game.level], game);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screen.Data()); GL_CHECK_ERRORS;

		glfwSwapBuffers(window_main);
  }
}

void SpikeHandler(Game &game, Image &screen)
{
  if (game.timer == 0)
    game.Update(game.Maps[game.level], screen, 0);
  if (game.timer == 110)
    game.Update(game.Maps[game.level], screen, 1);
  if (game.timer == 115)
    game.Update(game.Maps[game.level], screen, 2);
  if (game.timer == 120)
    game.Update(game.Maps[game.level], screen, 3);
  if (game.timer == 230)
    game.Update(game.Maps[game.level], screen, 2);
  if (game.timer == 235)
    game.Update(game.Maps[game.level], screen, 1);
  if (game.timer > 120 && game.timer < 230 && game.timer % 10 == 0)
    game.Update(game.Maps[game.level], screen, -1);
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window_main = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dungeon master", nullptr, nullptr);
	if (window_main == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window_main); 

	glfwSetKeyCallback        (window_main, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window_main, OnMouseMove); 
  glfwSetMouseButtonCallback(window_main, OnMouseButtonClicked);
	glfwSetScrollCallback     (window_main, OnMouseScroll);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

  std::vector<Image> spike_anim;
  spike_anim.push_back(Image("resources/frames/floor_spikes_anim_f0.png"));
  spike_anim.push_back(Image("resources/frames/floor_spikes_anim_f1.png"));
  spike_anim.push_back(Image("resources/frames/floor_spikes_anim_f2.png"));
  spike_anim.push_back(Image("resources/frames/floor_spikes_anim_f3.png"));
  spike_anim.push_back(Image("resources/frames/glitters_f0.png"));
  spike_anim.push_back(Image("resources/frames/glitters_f1.png"));
  
	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
  Image backup(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
  Image lvl_end("resources/lvl_complete.png");
  Image death("resources/death_screen.png");
  Image game_end("resources/game_complete.png");

  GameMap lvl1;
  lvl1.loadFile("lvl1.txt", &lvl1.player_start, spike_anim);
  lvl1.backup = backup;
  GameMap lvl2;
  lvl2.loadFile("lvl2.txt", &lvl2.player_start, spike_anim);
  lvl2.backup = backup;

  Game game;
  game.Maps.push_back(lvl1);
  game.Maps.push_back(lvl2);

  //Player enemy{{WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 30}};
  Player player{};

  std::vector<Image> player_idle;
  player_idle.push_back(Image("resources/frames/knight_m_idle_anim_f0.png"));
  player_idle.push_back(Image("resources/frames/knight_m_idle_anim_f1.png"));
  player_idle.push_back(Image("resources/frames/knight_m_idle_anim_f2.png"));
  player_idle.push_back(Image("resources/frames/knight_m_idle_anim_f3.png"));
  int i = 0;

  /*std::vector<std::string> en_path;
  en_path.push_back("resources/frames/big_demon_idle_anim_f0.png");
  en_path.push_back("resources/frames/big_demon_idle_anim_f1.png");
  en_path.push_back("resources/frames/big_demon_idle_anim_f2.png");
  en_path.push_back("resources/frames/big_demon_idle_anim_f3.png");*/

  /*player.mv_path.push_back("resources/frames/knight_m_run_anim_f0.png");
  player.mv_path.push_back("resources/frames/knight_m_run_anim_f1.png");
  player.mv_path.push_back("resources/frames/knight_m_run_anim_f2.png");
  player.mv_path.push_back("resources/frames/knight_m_run_anim_f3.png");*/

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

  //game loop
	while (!glfwWindowShouldClose(window_main))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
    //std::cout << "fps:" << 1 / deltaTime << std::endl;
    if (game.screen_load)
    {
      game.Render(game.Maps[game.level], screenBuffer);
      player.coords.x = game.Maps[game.level].player_start.x;
      player.coords.y = game.Maps[game.level].player_start.y;
      
      memcpy(game.Maps[game.level].backup.Data(), screenBuffer.Data(), sizeof(Pixel) * 256 * 40 * 40);
      game.screen_load = false;
    }
    else if (game.Maps[game.level].screen_next_lvl)
    {
      if (game.level == 1)
      {
        ScreenHandler(window_main, game, game_end, player, game.Maps[game.level].screen_next_lvl);
        game.level = 0;
        game.screen_load = true;
      }
      else
      {
        ScreenHandler(window_main, game, lvl_end, player, game.Maps[game.level].screen_next_lvl);
        if (!game.screen_load)
        {
          game.level++;
          game.screen_load = true;
        }
      }
    }
    else if (game.Maps[game.level].screen_dead) 
      ScreenHandler(window_main, game, death, player, game.Maps[game.level].screen_dead);
    else
    {
      glfwPollEvents();
      processPlayerMovement(player, game.Maps[game.level], game);
      
      SpikeHandler(game, screenBuffer);
      player.CheckPlayer(game.Maps[game.level]);
      if (game.timer % 6 == 0 && game.timer)
      {
        memcpy(player.Data(), player_idle[i].Data(), sizeof(Pixel) * player.Height() * player.Width());
        //enemy.Read_img(en_path[i]);
        i++;
      }
      game.timer++;
      
      player.Draw(screenBuffer, backup);
      //enemy.Draw(screenBuffer, backup);

		  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
      glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

		  glfwSwapBuffers(window_main);
      if (i == 4)
        i = 0;
      if (game.timer == 240)
        game.timer = 0;
    }
	}

	glfwTerminate();
	return 0;
}
