#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>

// --- AYARLAR ---
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int ROWS = 10;
const int COLS = 10;
const int MINE_COUNT = 15;

const float CELL_WIDTH = (float)WINDOW_WIDTH / COLS;
const float CELL_HEIGHT = (float)WINDOW_HEIGHT / ROWS;

struct Cell {
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    int neighborMines = 0;
};

std::vector<std::vector<Cell>> board;
bool gameOver = false;
TTF_Font* mainFont = nullptr;

// --- FONKSİYONLAR ---
void initBoard() {
    board.assign(ROWS, std::vector<Cell>(COLS));
    srand((unsigned int)time(NULL));
    int placedMines = 0;
    while (placedMines < MINE_COUNT) {
        int r = rand() % ROWS, c = rand() % COLS;
        if (!board[r][c].isMine) { board[r][c].isMine = true; placedMines++; }
    }
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (board[r][c].isMine) continue;
            int count = 0;
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && board[nr][nc].isMine) count++;
                }
            }
            board[r][c].neighborMines = count;
        }
    }
}

void revealCell(int r, int c) {
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS || board[r][c].isRevealed || board[r][c].isFlagged) return;
    board[r][c].isRevealed = true;
    if (board[r][c].neighborMines == 0 && !board[r][c].isMine) {
        for (int dr = -1; dr <= 1; dr++) for (int dc = -1; dc <= 1; dc++) revealCell(r + dr, c + dc);
    }
}

// Ekrana Yazı Yazdıran Fonksiyon
void drawText(SDL_Renderer* renderer, std::string text, float x, float y, SDL_Color color) {
    if (!mainFont) return;
    SDL_Surface* surf = TTF_RenderText_Blended(mainFont, text.c_str(), 0, color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FRect dst = { x, y, (float)surf->w, (float)surf->h };
    SDL_RenderTexture(renderer, tex, nullptr, &dst);
    SDL_DestroySurface(surf);
    SDL_DestroyTexture(tex);
}

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) return -1;
    if (TTF_Init() == -1) return -1; // TTF Başlatıldı

    // Fontu Yükle
    mainFont = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);
    if (!mainFont) {
        std::cout << "Font yuklenemedi!" << std::endl;
    }

    SDL_Window* window = SDL_CreateWindow("Mayin Tarlasi - Kerem", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    initBoard();
    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) isRunning = false;
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && !gameOver) {
                int c = (int)(event.button.x / CELL_WIDTH);
                int r = (int)(event.button.y / CELL_HEIGHT);
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (board[r][c].isMine) {
                        gameOver = true;
                        for(int i=0; i<ROWS; i++) for(int j=0; j<COLS; j++) if(board[i][j].isMine) board[i][j].isRevealed = true;
                    } else revealCell(r, c);
                } else if (event.button.button == SDL_BUTTON_RIGHT) board[r][c].isFlagged = !board[r][c].isFlagged;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                SDL_FRect rect = { c * CELL_WIDTH + 1, r * CELL_HEIGHT + 1, CELL_WIDTH - 2, CELL_HEIGHT - 2 };
                if (board[r][c].isRevealed) {
                    if (board[r][c].isMine) {
                        SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255);
                    } else {
                        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
                        SDL_RenderFillRect(renderer, &rect);

                        // Komşu mayın sayısını ekrana yazdır (0'dan büyükse)
                        if (board[r][c].neighborMines > 0) {
                            drawText(renderer, std::to_string(board[r][c].neighborMines), c * CELL_WIDTH + 25, r * CELL_HEIGHT + 15, {0, 0, 0, 255});
                        }
                        continue; // Yazıyı çizdikten sonra diğer dikdörtgeni çizmesin diye
                    }
                } else {
                    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
                    if (board[r][c].isFlagged) SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
                }
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(mainFont);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}