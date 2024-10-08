#include "application.h"

voxey::core::Application::Application(int argc, char** argv[]) : mWindow(nullptr)
{
	std::cout << "argc: " << argc << " argv: " << argv << std::endl;
}

voxey::core::Application::~Application()
{
	if (mWindow)
	{
		glfwDestroyWindow(mWindow);
		mWindow = nullptr;
	}
	glfwTerminate();
}

int voxey::core::Application::run()
{
	std::cout << "running application" << std::endl;
	char buffer[256];

	if (_getcwd(buffer, sizeof(buffer)) == NULL)
	{
		std::cerr << "error getting current working directory" << std::endl;
		return EXIT_FAILURE;
	}
	else
	{
		std::cout << "current working directory: " << buffer << std::endl;
	}

	mLogger = std::make_unique<voxey::core::Logger>();
	mLogger->run();

	if (!glfwInit())
	{
		std::cout << "error init glfw" << std::endl;
		return EXIT_FAILURE;
	}

	if (createWindow() != EXIT_SUCCESS)
	{
		std::cout << "error init glfw window" << std::endl;
		return EXIT_FAILURE;
	}

	mEngine = std::make_unique<voxey::core::Engine>(getWindow());
	return mEngine->run();
}

GLFWwindow* voxey::core::Application::getWindow()
{
	return mWindow;
}

int voxey::core::Application::createWindow()
{
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	const auto primaryMonitor = glfwGetPrimaryMonitor();
	const auto primaryMonitorVideoMode = glfwGetVideoMode(primaryMonitor);

	const auto screenWidth = primaryMonitorVideoMode->width;
	const auto screenHeight = primaryMonitorVideoMode->height;

	const char* title = "voxey";
	unsigned int windowWidth = 1280;
	unsigned int windowHeight = 720;

	mWindow = glfwCreateWindow(windowWidth, windowHeight, title, nullptr, nullptr);
	if (!mWindow)
	{
		return EXIT_FAILURE;
	}

	glfwSetWindowPos(mWindow, screenWidth / 2 - windowWidth / 2, screenHeight / 2 - windowHeight / 2);
	glfwMakeContextCurrent(mWindow);
	glfwSetFramebufferSizeCallback(mWindow, frameBufferCallback);

	mInputManager = std::make_unique<voxey::input::Input>();
	mInputManager->init(mWindow, mInputManager.get());

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "error init glad";
		return EXIT_FAILURE;
	}

	setupDefaultKeybinds();

	if (debugMode)
	{
		setupDebugKeybinds();
	}


	return EXIT_SUCCESS;
}

void voxey::core::Application::frameBufferCallback(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

void voxey::core::Application::setupDefaultKeybinds()
{
	voxey::input::Input::registerAction("ESC", GLFW_KEY_ESCAPE);
	voxey::input::Input::registerAction("W", GLFW_KEY_W);
	voxey::input::Input::registerAction("S", GLFW_KEY_S);
	voxey::input::Input::registerAction("A", GLFW_KEY_A);
	voxey::input::Input::registerAction("D", GLFW_KEY_D);
	voxey::input::Input::registerAction("SPACE", GLFW_KEY_SPACE);
	voxey::input::Input::registerAction("LEFT_SHIFT", GLFW_KEY_LEFT_SHIFT);
}

void voxey::core::Application::setupDebugKeybinds()
{
	std::cout << "debugMode keybinds" << std::endl;

	voxey::input::Input::registerAction("DEBUG_POLYGON_MODE", GLFW_KEY_T);
}
