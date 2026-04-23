
#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include <random>

class AttractantParticleSystem{
    static constexpr float PI = 3.14159f;
    int particleLimit;
    int activeCount;
    int nextIndex;
    std::vector<float> xpositions;
    std::vector<float> ypositions;
    std::vector<SDL_FRect> renderRectangles;

    int gridSquareSize;
    int gridRows;
    int gridCols;
    std::vector<int> concentrationGrid;
    float walkStepDistance;
    float particleSize;
    SDL_Renderer* renderer;
   
    std::mt19937 gen;
    std::uniform_real_distribution<float> distribution;

    void applyParticleLimit();

    void updateRandomWalk();
    void drawAllParticles();
    void updateParticleGridPosition(int index);
    public:
    AttractantParticleSystem(SDL_Renderer* renderer);
    void addParticle(float x, float y);
    void updateSystem();
    int getGridConcentration(float x, float y);

};