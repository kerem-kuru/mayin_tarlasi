#include <SDL3/SDL.h>
#include <iostream>

// --- AYARLAR ---
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {
    // SDL Başlatma
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL baslatilamadi: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Pencere ve Renderer Oluşturma
    SDL_Window* window = SDL_CreateWindow("Mayin Tarlasi - Kerem", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        std::cerr << "Pencere olusturulamadi: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Renderer olusturulamadi: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool isRunning = true;
    SDL_Event event;

    // Ana Oyun Döngüsü
    while (isRunning) {
        // Olayları (Event) Dinle
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
        }

        // Arka planı koyu gri yap (Ekranı temizle)
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        // Çizimleri ekrana yansıt
        SDL_RenderPresent(renderer);
    }

    // Program kapanırken hafızayı temizle
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}