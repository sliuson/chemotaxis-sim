#include <SDL3/SDL.h>
#include <emscripten.h>
#include <array>
#include <vector>
#include <math.h>

#include <AttractantParticleSystem.hpp>
#include <Cell.hpp>
SDL_Window* window;
SDL_Renderer* renderer;
class AttractantParticleGenerator;
class AttractantParticleSystem;
AttractantParticleSystem* attractants = nullptr;
std::vector<AttractantParticleGenerator> generators;
std::vector<Cell> cells;


const float TIMESTEP = 1.0f/120.0f;
Uint64 COUNTERFREQUENCY;
Uint64 lastCount; 
float accumulatedTime;




//TODO: add active count and ring buffer use resize when creating xpos,ypos, and rects.
class AttractantParticleGenerator{
    public:
    float x;
    float y;
    float size;
    float generationRate;
    float creationProgress = 0;
    AttractantParticleSystem* particleSystem;
    AttractantParticleGenerator(float x, float y, float generationRate, float size, AttractantParticleSystem* particleSystem){

        this-> x = x;
        this->y = y;
        this-> generationRate = generationRate;
        this->size = size;
        this->particleSystem = particleSystem;
    }
    void update(){
        creationProgress += TIMESTEP * generationRate;
        while(creationProgress >= 1){
            createNewAttractant();
            creationProgress-=1;
        }
        

    }
    void createNewAttractant(){
        particleSystem->addParticle(x,y);
    }
    void display(){
        SDL_SetRenderDrawColor(renderer,0,255,0,255);
        SDL_FRect particleRectangle = {x - size/2.0f, y - size/2.0f, size,size};
        SDL_RenderFillRect(renderer, &particleRectangle);
    }
};

void update(){
    SDL_SetRenderDrawColor(renderer,30,30,30,255);
    SDL_RenderClear(renderer);
  
    for (int i = 0; i < generators.size(); i ++){
        generators[i].update();
        generators[i].display();
    }

    attractants->updateSystem();

    for (int i = 0; i < cells.size(); i ++){
        cells[i].update();
        cells[i].display(renderer);
    }

    SDL_RenderPresent(renderer);
}
void mainloop(){
    Uint64 currentCount = SDL_GetPerformanceCounter();
    float elapsedSecondsSinceLastLoop = (float)(currentCount - lastCount)/COUNTERFREQUENCY;
    lastCount = currentCount;
    accumulatedTime += elapsedSecondsSinceLastLoop;
    if (accumulatedTime > 0.25f){
        accumulatedTime = 0.25f;
    }
    while(accumulatedTime >= TIMESTEP){
        update();
        accumulatedTime -= TIMESTEP;
    }
    

}
void setup(){
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    attractants = new AttractantParticleSystem(renderer);
    generators.emplace_back(300.0f,300.0f,2000.0f,10.0f,attractants);
    generators.emplace_back(100.0f,100.0f,400.0f,6.0f,attractants);
    cells.emplace_back(200.0f,200.0f,attractants);
    cells.emplace_back(400.0f,200.0f,attractants);
    cells.emplace_back(300.0f,300.0f,attractants);
    
    COUNTERFREQUENCY =  SDL_GetPerformanceFrequency();
    accumulatedTime = 0;
    lastCount = SDL_GetPerformanceCounter();
    

}


int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("sketch",1000,800,0,&window,&renderer);
    SDL_SetWindowFillDocument(window, true);
   
    setup();
    emscripten_set_main_loop(mainloop,0,1);
}