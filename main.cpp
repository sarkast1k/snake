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

   Food()
   {
    Image image = LoadImage("Textures/hood.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    position = GenerateRandomPos();
   }

   ~Food()
   {
    UnloadTexture(texture);
   }

   void Draw()
   {
     DrawTexture(texture, position.x * CellSize, position.y * CellSize, WHITE);
   }

   Vector2 GenerateRandomPos()
   {
    float x = GetRandomValue(0, CellCount - 1);
    float y = GetRandomValue(0, CellCount - 1);
    return Vector2{x, y};
  }

};

int main () 
{

  cout << "Starting the game..." << endl;

  SetTargetFPS(60);
  
  InitWindow(CellSize * CellCount, CellSize * CellCount, "Snake Game");
  Food food;
  Snake snake;
  while (WindowShouldClose() == 0)
  {
    BeginDrawing();
    snake.Update();
    ClearBackground(Green);
    food.Draw();
    snake.Draw();
    EndDrawing();
  }

  CloseWindow();

return 0;
}