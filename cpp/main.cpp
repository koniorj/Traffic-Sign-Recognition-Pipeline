#include <fstream>
#include <vector>
#include <iostream>
#include <filesystem>

#include "include/BatchRunner.h"
#include "include/Postprocessor.h"
#include "include/Classifier.h"
#include "include/Preprocessor.h"

using namespace std;

void runAnImage(std::string model_path, std::string image_path)
{
    std::cout << "Initializing..." << "\n";

    try {
            Classifier classifier(model_path);
            ImageData rawImage = ImageLoader::loadPPM(image_path);
            std::cout << "Image loaded: " << rawImage.width << "x" << rawImage.height << "\n";
            ProcessedData processedData = Preprocessor::process(rawImage, 224, 224);
            std::cout << "Image resized and normalized \n";
            std::vector<float> models_answers = classifier.classify(processedData);
            Postprocessor postprocessor;
            std::vector<float> softm = postprocessor.softmax(models_answers, 43);
            int class_prediction = postprocessor.topPrediction(models_answers);
            string class_label = postprocessor.getLabelsName(class_prediction);
            std::vector<float> probabilities = postprocessor.softmax(models_answers, 43);
            vector<std::pair<float, int>> topThree = postprocessor.topThreePredictions(models_answers);
            std::cout << "\nTOP THREE PREDICTIONS:" << std::endl;

            for (const auto& item : topThree) {
                int classId = item.second;
                float prob = probabilities[classId];

                std::cout << "CLASS: " << postprocessor.getLabelsName(classId)
                          << " | ID: " << classId
                          << " | CERTAINTY: " << (prob * 100.0f) << "%" << "\n";
            }

            std::cout << "ID RECOGNIZED: " << class_prediction << "\n";
            std::cout << "SIGN RECOGNIZED: " << class_label << "\n";

        }
        catch (const std::exception& e) {
            std::cerr << "ERROR: " << e.what() << "\n";
        }
}


void runBatches(std::string model_path)
{
    const std::string image_dir = "/mnt/c/Users/konio/CLionProjects/Traffic-Sign-Recognition/python/ppm_test/";
    const std::string csv_path = "/mnt/c/Users/konio/CLionProjects/Traffic-Sign-Recognition/python/ground_truth.csv";

    std::cout << "Initializing..." << "\n";

    try
    {
        Classifier classifier(model_path);
        Preprocessor preprocessor;
        Postprocessor postprocessor;
        BatchRunner batchRunner(classifier, preprocessor, postprocessor);

        const std::map<std::string, int> label_data = batchRunner.prepareLabelData(csv_path);
        batchRunner.runBatch(image_dir, label_data);
        batchRunner.printSummary();
        batchRunner.printBenchmark();
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
    }
}

int main()
{
    const std::string model_path = "/mnt/c/Users/konio/CLionProjects/Traffic-Sign-Recognition/python/model/traffic_sign_model2.onnx";
    const std::string image_path = "/mnt/c/Users/konio/CLionProjects/Traffic-Sign-Recognition/cpp/images/50km.ppm";

    // runAnImage(model_path, image_path);
    runBatches(model_path);

    return 0;
}