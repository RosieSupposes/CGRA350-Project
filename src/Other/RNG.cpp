#include "RNG.hpp"
#include "opengl.hpp"
#include <iostream>

using namespace glm;

SquirrelRNG::SquirrelRNG()
{
    resetSeedAndPosition(0, 0);
}
SquirrelRNG::SquirrelRNG(unsigned int seed){
    resetSeedAndPosition(seed, 0);
}

void SquirrelRNG::resetSeedAndPosition(unsigned int seed, int position) 
{ 
    m_seed = seed; 
    m_position = position;
}
unsigned int SquirrelRNG::getSeed()
{
    return m_seed; 
}
void SquirrelRNG::setCurrentPosition( int position) 
{
    m_position = position;
}
int SquirrelRNG::getCurrentPosition()
{
    return m_position;
}
unsigned int SquirrelRNG::getNextRand()
{
    unsigned int result = squirrel3(m_position, m_seed);
    m_position++;
    return result;
}
float SquirrelRNG::randomFloat(float minValue, float maxValue)
{
    return (minValue + normalisedValue() * (maxValue - minValue));
}

int SquirrelRNG::randomIntInRange(int minValueInclusive, int maxValueExclusive)
{
    return (int)(minValueInclusive + normalisedValue() * (maxValueExclusive - minValueInclusive));
}

float SquirrelRNG::normalisedValue()
{
    unsigned int result = getNextRand();
    float value = (1.0 * result)/UINT_MAX;
    return value;
}

glm::vec2 SquirrelRNG::randomVec2(vec2 min, vec3 max)
{
    float x = randomFloat(min.x, max.x);
    float y = randomFloat(min.y, max.y);
    return vec2(x, y);
}

glm::vec3 SquirrelRNG::randomVec3(vec3 min, vec3 max)
{
    float x = randomFloat(min.x, max.x);
    float y = randomFloat(min.y, max.y);
    float z = randomFloat(min.z, max.z);
    return vec3(x,y,z);
}

unsigned int SquirrelRNG::squirrel3(int position, unsigned int seed)
{
    constexpr unsigned int BIT_NOISE_1 = 0xB5297A4D;
    constexpr unsigned int BIT_NOISE_2 = 0x68E31DA4;
    constexpr unsigned int BIT_NOISE_3 = 0x1B56C4E9;
    
    unsigned int mangled = (unsigned int) position;
    mangled *= BIT_NOISE_1;
    mangled += seed;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE_2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE_3;
    mangled ^= (mangled >> 8);
    return mangled;
}
    
unsigned int SquirrelRNG::get1DNoise(int position){
    return squirrel3(position, 0);
}

unsigned int SquirrelRNG::get1DNoise(int position, unsigned int seed){
    return squirrel3(position, seed);
}

unsigned int SquirrelRNG::get2DNoise(int posX, int posY){
    return squirrel3(posX + (PRIME1 * posY), 0);
}

unsigned int SquirrelRNG::get2DNoise(int posX, int posY, unsigned int seed){
    return squirrel3(posX + (PRIME1 * posY), seed);
}

unsigned int SquirrelRNG::get3DNoise(int posX, int posY, int posZ){
    return squirrel3(posX + (PRIME1 * posY) + (PRIME2 * posZ), 0);
}

unsigned int SquirrelRNG::get3DNoise(int posX, int posY, int posZ, unsigned int seed){
    return squirrel3(posX + (PRIME1 * posY) + (PRIME2 * posZ), seed);
}

SquirrelRNG RNG::rng = SquirrelRNG();

SquirrelRNG* RNG::getRNG()
{
    return &rng;
}

void RNG::resetRNG(unsigned int seed, int position)
{
    getRNG()->resetSeedAndPosition(seed, position);
}
float RNG::getRandomFloat(float minValue, float maxValue)
{
    
    return getRNG()->randomFloat(minValue, maxValue);
}

int RNG::getRandomIntInRange(int minValueInclusive, int maxValueExclusive)
{
    return getRNG()->randomIntInRange(minValueInclusive, maxValueExclusive);
}

float RNG::getNormalisedValue()
{
    return getRNG()->normalisedValue();
}

glm::vec2 RNG::getRandomVec2(vec2 min, vec3 max)
{
    return getRNG()->randomVec2(min, max);
}

glm::vec3 RNG::getRandomVec3(vec3 min, vec3 max)
{
    return getRNG()->randomVec3(min, max);
}
