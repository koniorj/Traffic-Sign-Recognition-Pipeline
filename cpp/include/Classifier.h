#ifndef TRAFFICSIGNRECOGNITION_CLASSIFIER_H
#define TRAFFICSIGNRECOGNITION_CLASSIFIER_H

#include <iostream>
#include <vector>
#include <onnxruntime_cxx_api.h>
#include "Types.h"

class Classifier
{
    Ort::Env env;
    Ort::Session session;
    const char* inputName = "input";
    const char* outputName = "output";

    public:
    Classifier(const std::string& path) :
    // create an environment with warning-level logging
    env(ORT_LOGGING_LEVEL_WARNING, "TrafficSignNet"),
    // load the ONNX model from the provided path using default session options
    session(env, path.c_str(), Ort::SessionOptions{nullptr}) {}
    ~Classifier() = default;

    std::vector<float> classify(const ProcessedData& data);
};

#endif //TRAFFICSIGNRECOGNITION_CLASSIFIER_H