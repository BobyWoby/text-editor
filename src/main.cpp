#include "SDL3/SDL_error.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_test_font.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

enum Mode { NORMAL, INSERT };

class TextRenderer{
    private:

    public:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    std::string text_string{};
    TTF_Font *font; 
    TTF_TextEngine *text_engine; 
    TTF_Text *text; 

    int windowWidth, windowHeight;
    int textWidth, textHeight;
    SDL_FRect rect;

    SDL_Color col = SDL_Color(255, 255, 255);

    TextRenderer(){
        TTF_Init();
        SDL_Init(SDL_INIT_VIDEO);

        SDL_CreateWindowAndRenderer("Text Editor", 1280, 760, SDL_WINDOW_TRANSPARENT, &window, &renderer);

        font = TTF_OpenFont("/usr/share/fonts/JetBrainsMono/JetBrainsMonoNerdFont-Regular.ttf", 25);
        if (font == NULL) {
            SDL_Log("Error loading font: %s", SDL_GetError());
        }

        text_engine = TTF_CreateRendererTextEngine(renderer);
        text = TTF_CreateText(text_engine, font, text_string.c_str(), 0);

        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    }

    void render(){
        // If this isn't here, the program will leak memory and brick ur computer
        TTF_SetTextString(text, text_string.c_str(), sizeof(text_string));
        SDL_DestroyTexture(texture);
        SDL_Surface *surface; 
        TTF_GetStringSizeWrapped(font, text_string.c_str(), 0, windowWidth, &textWidth, &textHeight);

        
        if(text_string.length() > 0){
            // std::cout << text_string.c_str() << "\n"; 
            surface = TTF_RenderText_Solid_Wrapped(font, text_string.c_str(), 0, col, windowWidth);
            if(surface == NULL){
                std::cout << "Surface is NULL: " << SDL_GetError() << "\n";
            } 
            rect = SDL_FRect(0, 0, (float)std::min(textWidth, windowWidth), textHeight);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            if(texture == NULL){
                std::cout << "texture is NULL: " << SDL_GetError() << "\n";
            }
        }
        SDL_RenderClear(renderer);

        // TTF_DrawRendererText(text, 0, 0);
        SDL_RenderTexture(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);
        SDL_DestroySurface(surface);
    }
    ~TextRenderer(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyTexture(texture);
        TTF_DestroyRendererTextEngine(text_engine);
        SDL_DestroyWindow(window);
    }
};
int main(int argc, char *argv[]) {
    TextRenderer drawer{};
    bool running = true;
    Mode activeMode = NORMAL;

    std::string filepath = "";
    if(argc < 2){
        std::cout << "No file was passed in!\n";
        filepath = "file.txt";
    }else{
        std::cout << "argc: " << argc << ", argv: " << argv[1] << "\n";
        filepath = argv[1];
        if(std::filesystem::is_regular_file(filepath)){
            std::cout << filepath << " is a regular file!\n";
        }
    }
    assert(std::filesystem::is_regular_file(filepath));

    std::ifstream file(filepath);
    if(!file.is_open()){
        std::cout << "Failed to open " << filepath << "\n";
        return 0;
    }

    //write the file to the piece table here 
    char insertText[1024] = {0};
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_TEXT_INPUT:
                    strncpy(insertText, event.text.text, sizeof(insertText));

                    //TODO: add this to the piece table
                    TTF_AppendTextString(drawer.text, event.text.text, 0);
                    drawer.text_string += insertText;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    SDL_Keycode keycode = event.key.key;
                    if (event.key.mod == 0x1040 && keycode == SDLK_C){ // the SDL_KMOD_LCTRL is defined as 0x0040u which is wrong as the u doesn't get registered
                        SDL_StopTextInput(drawer.window);
                        activeMode = NORMAL;
                    } else if (keycode == SDLK_RETURN && activeMode == INSERT) {
                        strncpy(insertText, "\n", sizeof(insertText));
                        //TODO: add this to the piece table
                        drawer.text_string += "\n";
                    } else if (keycode == SDLK_ESCAPE || (keycode == SDLK_Q && activeMode != INSERT)) {
                        SDL_StopTextInput(drawer.window);
                        activeMode = NORMAL;
                        running = false;
                    } else if (keycode == SDLK_I) {
                        SDL_StartTextInput(drawer.window);
                        activeMode = INSERT;
                    }                    // process mode changes and stuff here
                    break;
            }
        }
        drawer.render();
    }
    return 0;
}
