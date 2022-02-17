/**
* 20.01.12 Quicksort
* Author: teaprog
*/
extern "C" void usleep(int x);
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdint.h>
#include <assert.h>
typedef uint32_t u32;

int screen_w = 1680;
int screen_h = 1050;
#define BG_COLOR 0.1f, 0.0f, 0.1f, 0.0f
//#define BG_COLOR 0.8f, 0.5f, 0.5f, 0.0f

bool quit = false;

SDL_Event event;

const int POLE_NUMBER = 64;

class Pole;

Pole** user_poles;
u32* pole_data_buffer;

void init();
void reshape();
void display();
void drawAndInputHandle();
void start_new_sort(int);

void mergeSort(Pole[], int);
void mergeSort(Pole[], int, int, int);
void merge(Pole[], int, int, int);

void quickSort(Pole [], int);
void quickSort(Pole [], int, int, int);
int partition(Pole [], int, int, int);

int maxHeight = 0;
int minHeight = 9999999;

SDL_Window *screen;
SDL_GLContext ctx;
int n_poles = POLE_NUMBER;

int reads;
int writes;

class Pole {
//	private:
	public:
		GLfloat xPos;
		GLfloat yPos;
		int height;
		GLfloat width;

		GLfloat colorR1, colorG1, colorB1;
		GLfloat colorR2, colorG2, colorB2;

		static int poleCounter;

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
			colorR1 = 0.6f;
			colorG1 = 0.6f;
			colorB1 = 0.6f;

			colorR2 = 1.0f;
			colorG2 = 1.0f;
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

        void setMarkColor()
        {
 			colorR1 = 0.1f;
			colorG1 = 0.1f;
			colorB1 = 0.6f;

			colorR2 = 0.1f;
			colorG2 = 0.1f;
			colorB2 = 0.7f;
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
            writes++;
			this->height = height;
		}

		int getHeight() {
            reads++;
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

void draw_poles()
{
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < POLE_NUMBER; i++)
    {
        auto p = pole[i];
        glColor3f(p.colorR2, p.colorG2, p.colorB2);
        glVertex2f(p.xPos, p.yPos);
        glColor3f(p.colorR2, p.colorG2, p.colorB2);//      c  
        glVertex2f(p.xPos + p.width, p.yPos);
        glVertex2f(p.xPos, p.yPos - p.height);//       | /

        glVertex2f(p.xPos + p.width, p.yPos);
        glColor3f(p.colorR1, p.colorG1, p.colorB1);
        glVertex2f(p.xPos + p.width,  p.yPos - p.height);
        glVertex2f(p.xPos, p.yPos - p.height);
    }

    glEnd();
}

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
					start_new_sort(0);
				else if (event.key.keysym.sym == SDLK_RETURN)
                    start_new_sort(1);
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
    {
		//pole[i].drawPole();
    }

    draw_poles();
    usleep(80000);
    
    SDL_GL_SwapWindow(screen);
}

void start_new_sort(int sort_algo) {
	for (int i = 0; i < n_poles; i++) {
		pole[i].changeHeight();
		pole[i].setStandardColor();
	}
    if (sort_algo)
        mergeSort(pole, n_poles);
    else
	    quickSort(pole, n_poles);
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

void swap_and_draw(Pole poles[], int x, int y) {
    assert(x != y);
    const int x_height = pole[x].getHeight();
    const int y_height = pole[y].getHeight();
    
    poles[x].setHeight(y_height);
    poles[x].setActionColor();
    // playSound(pole[high].getHeight());
    poles[y].setHeight(x_height);
    poles[y].setActionColor();
    drawAndInputHandle();
    poles[x].setStandardColor();
    poles[y].setStandardColor();
}

void mark_and_draw(Pole poles[], int x, int y) {
    assert(x != y);    
    poles[x].setMarkColor();
    poles[y].setMarkColor();
    
    drawAndInputHandle();
    
    poles[x].setStandardColor();
    poles[y].setStandardColor();
}


void mergeSort(Pole poles[], int arrayLength) {
    int currentStepSize = 2;
    int sorted_entries = 0;
    for(;;)
    {
        if (quit)
            return ;

        for(int i = 0;
            i < arrayLength;
            i += currentStepSize)
        {
            if (quit)
                return ;
            for(int j = i + 1;
                j < i + currentStepSize;
                j++)
            {
                int x = i;
                int y = j;
                
                int x_height = pole[x].getHeight();
                int y_height = pole[y].getHeight();
                if (x_height > y_height)
                    swap_and_draw(poles, x, y);
                else
                {
                    sorted_entries++;
                    mark_and_draw(poles, x, y);
                }
            }
            // sort of current_group done
            // now for the merge
            {
                merge(poles, i, i + (currentStepSize * 2), currentStepSize);
            }
            drawAndInputHandle();
        }
        currentStepSize *= 2;
        if (sorted_entries == arrayLength - 1) break;
    }
}

void merge(Pole poles[], int start, int end, int stepSize) {
    int n1 = start;
    int n2 = start + stepSize;
    int a_height = poles[n1].getHeight();
    int b_height = poles[n2].getHeight();
    while(n2 < end && n1 < stepSize)
    {
        if (a_height > b_height)
        {
            swap_and_draw(poles, n1, n2);
            n2++;
        }
        else
        {
            mark_and_draw(poles, n1, n2);
            n1++;
        }
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
            swap_and_draw(pole, high, low);
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
	SDL_Init(SDL_INIT_VIDEO);
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
                          SDL_WINDOW_OPENGL /*| SDL_WINDOW_FULLSCREEN*/);
    ctx = SDL_GL_CreateContext(screen);
	reshape();

	glClearColor(BG_COLOR);
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

/*
#define cast(T) (T)
#define MAX_COUNT 300
void readPoles(char* filename)
{
    auto f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    auto length = ftell(f);
    pole_data_buffer = cast(u32*) malloc(length);
    auto count = length / sizeof(u32);
    // clamp count
    // count = (count < MAX_COUNT ? count : MAX_COUNT);

    fseek(f, 0, SEEK_SET);
    fread(pole_data_buffer, sizeof(u32), count, f);
    fclose(f);

    n_poles = count;
    // n poles has to be set before calling the ctor
    user_poles = cast(Pole**) malloc(sizeof(Pole*) * count);

    for(int i = 0; i < count; i++)
    {
        // user_poles[i] = new Pole(pole_data_buffer[i]);
    }

    printf("n_poles: %d\n", n_poles);

}
*/
int main(int argc, char** argv) {
	init();
    // has to be done after init. pole constructor needs the screen size
    // readPoles(argv[1]);

	quickSort(pole, sizeof(pole)/sizeof(pole[0]));

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
						start_new_sort(0);
					else if (event.key.keysym.sym == SDLK_RETURN)
                        start_new_sort(1);
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
