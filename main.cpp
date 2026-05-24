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
const int TIME_LIMIT = 60; // 60 saniye süre

const float CELL_WIDTH = (float)WINDOW_WIDTH / COLS;
const float CELL_HEIGHT = (float)WINDOW_HEIGHT / ROWS;

// YENİ: GAME_WIN durumu eklendi
enum GameState { MENU, PLAYING, GAME_OVER, GAME_WIN };
GameState currentState = MENU;

struct Cell {
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    int neighborMines = 0;
};

std::vector<std::vector<Cell>> board;
TTF_Font* mainFont = nullptr;
Uint64 startTime = 0;

// Resim (Texture) Değişkenleri
SDL_Texture* flagTexture = nullptr;
SDL_Texture* mineTexture = nullptr;

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

void drawText(SDL_Renderer* renderer, std::string text, float x, float y, SDL_Color color) {
    if (!mainFont) return;
    SDL_Surface* surf = TTF_RenderText_Blended(mainFont, text.c_str(), 0, color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FRect dst = { x, y, (float)surf->w, (float)surf->h };
    SDL_RenderTexture(renderer, tex, nullptr, &dst);
    SDL_DestroySurface(surf);
    SDL_DestroyTexture(tex);
}

// .bmp Resmi Yükleme Fonksiyonu
SDL_Texture* loadBMPTexture(SDL_Renderer* renderer, const char* file) {
    SDL_Surface* surface = SDL_LoadBMP(file);
    if (!surface) {
        std::cout << file << " bulunamadi! Resim yerine renk kullanilacak." << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) return -1;
    if (TTF_Init() == -1) return -1;

    mainFont = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);
    if (!mainFont) std::cout << "Font yuklenemedi!" << std::endl;

    SDL_Window* window = SDL_CreateWindow("Mayin Tarlasi - Kerem", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    flagTexture = loadBMPTexture(renderer, "flag.bmp");
    mineTexture = loadBMPTexture(renderer, "mine.bmp");

    SDL_FRect startButton = { 150, 450, 300, 60 };
    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        if (currentState == PLAYING) {
            int elapsedSeconds = (SDL_GetTicks() - startTime) / 1000;
            if (TIME_LIMIT - elapsedSeconds <= 0) {
                currentState = GAME_OVER;
                for(int i=0; i<ROWS; i++) for(int j=0; j<COLS; j++) if(board[i][j].isMine) board[i][j].isRevealed = true;
            }
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) isRunning = false;

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (currentState == MENU) {
                    if (event.button.x >= startButton.x && event.button.x <= startButton.x + startButton.w &&
                        event.button.y >= startButton.y && event.button.y <= startButton.y + startButton.h) {
                        initBoard();
                        currentState = PLAYING;
                        startTime = SDL_GetTicks();
                    }
                } else if (currentState == PLAYING) {
                    int c = (int)(event.button.x / CELL_WIDTH);
                    int r = (int)(event.button.y / CELL_HEIGHT);

                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (board[r][c].isMine) {
                            currentState = GAME_OVER;
                            for(int i=0; i<ROWS; i++) for(int j=0; j<COLS; j++) if(board[i][j].isMine) board[i][j].isRevealed = true;
                        } else {
                            revealCell(r, c);

                            // (Win Condition)
                            int revealedSafeCells = 0;
                            for(int i=0; i<ROWS; i++) {
                                for(int j=0; j<COLS; j++) {
                                    if(board[i][j].isRevealed && !board[i][j].isMine) revealedSafeCells++;
                                }
                            }
                            // Toplam güvenli hücre sayısına ulaşıldıysa kazan
                            if (revealedSafeCells == (ROWS * COLS) - MINE_COUNT) {
                                currentState = GAME_WIN;
                            }
                        }
                    } else if (event.button.button == SDL_BUTTON_RIGHT) {
                        board[r][c].isFlagged = !board[r][c].isFlagged;
                    }
                // YENİ: Kazandıktan veya kaybettikten sonra tıklanınca menüye dön
                } else if (currentState == GAME_OVER || currentState == GAME_WIN) {
                    currentState = MENU;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        if (currentState == MENU) {
            drawText(renderer, "MAYIN TARLASI - REHBER", 150, 30, {255, 255, 255, 255});

            SDL_FRect r1 = {50, 100, 30, 30}; SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255); SDL_RenderFillRect(renderer, &r1);
            drawText(renderer, "Kapali Kutu", 100, 100, {255, 255, 255, 255});

            SDL_FRect r2 = {50, 150, 30, 30};
            if (flagTexture) SDL_RenderTexture(renderer, flagTexture, nullptr, &r2);
            else { SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255); SDL_RenderFillRect(renderer, &r2); }
            drawText(renderer, "Bayrak (Sag Tik)", 100, 150, {255, 255, 255, 255});

            SDL_FRect r3 = {50, 200, 30, 30};
            if (mineTexture) SDL_RenderTexture(renderer, mineTexture, nullptr, &r3);
            else { SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255); SDL_RenderFillRect(renderer, &r3); }
            drawText(renderer, "Mayin (Tehlike)", 100, 200, {255, 255, 255, 255});

            SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);
            SDL_RenderFillRect(renderer, &startButton);
            drawText(renderer, "OYUNU BASLAT", startButton.x + 55, startButton.y + 15, {255, 255, 255, 255});

        } else {
            for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                    SDL_FRect rect = { c * CELL_WIDTH + 1, r * CELL_HEIGHT + 1, CELL_WIDTH - 2, CELL_HEIGHT - 2 };
                    if (board[r][c].isRevealed) {
                        if (board[r][c].isMine) {
                            if (mineTexture) SDL_RenderTexture(renderer, mineTexture, nullptr, &rect);
                            else { SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255); SDL_RenderFillRect(renderer, &rect); }
                        } else {
                            SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
                            SDL_RenderFillRect(renderer, &rect);
                            if (board[r][c].neighborMines > 0)
                                drawText(renderer, std::to_string(board[r][c].neighborMines), c * CELL_WIDTH + 25, r * CELL_HEIGHT + 15, {0, 0, 0, 255});
                            continue;
                        }
                    } else {
                        SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
                        SDL_RenderFillRect(renderer, &rect);
                        if (board[r][c].isFlagged) {
                            if (flagTexture) SDL_RenderTexture(renderer, flagTexture, nullptr, &rect);
                            else { SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255); SDL_RenderFillRect(renderer, &rect); }
                        }
                    }
                }
            }

            if (currentState == PLAYING) {
                int elapsedSeconds = (SDL_GetTicks() - startTime) / 1000;
                int remainingSeconds = TIME_LIMIT - elapsedSeconds;
                SDL_FRect timeBg = {5, 5, 120, 35};
                SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
                SDL_RenderFillRect(renderer, &timeBg);
                drawText(renderer, "Sure: " + std::to_string(remainingSeconds), 10, 10, {255, 255, 0, 255});
            }

            // YENİ: Oyun sonu mesajları (Kaybetme ve Kazanma)
            if (currentState == GAME_OVER) {
                drawText(renderer, "GUM! MENU ICIN TIKLA", 150, 280, {255, 0, 0, 255});
            } else if (currentState == GAME_WIN) {
                // Arka plana ufak koyu bir kutu atalım ki yeşil yazı daha net okunsun
                SDL_FRect winBg = {110, 275, 380, 40};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
                SDL_RenderFillRect(renderer, &winBg);

                drawText(renderer, "TEBRIKLER KAZANDIN!", 115, 280, {0, 255, 0, 255});
            }
        }
        SDL_RenderPresent(renderer);
    }

    if (flagTexture) SDL_DestroyTexture(flagTexture);
    if (mineTexture) SDL_DestroyTexture(mineTexture);

    TTF_CloseFont(mainFont);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
