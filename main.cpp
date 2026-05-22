#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <ctime>

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

// --- FONKSİYONLAR ---
void initBoard() {
    board.assign(ROWS, std::vector<Cell>(COLS));
    srand((unsigned int)time(NULL));
    int placedMines = 0;
    while (placedMines < MINE_COUNT) {
        int r = rand() % ROWS, c = rand() % COLS;
        if (!board[r][c].isMine) {
            board[r][c].isMine = true;
            placedMines++;
        }
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

    // Flood Fill Algoritması (Boşsa komşuları aç)
    if (board[r][c].neighborMines == 0 && !board[r][c].isMine) {
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                revealCell(r + dr, c + dc);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) return -1;

    SDL_Window* window = SDL_CreateWindow("Mayin Tarlasi - Kerem", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    initBoard(); // Oyunu başlat

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) isRunning = false;

            // Tıklama Kontrolleri
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && !gameOver) {
                int c = (int)(event.button.x / CELL_WIDTH);
                int r = (int)(event.button.y / CELL_HEIGHT);

                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (board[r][c].isMine) {
                        gameOver = true;
                        for(int i=0; i<ROWS; i++) for(int j=0; j<COLS; j++) if(board[i][j].isMine) board[i][j].isRevealed = true;
                    } else {
                        revealCell(r, c);
                    }
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    board[r][c].isFlagged = !board[r][c].isFlagged;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        // Izgarayı (Grid) Çizdirme
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                SDL_FRect rect = { c * CELL_WIDTH + 1, r * CELL_HEIGHT + 1, CELL_WIDTH - 2, CELL_HEIGHT - 2 };
                if (board[r][c].isRevealed) {
                    if (board[r][c].isMine) {
                        SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255); // Patlayan Mayın (Kırmızı)
                    } else {
                        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255); // Açılmış Kare (Açık Gri)
                    }
                } else {
                    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255); // Kapalı Kare (Koyu Gri)
                    if (board[r][c].isFlagged) {
                        SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255); // Bayrak (Turuncu)
                    }
                }
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}