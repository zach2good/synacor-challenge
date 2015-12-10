#include "window.h"

Window::Window(const char* title, int width, int height)
	: TITLE(title), WIDTH(width), HEIGHT(height)
{
	init();
}

Window::~Window()
{
	cleanUp();
}

void Window::init()
{
	//Start SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL Error");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	// Start a Window
	m_Window = SDL_CreateWindow(TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH, HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!m_Window) {
		printf("Window Error");
	}

	// Start OpenGL Context
	m_Context = SDL_GL_CreateContext(m_Window);
	if (!m_Context) {
		printf("Context Error");
	}

	// Limit framerate to help screen tearing
	SDL_GL_SetSwapInterval(1);

	// Print Info
	//printf("GL_VERSION: %s \n", glGetString(GL_VERSION));
	//printf("GL_VENDOR: %s \n", glGetString(GL_VENDOR));
	//printf("GL_SHADING_LANGUAGE_VERSION: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	isRunning = true;
}

void Window::cleanUp()
{
	SDL_GL_DeleteContext(m_Context);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

SDL_Window* Window::getWindow() const
{
	return m_Window;
}

SDL_GLContext Window::getContext()
{
	return m_Context;
}

void Window::update()
{

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONDOWN:
			break;
		case SDL_MOUSEBUTTONUP:
			break;
		case SDL_KEYUP:
			break;
		case SDL_KEYDOWN:

			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE: requestClose(); break;
			default:
				break;
			}
			break;

		case SDL_QUIT:
			requestClose();
			break;
		default:
			break;
		}
	}
}

void Window::clear()
{
	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swap()
{
	SDL_GL_SwapWindow(m_Window);	
}

bool Window::isCloseRequested()
{
	return !isRunning;
}

void Window::requestClose()
{
	isRunning = false;
}
