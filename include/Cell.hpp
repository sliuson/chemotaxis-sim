#pragma once 
#include <SDL3/SDL.h>
#include <AttractantParticleSystem.hpp>
class Cell{
    
    int currentConcentration;
    int lastConcentration;
    int defaultRunTime;
    float x;
    float y;
    float velx;
    float vely;
    float currentAngle;
    float speed;
    float size;
    AttractantParticleSystem* attractants;
    int counter;

    static constexpr float PI = 3.14159f;
    std::mt19937 gen;
    std::uniform_real_distribution<float> distribution;

    void tumble();
    public:
    Cell(float x,float y,AttractantParticleSystem* attractants);
    
    void update();
    void display(SDL_Renderer* renderer);
};