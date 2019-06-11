#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <vector>

#include <cstdint>
#include <random>

//This permutation table is the table provided in some example code in the paper Ken Perlin wrote on Simplex Noise

static const uint8_t permutationTable[256] = {
    151, 160, 137, 91, 90, 15,
    131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
    190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
    77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
    102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
    135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
    5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
    223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
    129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
    251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
    49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

static float computeGradient(unsigned int hashNumber, float xPos, float yPos) {

    const unsigned int convertedHash = hashNumber & 0x3F;
    const float uF = convertedHash < 4 ? xPos : yPos;
    const float vF = convertedHash < 4 ? yPos : xPos;
    return ((convertedHash & 1) ? -uF : uF) + ((convertedHash & 2) ? -2.0f * vF : 2.0f * vF);
}

//2D Perlin Noise implementation following the methods used in the paper Ken Perlin wrote on Simplex Noise

float getNoiseAt(float xPos, float yPos) {
    
    float noiseContribution0, noiseContribution1, noiseContribution2;

    static const float skewingFactor0 = 0.366025403f;
    static const float skewingFactor1 = 0.211324865f;

    const float skewConstant = (xPos + yPos) * skewingFactor0;

    const float xSkewed = xPos + skewConstant;
    const float ySkewed = yPos + skewConstant;

    const int xFloor = floor(xSkewed);
    const int yFloor = floor(ySkewed);

    const float unskewConstant = static_cast<float>(xFloor + yFloor) * skewingFactor1;

    const float xUnskewed = xFloor - unskewConstant;
    const float yUnskewed = yFloor - unskewConstant;

    const float xOffset0 = xPos - xUnskewed;
    const float yOffset0 = yPos - yUnskewed;

    int middleOffsetI, middleOffsetJ;

    if (xOffset0 > yOffset0) { middleOffsetI = 1; middleOffsetJ = 0; }
    else { middleOffsetI = 0; middleOffsetJ = 1; }

    const float xOffset1 = xOffset0 - middleOffsetI + skewingFactor1;
    const float yOffset1 = yOffset0 - middleOffsetJ + skewingFactor1;

    const float xOffset2 = xOffset0 - 1.0f + 2.0f * skewingFactor1;
    const float yOffset2 = yOffset0 - 1.0f + 2.0f * skewingFactor1;

    // Work out the hashed gradient indices of the three simplex corners
    const int gradientIndices0 = permutationTable[(xFloor + permutationTable[(yFloor)])];
    const int gradientIndices1 = permutationTable[(xFloor + middleOffsetI + permutationTable[(yFloor + middleOffsetJ)])];
    const int gradientIndices2 = permutationTable[(xFloor + 1 + permutationTable[(yFloor + 1)])];

    // Calculate the contribution from the first corner
    float firstContribution = 0.5f - xOffset0 * xOffset0 - yOffset0 * yOffset0;

    if(!(firstContribution < 0.0f)) firstContribution *= firstContribution;
    noiseContribution0 = firstContribution < 0.0f ? 0.0f : firstContribution * firstContribution * computeGradient(gradientIndices0, xOffset0, yOffset0);

    // Calculate the contribution from the second corner
    float secondContribution = 0.5f - xOffset1 * xOffset1 - yOffset1 * yOffset1;

    if(!(secondContribution < 0.0f)) secondContribution *= secondContribution;
    noiseContribution1 = secondContribution < 0.0f ? 0.0f : secondContribution * secondContribution * computeGradient(gradientIndices1, xOffset1, yOffset1);

    // Calculate the contribution from the third corner
    float thirdContribution = 0.5f - xOffset2 * xOffset2 - yOffset2 * yOffset2;

    if(!(thirdContribution < 0.0f)) thirdContribution *= thirdContribution;
    thirdContribution = thirdContribution < 0.0f ? 0.0f : thirdContribution * thirdContribution * computeGradient(gradientIndices2, xOffset2, yOffset2);

    // All the noise contribution are added together then scaled so that the values are between -1 and 1.
    return 45.23065f * (noiseContribution0 + noiseContribution1 + noiseContribution2);
}

static void generateNoiseMap(int terrainSize, float noiseScale, float lacanarityValue, float persistanceValue, int amountOfOctaves, float*** outputMap){

    float **noiseMap = new float*[terrainSize];

    float maxHeight = FLT_MIN;
    float minHeight = FLT_MAX;

    for(int i = 0; i < terrainSize; ++i){

        noiseMap[i] = new float[terrainSize];

        for(int j = 0; j < terrainSize; ++j){
            
            float noiseValue = 0;

            float sampleFrequency = 1;
            float sampleAmplitude = 1;

            for(int k = 0; k < amountOfOctaves; k++){

                float sampleX = (i - terrainSize/2) / noiseScale * sampleFrequency;
                float sampleZ = (j - terrainSize/2) / noiseScale * sampleFrequency;

                float sampledNoise = getNoiseAt(sampleX, sampleZ);

                noiseValue += sampleAmplitude * sampledNoise;

                sampleAmplitude *= persistanceValue;
                sampleFrequency *= lacanarityValue;
            }

            if(noiseValue > maxHeight) maxHeight = noiseValue;
            if(noiseValue < minHeight) minHeight = noiseValue;

            noiseMap[i][j] = noiseValue;
        }
    }

    for(int i = 0; i < terrainSize; ++i){
        for(int j = 0; j < terrainSize; ++j){
            noiseMap[i][j] = (noiseMap[i][j] - minHeight) / (maxHeight - minHeight);
        }
    }

    *outputMap = noiseMap;
}