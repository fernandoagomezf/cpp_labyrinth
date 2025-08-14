
#include "gameapp.h"

using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::system_clock;
using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::stoi;
using std::string;
using std::vector;
using game::GameApp;

GameApp::GameApp() {
    _window = nullptr;
    _renderer = nullptr;
}

bool GameApp::init(const string& title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! Error: " << SDL_GetError() << endl;
        return false;
    }
    
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!_window) {
        cerr << "Window could not be created! Error: " << SDL_GetError() << endl;
        return false;
    }
    
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer) {
        cerr << "Renderer could not be created! Error: " << SDL_GetError() << endl;
        return false;
    }
    
    return true;
}

void GameApp::loadLevel(const std::string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Failed to open level file: " << filePath << endl;
        return;
    }
    
    string line;
    string textureConfig;
    auto readingData = false;
    auto readingTextures = false;
    
    while (getline(file, line)) {
        if (line.find("WIDTH") != string::npos) {
            _levelWidth = stoi(line.substr(6));
        } else if (line.find("HEIGHT") != string::npos) {
            _levelHeight = stoi(line.substr(7));
        } else if (line.find("TIME_LIMIT") != string::npos) {
            _timeLimit = stoi(line.substr(11));
        } else if (line == "DATA") {
            readingData = true;
            _grid.resize(_levelHeight, vector<Cell>(_levelWidth));
            continue;
        } else if (line == "TEXTURES") {
            readingData = false;
            readingTextures = true;
            continue;
        }
        
        if (readingData && line.length() >= _levelWidth * 2 - 1) {
            int y = static_cast<int>(_grid.size()) - _levelHeight;
            for (int x = 0; x < _levelWidth; x++) {
                char cellChar = line[x * 2];
                CellType type;
                
                if (cellChar == '#') type = WALL;
                else if (cellChar == 'P') {
                    type = PLAYER;
                    _playerX = x;
                    _playerY = y;
                } else if (cellChar == 'E') {
                    type = EXIT;
                    _exitX = x;
                    _exitY = y;
                } else {
                    type = PATH;
                }
                
                _grid[y][x] = { type, nullptr, {x * 32, y * 32, 32, 32} };
            }
        } else if (readingTextures) {
            textureConfig += line + "\n";
        }
    }
    
    loadTextures(textureConfig);
    _startTime = std::chrono::system_clock::now();
}

void GameApp::run() {
    bool running = true;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        
        handleInput();
        updateTimer();
        render();
        
        SDL_Delay(16); // ~60 FPS
    }
}

void GameApp::handleInput() {
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    
    if (keystates[SDL_SCANCODE_W]) {
        movePlayer(0, -1);
    }
    if (keystates[SDL_SCANCODE_S]) {
        movePlayer(0, 1);
    }
    if (keystates[SDL_SCANCODE_A]) {
        movePlayer(-1, 0);
    }
    if (keystates[SDL_SCANCODE_D]) {
        movePlayer(1, 0);
    }
}

bool GameApp::movePlayer(int dx, int dy) {
    int newX = _playerX + dx;
    int newY = _playerY + dy;
    
    if (newX < 0 || newX >= _levelWidth || newY < 0 || newY >= _levelHeight) 
        return false;
    
    if (_grid[newY][newX].type == WALL) 
        return false;
    
    // Move player
    _grid[_playerY][_playerX].type = PATH;
    _grid[_playerY][_playerX].texture = _textureMap[' '];
    
    _playerX = newX;
    _playerY = newY;
    
    _grid[_playerY][_playerX].type = PLAYER;
    _grid[_playerY][_playerX].texture = _textureMap['P'];
    
    // Check win condition
    if (_playerX == _exitX && _playerY == _exitY) {
        std::cout << "You escaped the maze!" << endl;
        // Add win state handling
    }
    
    return true;
}

void GameApp::render() {
    SDL_RenderClear(_renderer);
    
    // Render grid
    for (const auto& row : _grid) {
        for (const auto& cell : row) {
            SDL_RenderCopy(_renderer, cell.texture, NULL, &cell.rect);
        }
    }
    
    // Render UI (timer)
    updateTimer();
    
    SDL_RenderPresent(_renderer);
}

void GameApp::updateTimer() {
    auto now = system_clock::now();
    auto elapsed = duration_cast<seconds>(now - _startTime);
    int remaining = _timeLimit - elapsed.count();
    
    if (remaining <= 0) {
        cout << "Time's up! Game Over!" << endl;
        // Handle game over
    }
    
    // TODO: Render timer on screen
}