#include "../include/Postprocessor.h"

#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;

int Postprocessor::topPrediction(const std::vector<float>& models_answers)
{
    auto topThree = Postprocessor::topThreePredictions(models_answers);
    // if (topThree[0].first < 0.7f)
    // {
    //     std::cout << "[WARNING] Low certainty!: " << (topThree[0].first * 100) << "%" << std::endl;
    // }

    return topThree[0].second;
}

string Postprocessor::getLabelsName(int classId)
{
    if (classId >= 0 && classId < labels.size()) {
        return labels[classId];
    }
    return "Invalid ID";
}


vector<std::pair<float, int>> Postprocessor::topThreePredictions(const std::vector<float>& models_answers)
{
    vector<float> probabilities = softmax(models_answers, 43);
    vector<std::pair<float, int>> topThree;

    for (int i = 0; i < (int)probabilities.size(); ++i) {
        topThree.emplace_back(probabilities[i], i);
    }

    ranges::partial_sort(topThree, topThree.begin() + 3,
                         [](const auto& a, const auto& b) {
                             return a.first > b.first;
                         });

    topThree.erase(topThree.begin()+3, topThree.end());

    return topThree;
}

std::vector<float> Postprocessor::softmax(const std::vector<float>& models_answers, const int64_t dimensions=43)
{
    std::vector<float> output(dimensions);;
    float max_input = *std::max_element(models_answers.begin(), models_answers.end());

    float tmpsum = 0.0f;
    for (int i = 0; i < dimensions; ++i)
    {
        const float z = std::exp(models_answers[i] - max_input);
        output[i] = z;
        tmpsum += z;
    }

    for (int i = 0; i < dimensions; ++i)
    {
        output[i] /= tmpsum;
    }

    return output;
}