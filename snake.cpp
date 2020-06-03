#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_HEIGHT = 800;
const int SCREEN_WIDTH = 800;

// each square will have 32 pixel height and width
const int GRID_SQUARE_DIMENSION = 32;

// the size of grid
const int GRID_DIMENSION = 25;

// window and screen setup
bool init();

// loading of all the media elements
bool loadMedia();

// for handling of memory deallocation and closing procedure of SDL
bool close();

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// The head of the snake upwards diretion
SDL_Surface *snakeHeadUp = NULL;

SDL_Surface *snakeHeadDown = NULL;
SDL_Surface *snakeHeadLeft = NULL;
SDL_Surface *snakeHeadRight = NULL;

SDL_Surface *snakeHead[4];

// The body of the snake upwards direction
SDL_Surface *snakeBodyUp = NULL;

SDL_Surface *snakeBodyDown = NULL;
SDL_Surface *snakeBodyLeft = NULL;
SDL_Surface *snakeBodyRight = NULL;

SDL_Surface *snakeBody[4];

// The corner of snake body
SDL_Surface *snakeCorner = NULL;

// All possible directions
enum Directions
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct SnakeNode
{
    int x;
    int y;
    Directions d; // for direction the sprite should be rendered in
    struct SnakeNode *next;
    SDL_Surface *snakeSpirte;
};

struct SnakeNode *snake = (SnakeNode *)malloc(sizeof(struct SnakeNode));

bool init()
{
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Create Window
        gWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, SCREEN_HEIGHT,
                                   SCREEN_WIDTH, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be loaded! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return success;
}

bool loadMedia()
{
    bool success;

    // Load snake head surface
    snakeBodyUp = SDL_LoadBMP("bmp/snake_body_up.bmp");
    if (snakeBodyUp == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s\n", "bmp/snake_head_up.bmp", SDL_GetError());
        success = false;
    }

    snakeBodyDown = SDL_LoadBMP("bmp/snake_body_down.bmp");
    if (snakeBodyDown == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s\n", "bmp/snake_head_down.bmp", SDL_GetError());
        success = false;
    }

    snakeBodyLeft = SDL_LoadBMP("bmp/snake_body_left.bmp");
    if (snakeBodyLeft == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s\n", "bmp/snake_head_left.bmp", SDL_GetError());
        success = false;
    }

    snakeBodyRight = SDL_LoadBMP("bmp/snake_body_right.bmp");
    if (snakeBodyRight == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s\n", "bmp/snake_head_right.bmp", SDL_GetError());
        success = false;
    }

    snakeBody[UP] = snakeBodyUp;
    snakeBody[DOWN] = snakeBodyDown;
    snakeBody[LEFT] = snakeBodyLeft;
    snakeBody[RIGHT] = snakeBodyRight;

    // Load snake body surface
    snakeHeadUp = SDL_LoadBMP("bmp/snake_head_up.bmp");
    if (snakeHeadUp == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s\n", "bmp/snake_body_up.bmp", SDL_GetError());
        success = false;
    }

    snakeHeadDown = SDL_LoadBMP("bmp/snake_head_down.bmp");
    if (snakeHeadDown == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s\n", "bmp/snake_body_down.bmp", SDL_GetError());
        success = false;
    }

    snakeHeadLeft = SDL_LoadBMP("bmp/snake_head_left.bmp");
    if (snakeHeadLeft == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s\n", "bmp/snake_body_left.bmp", SDL_GetError());
        success = false;
    }

    snakeHeadRight = SDL_LoadBMP("bmp/snake_head_right.bmp");
    if (snakeHeadRight == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s\n", "bmp/snake_body_right.bmp", SDL_GetError());
        success = false;
    }

    snakeHead[UP] = snakeHeadUp;
    snakeHead[DOWN] = snakeHeadDown;
    snakeHead[LEFT] = snakeHeadLeft;
    snakeHead[RIGHT] = snakeHeadRight;

    snakeCorner = SDL_LoadBMP("bmp/snake_body_corner_up_right.bmp");
    if (snakeCorner == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s\n", "bmp/snake_body_corner_up_right.bmp", SDL_GetError());
        success = false;
    }

    return success;
}

bool close()
{
    //Deallocate surface
    SDL_FreeSurface(snakeHeadUp);
    SDL_FreeSurface(snakeBodyUp);
    snakeBodyUp = NULL;
    snakeHeadUp = NULL;

    //Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // free the snake memory
    struct SnakeNode *head = snake;
    while (head != NULL)
    {
        struct SnakeNode *temp = head;
        head = head->next;
        free(temp);
    }

    //Quit SDL subsystems
    SDL_Quit();
}

void increaseSnakeSizeByOne(struct SnakeNode *head, Directions d)
{
    struct SnakeNode *body = (SnakeNode *)malloc(sizeof(struct SnakeNode));
    struct SnakeNode *temp = head->next;

    body->x = head->x;
    body->y = head->y;
    body->next = temp;
    body->snakeSpirte = snakeBody[d];

    snake->next = body;
    switch (d)
    {
    case UP:
        snake->y -= 1;
        break;
    case DOWN:
        snake->y += 1;
        break;
    case RIGHT:
        snake->x += 1;
        break;
    case LEFT:
        snake->x -= 1;
        break;
    default:
        break;
    }
}

int main()
{
    if (!init())
    {
        printf("Failed to Load!");
    }
    else
    {
        if (!loadMedia())
        {
            printf("Failed to load media!");
        }
        else
        {
            bool quit = false;

            SDL_Event e;

            // creating a snake
            // TODO: move to some function
            snake->x = 10;
            snake->y = 10;
            snake->d = UP;
            snake->next = NULL;
            snake->snakeSpirte = snakeHeadUp;

            while (!quit)
            {
                // Clear the current surface
                SDL_FillRect(gScreenSurface, NULL, 0x000000);

                struct SnakeNode *head = snake;

                Directions old_direction = UP;
                Directions new_direction = UP;

                // did the snake move ?
                bool movement = false;

                //Handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    //User requests quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    // keyboard handle
                    else if (e.type == SDL_KEYDOWN)
                    {
                        head = snake;
                        int temp_x = head->x;
                        int temp_y = head->y;
                        Directions temp_d = head->d;

                        old_direction = head->d;

                        switch (e.key.keysym.sym)
                        {
                        case SDLK_UP:
                            if (head->d != DOWN)
                            {
                                head->y -= 1;
                                head->d = UP;
                                movement = true;
                            }
                            break;
                        case SDLK_DOWN:
                            if (head->d != UP)
                            {
                                head->y += 1;
                                head->d = DOWN;
                                movement = true;
                            }
                            break;
                        case SDLK_RIGHT:
                            if (head->d != LEFT)
                            {
                                head->x += 1;
                                head->d = RIGHT;
                                movement = true;
                            }
                            break;
                        case SDLK_LEFT:
                            if (head->d != RIGHT)
                            {
                                head->x -= 1;
                                head->d = LEFT;
                                movement = true;
                            }
                            break;
                        default:
                            movement = false;
                            break;
                        }

                        new_direction = head->d;

                        // changing snake linked list
                        if (movement)
                        {
                            // change head sprite according to direction
                            head->snakeSpirte = snakeHead[head->d];

                            // moving on to first body block
                            head = head->next;
                            SDL_Surface *temp_snakeSprite;

                            if (head != NULL)
                            {
                                temp_snakeSprite = head->snakeSpirte;
                                if (old_direction == new_direction)
                                {
                                    head->snakeSpirte = snakeBody[temp_d];
                                }else{
                                    head->snakeSpirte = snakeCorner;
                                }

                                int temp_temp_x = head->x;
                                int temp_temp_y = head->y;
                                Directions temp_temp_d = head->d;

                                head->x = temp_x;
                                head->y = temp_y;
                                head->d = temp_d;

                                temp_x = temp_temp_x;
                                temp_y = temp_temp_y;
                                temp_d = temp_temp_d;

                                head = head->next;
                            }
                            while (head != NULL)
                            {
                                int temp_temp_x = head->x;
                                int temp_temp_y = head->y;
                                Directions temp_temp_d = head->d;
                                SDL_Surface *temp_temp_snakeSprite = head->snakeSpirte;

                                head->x = temp_x;
                                head->y = temp_y;
                                head->d = temp_d;
                                head->snakeSpirte = temp_snakeSprite;

                                temp_x = temp_temp_x;
                                temp_y = temp_temp_y;
                                temp_d = temp_temp_d;
                                temp_snakeSprite = temp_temp_snakeSprite;

                                head = head->next;
                            }
                        }

                        // For debugging only
                        switch (e.key.keysym.sym)
                        {
                        case SDLK_w:
                            increaseSnakeSizeByOne(head, UP);
                            break;
                        case SDLK_a:
                            increaseSnakeSizeByOne(head, LEFT);
                            break;
                        case SDLK_s:
                            increaseSnakeSizeByOne(head, DOWN);
                            break;
                        case SDLK_d:
                            increaseSnakeSizeByOne(head, RIGHT);
                            break;
                        default:
                            break;
                        }
                    }
                }

                // rendering portion
                if (movement)
                {
                    SDL_Rect *dstrect = (SDL_Rect *)malloc(sizeof(SDL_Rect));

                    head = snake;
                    // render the head of the snake
                    dstrect->x = head->x * GRID_SQUARE_DIMENSION;
                    dstrect->y = head->y * GRID_SQUARE_DIMENSION;
                    dstrect->h = GRID_SQUARE_DIMENSION;
                    dstrect->w = GRID_SQUARE_DIMENSION;

                    SDL_BlitSurface(head->snakeSpirte, NULL, gScreenSurface, dstrect);

                    head = head->next;

                    // handle the rendering of block next to head for corner control
                    if (head != NULL)
                    {
                        dstrect->x = head->x * GRID_SQUARE_DIMENSION;
                        dstrect->y = head->y * GRID_SQUARE_DIMENSION;
                        dstrect->h = GRID_SQUARE_DIMENSION;
                        dstrect->w = GRID_SQUARE_DIMENSION;

                        SDL_BlitSurface(head->snakeSpirte, NULL, gScreenSurface, dstrect);

                        head = head->next;
                    }

                    // render the remaining body of the snake
                    while (head != NULL)
                    {
                        dstrect->x = head->x * GRID_SQUARE_DIMENSION;
                        dstrect->y = head->y * GRID_SQUARE_DIMENSION;
                        dstrect->h = GRID_SQUARE_DIMENSION;
                        dstrect->w = GRID_SQUARE_DIMENSION;

                        SDL_BlitSurface(head->snakeSpirte, NULL, gScreenSurface, dstrect);
                        head = head->next;
                    }
                    free(dstrect);
                    SDL_UpdateWindowSurface(gWindow);
                }
            }
        }
    }

    close();

    return 0;
}