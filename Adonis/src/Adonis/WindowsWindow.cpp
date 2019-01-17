#include "pch.h"
#include "WindowsWindow.h"

#include "Adonis/Eventsystem/EventManager.h"
#include "Adonis/Eventsystem/Events/Events.h"


namespace Adonis {

	static bool s_glfw_initialized = false;

	static void GLFWErrorCallback(int error, const char* desc) {
		AD_CORE_ERROR("GLFW reported error: {0} with description: {1}", error, desc);
	}

	std::unique_ptr<IWindow> IWindow::create(const uint16_t& width, const uint16_t& height, const std::string& title, WindowMode mode, const bool& vsync) {
		return std::make_unique<WindowsWindow>(width, height, title, mode, vsync);
	}

	WindowsWindow::WindowsWindow(const uint16_t& width, const uint16_t& height, const std::string& title, WindowMode mode, const bool& vsync) :
		m_width(width),
		m_height(height),
		m_title(title),
		m_mode(mode),
		m_vsync(vsync){				
		init();
	}

	WindowsWindow::~WindowsWindow() {
	}

	void WindowsWindow::init() {
		
		if (!s_glfw_initialized) {
			int status = glfwInit();
			AD_CORE_ASSERT(status, "Failed to initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_glfw_initialized = true;
		}

		switch (m_mode) {
		case WindowMode::Windowed:
			m_window = std::unique_ptr<GLFWwindow, void(*)(GLFWwindow *)>(glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr), WindowsWindow::shutdown);
			break;
		case WindowMode::Borderless:
		{
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* vid_mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, vid_mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, vid_mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, vid_mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, vid_mode->refreshRate);

			m_window = std::unique_ptr<GLFWwindow, void(*)(GLFWwindow *)>(glfwCreateWindow(vid_mode->width, vid_mode->height, m_title.c_str(), monitor, nullptr), glfwDestroyWindow);
		}
			break;
		case WindowMode::Fullscreen:
			m_window = std::unique_ptr<GLFWwindow, void(*)(GLFWwindow *)>(glfwCreateWindow(m_width, m_height, m_title.c_str(), glfwGetPrimaryMonitor(), nullptr), glfwDestroyWindow);
			break;
		default:break;
		}

		//Set member variables
		glfwGetFramebufferSize(m_window.get(), &m_framebuffer_width, &m_framebuffer_height);

		glfwMakeContextCurrent(m_window.get());
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AD_CORE_ASSERT(status, "Failed to initialize GLAD");

		//set vsync
		glfwSwapInterval(m_vsync);
		
		glfwSetKeyCallback(m_window.get(), [] (GLFWwindow* window, int key, int scancode, int action, int mods){
			switch (action) {
			case GLFW_PRESS:
				EventManager::queueEvent<KeyPressed>(key, mods);
				break;
			case GLFW_RELEASE:
				EventManager::queueEvent<KeyReleased>(key, mods);				
				break;
			case GLFW_REPEAT://TODO
				break;
			default: break;
			}
		});

		glfwSetMouseButtonCallback(m_window.get(), [](GLFWwindow*, int button, int action, int mods) {
			switch (action) {
			case GLFW_PRESS:
				EventManager::queueEvent<MouseButtonPressed>(button, mods);
				break;
			case GLFW_RELEASE:
				EventManager::queueEvent<MouseButtonReleased>(button, mods);
				break;
			case GLFW_REPEAT://TODO
				break;
			default: break;
			}
		});

		glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow*, double x, double y) {
			EventManager::queueEvent<MouseMovedEvent>(x, y);
		});

		glfwSetScrollCallback(m_window.get(), [](GLFWwindow*, double xoffset, double yoffset) {
			EventManager::queueEvent<MouseMovedEvent>(xoffset, yoffset);
		});

		glfwSetWindowSizeCallback(m_window.get(), [](GLFWwindow*, int width, int height) {
			EventManager::queueEvent<WindowResizeEvent>(width, height);
		});

		glfwSetWindowCloseCallback(m_window.get(), [](GLFWwindow*) {
			EventManager::queueEvent<WindowCloseEvent>();
		});

		on_event<UpdateEvent>(  std::bind(&WindowsWindow::on_update, this, std::placeholders::_1));
		on_event<PreRenderEvent>(  std::bind(&WindowsWindow::on_pre_render, this, std::placeholders::_1));
		on_event<RenderEvent>(  std::bind(&WindowsWindow::on_render, this, std::placeholders::_1));
		on_event<PostRenderEvent>( std::bind(&WindowsWindow::on_post_render, this, std::placeholders::_1));

	}

	void WindowsWindow::toggle_fullscreen() {
		auto monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (m_mode != WindowMode::Fullscreen) {
			glfwSetWindowMonitor(m_window.get(), monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			m_mode = WindowMode::Fullscreen;
		}
		else {
			glfwSetWindowMonitor(m_window.get(), nullptr, 0, 30, m_width, m_height, mode->refreshRate);
			m_mode = WindowMode::Windowed;
		}
	}

	void WindowsWindow::on_update(const event_ptr_t<UpdateEvent>& ev) {
		glfwGetWindowSize(m_window.get(), &m_width, &m_height);
		glfwGetFramebufferSize(m_window.get(), &m_framebuffer_width, &m_framebuffer_height);
	}

	void WindowsWindow::shutdown(GLFWwindow* window) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void WindowsWindow::on_exit(const event_ptr_t<WindowCloseEvent>& event) {
	}

	void WindowsWindow::on_pre_render(const event_ptr_t<PreRenderEvent>& event) {
		glfwPollEvents();
		glClearColor(1.0, 0.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void WindowsWindow::on_render(const event_ptr_t<RenderEvent>& event) {
		
	}

	void WindowsWindow::on_post_render(const event_ptr_t<PostRenderEvent>& event) {
		glfwSwapBuffers(m_window.get());
	}

}