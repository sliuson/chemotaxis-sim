#include <SDL3/SDL.h>
#include <vector>
#include <AttractantParticleSystem.hpp>


AttractantParticleSystem::AttractantParticleSystem(SDL_Renderer* renderer){
    walkStepDistance = 5;
    particleSize = 2;
    this->renderer = renderer;
    particleLimit = 30000;
    activeCount = 0;
    nextIndex = 0;

    //initializes all particles.
    xpositions.resize(particleLimit);
    ypositions.resize(particleLimit);
    renderRectangles.resize(particleLimit);

    for (int i = 0; i < renderRectangles.size(); i ++){
        renderRectangles[i].w = particleSize;
        renderRectangles[i].h = particleSize;
    }
    gridSquareSize = 10;
    gridRows = 1000/gridSquareSize;
    gridCols = 1000/gridSquareSize;

    concentrationGrid = std::vector<int>(gridRows*gridCols,0);

    std::random_device rd;
    gen.seed(rd());
    distribution = std::uniform_real_distribution<float>(0.0f,1.0f);
    
}
void AttractantParticleSystem::updateParticleGridPosition(int index){
    int colPosition = xpositions[index]/gridSquareSize;
    int rowPosition = ypositions[index]/gridSquareSize;

    concentrationGrid[rowPosition*gridCols + colPosition]++; 
}

void AttractantParticleSystem::updateRandomWalk(){
    //reset the concentration grid for a new tallying
    std::fill(concentrationGrid.begin(), concentrationGrid.end(), 0);

    for (int i = 0; i < activeCount; i ++){
        float randomAngle = distribution(gen) * 2 * PI;
        xpositions[i] += std::cos(randomAngle) * walkStepDistance;
        ypositions[i] += std::sin(randomAngle) * walkStepDistance;
        renderRectangles[i].x = xpositions[i];
        renderRectangles[i].y = ypositions[i];

        updateParticleGridPosition(i);

    }
}

void AttractantParticleSystem::drawAllParticles(){
    SDL_SetRenderDrawColor(renderer,0,255,0,40);
    SDL_RenderFillRects(renderer, renderRectangles.data(), activeCount);
}

void AttractantParticleSystem::addParticle(float x, float y){
    xpositions[nextIndex] = x;
    ypositions[nextIndex] = y;
    renderRectangles[nextIndex].x = x - particleSize/2.0f;
    renderRectangles[nextIndex].y = y - particleSize/2.0f;
    
    nextIndex ++;
    if(nextIndex >= particleLimit){
        nextIndex = 0;
    }
    
    if(activeCount < particleLimit){
        activeCount ++;
    }

}
void AttractantParticleSystem::updateSystem(){
    updateRandomWalk();
    drawAllParticles();
}

int AttractantParticleSystem::getGridConcentration(float x, float y){
    int colPosition = x/gridSquareSize;
    int rowPosition = y/gridSquareSize;

    return concentrationGrid[rowPosition*gridCols + colPosition]; 
}
