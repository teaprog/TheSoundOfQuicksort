/**
* 20.01.12 Quicksort
* Author: teaprog
*/

#include <SDL.h>
#include <SDL_opengl.h>

#include <vector>
using std::vector;

int screen_w = 1800;
int screen_h = 950;

bool quit = false;

SDL_Event event;

const int POLE_NUMBER = 500;

class Pole;

void init();
void reshape();
void display();
void drawAndInputHandle();
void shuffle();

void quickSort(Pole [], int);
void quickSort(Pole [], int, int, int);
int partition(Pole [], int, int, int);

int maxHeight = 0;
int minHeight = 9999999;

SDL_Window *screen;
SDL_GLContext ctx;

class Pole {
	private:
		GLfloat xPos;
		GLfloat yPos;
		int height;
		GLfloat width;

		GLfloat colorR1, colorG1, colorB1;
		GLfloat colorR2, colorG2, colorB2;

		static int poleCounter;
	public:
		Pole() {
			GLfloat spaceBetween = 0.0f;
			width = (screen_w - 20.0f - POLE_NUMBER * spaceBetween) / (POLE_NUMBER);
			changeHeight();

			xPos = 10.0f + poleCounter * (width + spaceBetween);
			yPos = screen_h - 100.0f;

			poleCounter++;

			setStandardColor();
		}

		void setStandardColor() {
			colorR1 = 0.0f;
			colorG1 = 0.3f;
			colorB1 = 0.0f;

			colorR2 = 0.0f;
			colorG2 = 0.0f;
			colorB2 = 1.0f;
		}

		void setActionColor() {
			colorR1 = 1.0f;
			colorG1 = 0.0f;
			colorB1 = 0.0f;

			colorR2 = 1.0f;
			colorG2 = 0.0f;
			colorB2 = 0.0f;
		}

		void setFinishedColor() {
			colorR1 = 0.0f;
			colorG1 = 0.5f;
			colorB1 = 0.0f;

			colorR2 = 0.0f;
			colorG2 = 1.0f;
			colorB2 = 0.0f;
		}

		void changeHeight() {
			height = rand() % (screen_h - 200);

			if (height > maxHeight)
				maxHeight = height;
			if (height < minHeight)
				minHeight = height;
		}

		void setHeight(int height) {
			this->height = height;
		}

		int getHeight() {
			return height;
		}

		void drawPole() {
			glColor3f(colorR1, colorG1, colorB1);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(xPos, yPos);
			glVertex2f(xPos + width, yPos);
			glColor3f(colorR2, colorG2, colorB2);
			glVertex2f(xPos + width, yPos - height);
			glVertex2f(xPos, yPos - height);
			glEnd();
		}
} pole[POLE_NUMBER];

int Pole::poleCounter = 0;

void drawAndInputHandle() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				quit = true;
				return;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
					return;
				}
				else if (event.key.keysym.sym == SDLK_SPACE)
					shuffle();
				break;
				case SDL_WINDOWEVENT:
                {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        screen_w = event.window.data1;
                        screen_h = event.window.data2;
                        reshape();
                    }
                }

			default:
				break;
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	for (int i = 0; i < POLE_NUMBER; i++)
		pole[i].drawPole();

	SDL_GL_SwapWindow(screen);
}

void shuffle() {
	for (int i = 0; i < POLE_NUMBER; i++) {
		pole[i].changeHeight();
		pole[i].setStandardColor();
	}

	quickSort(pole, POLE_NUMBER);
}

void quickSort(Pole pole[], int arrayLength) {
	quickSort(pole, arrayLength, 0, arrayLength-1);
}

void quickSort(Pole pole[], int arrayLength, int first, int last) {
	if (quit)
		return;

	if (last > first) {
		int pivot = partition(pole, arrayLength, first, last);
		pole[pivot].setFinishedColor();
		quickSort(pole, arrayLength, first, pivot-1);
		quickSort(pole, arrayLength, pivot+1, last);
	}
	else {
		pole[first].setFinishedColor();
	}
}

int partition(Pole pole[], int arrayLength, int first, int last) {
	int pivot = pole[first].getHeight();
	int low = first+1;
	int high = last;

	while (high > low) {
		if (quit)
			return 0;

		while (low <= high && pole[low].getHeight() <= pivot)
			low++;
		while (low <= high && pole[high].getHeight() > pivot)
			high--;

		if (high > low) {
			int temp = pole[high].getHeight();
			pole[high].setHeight(pole[low].getHeight());
			pole[high].setActionColor();
			// playSound(pole[high].getHeight());
			pole[low].setHeight(temp);
			pole[low].setActionColor();
			// playSound(pole[low].getHeight());
			drawAndInputHandle();
			pole[high].setStandardColor();
			pole[low].setStandardColor();
		}
	}

	while (high > first && pole[high].getHeight() >= pivot)
		high--;

	if (pivot > pole[high].getHeight()) {
		pole[first].setHeight(pole[high].getHeight());
		pole[first].setActionColor();
		// playSound(pole[first].getHeight());
		pole[high].setHeight(pivot);
		pole[high].setActionColor();
		// playSound(pole[high].getHeight());
		drawAndInputHandle();
		pole[first].setStandardColor();
		pole[high].setStandardColor();
		return high;
	}
	else {
		return first;
	}
}

void init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	//SDL_WM_SetCaption("20.01.12 Quicksort", 0);
	//SDL_putenv("SDL_VIDEO_CENTERED=center");
	//SDL_ShowCursor(0);

    SDL_Rect r;
    if (SDL_GetDisplayBounds(0, &r) != 0) {
        SDL_LogError(0, "SDL_GetDisplayBounds failed: %s", SDL_GetError());
    }
    else
        SDL_Log("width: %d, height: %d\n", r.w, r.h);

    if (r.w && r.h)
    {
        screen_w = r.w;
        screen_h = r.h;
    }


    screen = SDL_CreateWindow("TheSoundOfQuicksort",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          screen_w, screen_h,
                          SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    ctx = SDL_GL_CreateContext(screen);
	reshape();

	glClearColor(0.8f, 0.5f, 0.5f, 0.0f);
}

void reshape() {
	glViewport(0, 0, screen_w, screen_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, screen_w, screen_h, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	for (int i = 0; i < POLE_NUMBER; i++)
		pole[i].drawPole();

	SDL_GL_SwapWindow(screen);
}

int main(int argc, char** argv) {
	init();

	quickSort(pole, POLE_NUMBER);

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						quit = true;
					else if (event.key.keysym.sym == SDLK_SPACE)
						shuffle();
					break;
				case SDL_WINDOWEVENT:
                {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        screen_w = event.window.data1;
                        screen_h = event.window.data2;
                        reshape();
                    }
                }
				break;
				default:
					break;
			}
		}

		display();
	}

	SDL_Quit();
	return 0;
}
