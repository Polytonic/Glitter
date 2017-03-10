// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <SDL.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

int main(int argc, char * argv[]) 
{
	//Start SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL Error");
		return EXIT_FAILURE;
	}

	// Start a Window
	SDL_Window* m_Window = SDL_CreateWindow("OpenGL Skeleton",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		mWidth, mHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!m_Window) {
		printf("Window Error");
		return EXIT_FAILURE;
	}

	// Start OpenGL Context
	SDL_GLContext m_Context = SDL_GL_CreateContext(m_Window);
	if (!m_Context) {
		printf("Context Error");
		return EXIT_FAILURE;
	}

	// Load GLAD Extentions
	if (!gladLoadGL()) {
		printf("GLAD Error");
		return EXIT_FAILURE;
	}

	// Print Info
	printf("GL_VERSION: %s \n", glGetString(GL_VERSION));
	printf("GL_VENDOR: %s \n", glGetString(GL_VENDOR));
	printf("GL_SHADING_LANGUAGE_VERSION: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	bool running = true;
	while (running) {

		// Poll inputs
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
				running = false;
			}
		}

		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(m_Window);

	}

	SDL_GL_DeleteContext(m_Context);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
    return EXIT_SUCCESS;
}
