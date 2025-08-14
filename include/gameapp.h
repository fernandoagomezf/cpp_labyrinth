#pragma once

#include "common.h"
#include "cell.h"

namespace game {
    using std::string;
    using std::vector;
    using std::unordered_map;
    using std::chrono::system_clock;
    using std::chrono::time_point;
    using game::Cell;
    using game::CellType;

    class GameApp {
        public:
            GameApp();
            ~GameApp();
            
            bool init(const string& title, int width, int height);
            void loadLevel(const string& filePath);
            void run();
            
        private:   
            bool loadTextures(const string& config);
            void render();
            void handleInput();
            bool movePlayer(int dx, int dy);
            void updateTimer();
            void clean();         

            SDL_Window* _window;
            SDL_Renderer* _renderer;
            vector<vector<Cell>> _grid;
            int _playerX;
            int _playerY;
            int _exitX; 
            int _exitY;
            int _levelWidth; 
            int _levelHeight;
            int _timeLimit;
            time_point<system_clock> _startTime;            
            unordered_map<char, SDL_Texture*> _textureMap;
    };
}