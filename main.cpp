#include <raylib.h>
#include <vector>

using namespace std;

int cell_size = 28;
int cell_number = 20;
int OFFSET = 50;

Color green = Color{173,204,96,255};
Color dark_green = Color{43,51,24,255};

double previousTime = GetTime();

bool EventTriggered(double interval) {
  // Get current time
  double currentTime = GetTime();

  // Check if the interval has passed
  if (currentTime - previousTime >= interval) {
    // Update the previous time to the current time
    previousTime = currentTime;
    return true;
  } else {
    return false;
  }
}

bool operator==(Vector2 vector1, Vector2 vector2)
{
    if(vector1.x == vector2.x && vector1.y == vector2.y)
    {
        return true;
    }
    return false;
}

bool ElementInVector(vector<Vector2> vector, Vector2 element)
{
 for (size_t i = 0; i < vector.size(); i++)
    {
        if(vector[i] == element){
            return true;
        }
    }
    return false;
} 

class Snake {

public:
vector<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
Vector2 direction = {1, 0};
bool add_segment = false;

void Draw(){

    for (size_t i = 0; i < body.size(); i++)
    {
        float x = body[i].x;
        float y = body[i].y;
        DrawRectangleRounded(Rectangle{OFFSET + x * cell_size, OFFSET + y * cell_size, (float)cell_size, (float)cell_size}, 0.5, 6, dark_green);
    }
}

void Update(){

    if (add_segment){
        body.insert(body.begin(), Vector2{body[0].x + direction.x, body[0].y + direction.y} );
        add_segment = false;
    }
    else{
    body.pop_back();
    Vector2 head = {body[0].x + direction.x, body[0].y + direction.y};
    body.insert(body.begin(), head);
    }
}

void Reset(){
    body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    direction = {1, 0};
}

};

class Food {

public:
Vector2 pos ;
Texture2D texture;

    Food() {
        pos = GenerateRandomPos();
        Image image = LoadImage("Graphics/food.png"); 
        texture = LoadTextureFromImage(image);
        UnloadImage(image);  
}

void Draw()
{
    DrawTexture(texture, OFFSET + pos.x * cell_size, OFFSET + pos.y * cell_size, WHITE);   
}

void Recreate(vector<Vector2> snake_body) {
    pos = GenerateRandomPos();
    while (ElementInVector(snake_body, pos))
    {
        pos = GenerateRandomPos();
    }
}

Vector2 GenerateRandomPos()
{
    float x = GetRandomValue(0, cell_number-1);
    float y = GetRandomValue(0, cell_number-1);
    Vector2 new_pos = {x, y};
    return new_pos;
}
};

class Game {

public:
    Snake snake;
    Food food;
    bool running = false;
    int score = 0;

    Game(){
        snake = Snake();
        food = Food();
    }
    void Draw() {
        snake.Draw();
        food.Draw();
    }

    void Update() {
        snake.Update();
        CheckCollisionWithFood();
        CheckCollisionWithSelf();
        CheckCollisionWithEdges();
    }

    void CheckCollisionWithEdges(){
        if(snake.body[0].x == cell_number || snake.body[0].x == -1) {
            GameOver();
        }
        if(snake.body[0].y == cell_number || snake.body[0].y == -1) {
            GameOver();
        }
    }

    void CheckCollisionWithFood(){
        if(snake.body[0] == food.pos)
        {
            food.Recreate(snake.body);
            snake.add_segment = true;
            score ++;
        }
    }
    void CheckCollisionWithSelf(){
        vector<Vector2> body_without_head = snake.body;
        body_without_head.erase(body_without_head.begin());
        if(ElementInVector(body_without_head, snake.body[0]) )
        {
            GameOver();
        }
    }

    void GameOver(){
        snake.Reset();
        food.Recreate(snake.body);
        running = false;
        score = 0;
    }
};

int main () {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(OFFSET*2 + cell_size*cell_number, OFFSET*2 + cell_size*cell_number, "Retro Snake");
    SetTargetFPS(60);

    Game game = Game();

    while (WindowShouldClose() == false){
        //Check for events
        if(IsKeyDown(KEY_DOWN)){
            if(game.snake.direction.y != -1) {
            game.snake.direction = Vector2{0,1};
            }
            game.running = true;
        }
        if(IsKeyDown(KEY_UP)){
            if(game.snake.direction.y !=1){
                game.snake.direction = Vector2{0,-1};
            }
            game.running = true;
        }
        if(IsKeyDown(KEY_RIGHT)){
            if(game.snake.direction.x != -1){
            game.snake.direction = Vector2{1,0};
              }
            game.running = true;
        }
        if(IsKeyDown(KEY_LEFT)){
            if(game.snake.direction.x != 1){
                game.snake.direction = Vector2{-1,0};
            }
            game.running = true;
        }
  
        BeginDrawing();
    
        //Updating
        if (EventTriggered(0.25) && game.running) {
            game.Update();
        }
        
        //Drawing
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{float(OFFSET -5), float(OFFSET-5), float(cell_number*cell_size + 10), (float)cell_number*cell_size +10},5,dark_green);  
        DrawText(TextFormat("%i", game.score), 45, cell_number*cell_size + OFFSET +10, 30, dark_green);
        DrawText("RETRO SNAKE", 45, 12, 30, dark_green);
        game.Draw();
        EndDrawing();
        }

        UnloadTexture(game.food.texture);
        CloseWindow();
        return 0;
}