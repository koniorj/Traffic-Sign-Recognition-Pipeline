#include "../include/BatchRunner.h"
#include <filesystem>
#include <map>
namespace fs = std::filesystem;

std::map<std::string, int> BatchRunner::prepareLabelData(const std::string& csv_path)
{
    std::map<std::string, int> labels;

    std::ifstream file(csv_path);
    std::string line;
    if (!file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return labels;
    }

    std::getline(file, line);
    while (std::getline(file, line))
    {
        int comma = line.find(',');
        std::string filename = line.substr(0, comma);
        int classId = std::stoi(line.substr(comma + 1));
        labels[filename] = classId;
    }
    return labels;
}

void BatchRunner::runBatch(const std::string& image_dir, std::map<std::string, int> label_data)
{
    for (const auto& entry : fs::directory_iterator(image_dir)) {
        // if (entry.path().extension() != ".ppm") continue;

        std::string filename = entry.path().filename().string();
        std::string imagePath = entry.path().string();
        int correct_class = label_data[filename];

        auto start = std::chrono::high_resolution_clock::now();

        ImageData rawImage = ImageLoader::loadPPM(imagePath);
        ProcessedData processedData = preprocessor.process(rawImage, 224, 224);
        std::vector<float> models_answers = classifier.classify(processedData);
        int class_prediction = postprocessor.topPrediction(models_answers);

        auto end = std::chrono::high_resolution_clock::now();
        double image_duration = std::chrono::duration<double, std::milli>(end - start).count();
        time_in_ms_ += std::chrono::duration<double, std::milli>(end - start).count();

        if (image_duration < min_time_ms_) min_time_ms_ = image_duration;
        if (image_duration > max_time_ms_) max_time_ms_ = image_duration;

        ++total_;
        if (class_prediction == correct_class)
        {
            ++correct_;
        }
    }
}

void BatchRunner::printSummary() const
{
    printf("\n --------------------------------------\n");
    printf("MODEL SUMMARY\n");
    printf("--------------------------------------\n");
    printf("%d images processed \n", total_);
    printf("Accuracy: %.2f%% (%d/%d) \n",
        (static_cast<float>(correct_) / total_) * 100.0f, correct_, total_);
    printf("Average time: %.2f ms/image \n", time_in_ms_/total_);
    printf("Total time: %.2f ms \n", time_in_ms_);
}

void BatchRunner::printBenchmark() const
{
    if (total_ == 0) {
        printf("Performance Benchmark: No data yet\n");
        return;
    }

    // time spent on a single image
    double avg_latency = time_in_ms_ / total_;
    // frames processed per second
    double fps = 1000.0 / avg_latency;

    printf("\n --------------------------------------\n");
    printf("BENCHMARK RESULTS\n");
    printf("--------------------------------------\n");
    printf("%d images processed \n", total_);
    printf("Average time: %.2f ms/image \n", avg_latency);
    printf("Min time: %.2f ms/image \n", min_time_ms_);
    printf("Max time: %.2f ms/image \n", max_time_ms_);
    printf("Total time: %.2f ms \n", time_in_ms_);
    printf("FPS: %.2f FPS\n", fps);
}
