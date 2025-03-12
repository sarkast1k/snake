  #include <iostream>
  #include <raylib.h>
  #include <deque>
  #include <raymath.h>

  using namespace std;

  Color Green = {173,204,96,255};
  Color DarkGreen = {43,51,24,255};

  int CellSize = 30;
  int CellCount = 25;

  double LastUpdateTime = 0;

  bool ElementInDeque(Vector2 element, deque<Vector2> deque)
  {
    for (unsigned int i = 0; i < deque.size(); i++)
    {
      if(Vector2Equals(deque[i], element))
      {
        return true;
      }
    }
    return false;
  }

  bool EventTriggered(double interval)
  {
    double CurrentTime = GetTime();
    if(CurrentTime - LastUpdateTime >= interval)
    {
      LastUpdateTime = CurrentTime;
      return true;
    }return false;
  }

  class Snake
  {
  public:
      deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
      Vector2 direction = {1,0};

      void Draw()
    {
      for (unsigned int i = 0; i < body.size(); i++)
      {
        float x = body[i].x;
        float y = body[i].y;
        Rectangle segment = Rectangle{x * CellSize, y* CellSize, (float)CellSize, (float)CellSize};
        DrawRectangleRounded(segment, 0.5, 6, DarkGreen);
      }
    }

    void Update()
    {
      body.pop_back();
      body.push_front(Vector2Add(body[0], direction));
    }
      
  };

  class Food{
  public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody)
    {
      Image image = LoadImage("Textures/hood.png");
      texture = LoadTextureFromImage(image);
      UnloadImage(image);
      position = GenerateRandomPos(snakeBody);
    }

    ~Food()
    {
      UnloadTexture(texture);
    }

    void Draw()
    {
      DrawTexture(texture, position.x * CellSize, position.y * CellSize, WHITE);
    }

    Vector2 GenerateRandomCell()
    {
      float x = GetRandomValue(0, CellCount - 1);
      float y = GetRandomValue(0, CellCount - 1);
      return Vector2{x,y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
    {
      Vector2 position = GenerateRandomCell();
      while (ElementInDeque(position, snakeBody))
      {
        position = GenerateRandomCell();
      }
      return position;
    }

  };

  class Game
  {
public: 

  Snake snake = Snake();
  Food food = Food(snake.body);

  void Draw()
  {
    food.Draw();
    snake.Draw();
  }
  
  void Update()
  {
    snake.Update();
    EatFood();
  }

  void EatFood()
  {
    if (Vector2Equals(snake.body[0], food.position))
    {
      food.position = food.GenerateRandomPos(snake.body);
    }
    
  }

  };

  int main () 
  {

    cout << "Starting the game..." << endl;

    SetTargetFPS(60);
    
    InitWindow(CellSize * CellCount, CellSize * CellCount, "Snake Game");
    
    Game game = Game();

    while (WindowShouldClose() == 0)
    {
      BeginDrawing();

      if(EventTriggered(0.2))
      {
        game.Update();
      } 

      if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){game.snake.direction = {0, -1};}
      if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){game.snake.direction = {0, 1};}
      if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){game.snake.direction = {-1, 0};}
      if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){game.snake.direction = {1, 0};}

      ClearBackground(DARKBLUE);
      game.Draw();
      EndDrawing();
    }

    CloseWindow();

  return 0;
  }