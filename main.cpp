  #include <iostream>
  #include <raylib.h>
  #include <deque>
  #include <raymath.h>

  using namespace std;

  Color Green = {173,204,96,255};
  Color DarkGreen = {43,51,24,255};

  int CellSize = 30;
  int CellCount = 25;
  int offset = 75;
  
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
    bool addSegment = false;
    
    void reset()
    {
      body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
      direction = {1,0};
    }
    
    void Draw()
    {
      for (unsigned int i = 0; i < body.size(); i++)
      {
        float x = body[i].x;
        float y = body[i].y;
        Rectangle segment = Rectangle{offset + x * CellSize, offset + y* CellSize, (float)CellSize, (float)CellSize};
        DrawRectangleRounded(segment, 0.5, 6, DarkGreen);
      }
    }

    void Update()
    {        
      body.push_front(Vector2Add(body[0], direction));
      if(addSegment == true)
      {
        addSegment = false;
      }
      else
      {
        body.pop_back();
      }
      
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
      DrawTexture(texture, offset + position.x * CellSize, offset + position.y * CellSize, WHITE);
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

  int score = 0;
  Snake snake = Snake();
  Food food = Food(snake.body);
  bool Running = true;

  void Draw()
  {
    food.Draw();
    snake.Draw();
  }
  
  void Update()
  {
    if(Running)
    {
    snake.Update();
    EatFood();
    CheckCollision();
    CheckCollisionWithBody();
    }
  }

  void EatFood()
  {
    if (Vector2Equals(snake.body[0], food.position))
    {
      food.position = food.GenerateRandomPos(snake.body);
      snake.addSegment = true;
      score ++;
    }
    
  }

  void GameOver()
  {
    cout << "collided" << endl;
    snake.reset();
    food.position = food.GenerateRandomPos(snake.body);
    Running = false;
    score = 0;
  }

  void CheckCollision()
  {
    if(snake.body[0].x == CellCount || snake.body[0].x == -1)
    {
      GameOver();
    }
    if(snake.body[0].y == CellCount || snake.body[0].y == -1)
    {
      GameOver();
    }
  }

  void CheckCollisionWithBody()
  {
    deque<Vector2> HeadlessBody = snake.body;
    HeadlessBody.pop_front();
    if (ElementInDeque(snake.body[0], HeadlessBody))
    {
      GameOver();
    }
    
  }

  };

  int main () 
  {

    cout << "Starting the game..." << endl;

    SetTargetFPS(60);
    
    InitWindow(2*offset + CellSize*CellCount, 2*offset + CellSize*CellCount, "Snake Game");
    
    Game game = Game();

    while (WindowShouldClose() == 0)
    {
      BeginDrawing();

      if(EventTriggered(0.2))
      {
        game.Update();
      } 

      if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){game.snake.direction = {0, -1};game.Running = true;}
      if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){game.snake.direction = {0, 1};game.Running = true;}
      if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){game.snake.direction = {-1, 0};game.Running = true;}
      if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){game.snake.direction = {1, 0};game.Running = true;}

      ClearBackground(DARKBLUE);
      DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)CellSize*CellCount+10, (float)CellSize*CellCount+10}, 5, BLUE);
      DrawText(TextFormat("%i", game.score), offset - 5, 20, 40, WHITE);
      game.Draw();
      EndDrawing();
    }

    CloseWindow();

  return 0;
  }