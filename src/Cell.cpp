#include <Cell.hpp>
#include <AttractantParticleSystem.hpp>
#include <SDL3/SDL.h>
#include <random>

void Cell::tumble(){
    //set angle to random new angle
    float randomAngle = distribution(gen) * 2 * PI;
    velx = std::cos(randomAngle) * speed;
    vely = std::sin(randomAngle) * speed;

}

Cell::Cell(float x, float y, AttractantParticleSystem* attractants){
    this->x = x;
    this->y = y;
    this->speed = 0.2f;
    this->attractants = attractants;
    size = 20;
    counter = 0;

    //TODO: consolidate with attractantparticlesystem's random (maybe make a getRandomAngle function?)
    std::random_device rd;
    gen.seed(rd());
    distribution = std::uniform_real_distribution<float>(0.0f,1.0f);

    //set random angle on spawn
    tumble();
}
void Cell::display(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer,255/(attractants->getGridConcentration(x,y)+1),(attractants->getGridConcentration(x,y)),0,255);
    SDL_FRect cellRect({x-size/2, y - size/2, size,size});
    SDL_RenderFillRect(renderer, &cellRect);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderRect(renderer, &cellRect);
}


void Cell::update(){
    x+=velx;
    y+=vely;
    counter ++;
    //each count is 1/120 s TODO: dont hardcode
    float timeStep = 1.0f/120.0f;
    if (counter >= 40){
        counter = 0;

        if(currentConcentration <= lastConcentration){
            tumble();
        }
        
        lastConcentration = currentConcentration;
        currentConcentration = attractants->getGridConcentration(x,y);
    }
}
