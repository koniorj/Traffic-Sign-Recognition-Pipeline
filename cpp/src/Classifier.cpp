#include <iostream>
#include <vector>
#include <numeric>
#include <onnxruntime_cxx_api.h>
#include "Classifier.h"

std::vector<float> Classifier::classify(const ProcessedData& data)
{
    // input - (1, 3, 224, 224)
    std::vector<int64_t> input_dimensions = {1, 3, 224, 224};

    Ort::MemoryInfo memory_info{ nullptr };     // used to allocate memory for input
    try {
        memory_info = std::move(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault));
    }
    catch (Ort::Exception& oe)
    {
        std::cerr << "ONNX exception caught: " << oe.what() << ". Code: " << oe.GetOrtErrorCode() << ".\n";
        return std::vector<float>();
    }

    Ort::Value input_tensor;
    try
    {
        input_tensor = Ort::Value::CreateTensor<float>(
             memory_info,
             const_cast<float*>(data.normalizedPixels.data()),
             data.normalizedPixels.size(),
             input_dimensions.data(),
             input_dimensions.size()
        );
    }
    catch (Ort::Exception& oe) {
        std::cerr << "ONNX exception caught: " << oe.what() << ". Code: " << oe.GetOrtErrorCode() << ".\n";
        return std::vector<float>();
    }

    try {
        auto output_tensor = session.Run(
            Ort::RunOptions{ nullptr },
            &inputName,
            &input_tensor,
            1,
            &outputName,
            1);

        Ort::Value& result = output_tensor.front();

        float* models_answers = result.GetTensorMutableData<float>();
        std::vector results(models_answers, models_answers + 43);

        return results;
    }
    catch (Ort::Exception& oe) {
        std::cout << "ONNX exception caught: " << oe.what() << ". Code: " << oe.GetOrtErrorCode() << ".\n";
        return std::vector<float>();
    }
}