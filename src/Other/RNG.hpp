#pragma once

#include <glm/glm.hpp>

class SquirrelRNG
{
private:
    unsigned int m_seed = 0;
    int m_position = 0;
    static const int PRIME1 = 0;
    static const int PRIME2 = 0;
    unsigned int getNextRand();
public:
    SquirrelRNG();
    SquirrelRNG(unsigned int seed);

    void resetSeedAndPosition(unsigned int seed, int position);
    unsigned int getSeed();
    void setCurrentPosition( int position);
    float normalisedValue();
    int getCurrentPosition();
    float randomFloat(float minValue, float maxValue);
    int randomIntInRange(int minValueInclusive, int maxValueExclusive);
    static unsigned int squirrel3(int position, unsigned int seed);
    glm::vec2 randomVec2(glm::vec2 min, glm::vec3 max);
    glm::vec3 randomVec3(glm::vec3 min, glm::vec3 max);
    unsigned int get1DNoise(int position);
    unsigned int get1DNoise(int position, unsigned int seed);
    unsigned int get2DNoise(int posX, int posY);
    unsigned int get2DNoise(int posX, int posY, unsigned int seed);
    unsigned int get3DNoise(int posX, int posY, int posZ);
    unsigned int get3DNoise(int posX, int posY, int posZ, unsigned int seed);
};

class RNG
{
private:
    static SquirrelRNG rng;
public:
    static SquirrelRNG* getRNG();
    static unsigned int getSeed();
    static int getCurrentPosition();
    static void resetRNG(unsigned int seed, int position);
    static float getRandomFloat(float minValue, float maxValue);
    static int getRandomIntInRange(int minValueInclusive, int maxValueExclusive);
    static float getNormalisedValue();
    static glm::vec2 getRandomVec2(glm::vec2 min, glm::vec3 max);
    static glm::vec3 getRandomVec3(glm::vec3 min, glm::vec3 max);
};

struct data{
    unsigned int seed;
    int position;
};