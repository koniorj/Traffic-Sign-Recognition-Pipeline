#ifndef TRAFFICSIGNRECOGNITION_BATCHRUNNER_H
#define TRAFFICSIGNRECOGNITION_BATCHRUNNER_H
#include "Classifier.h"
#include "Preprocessor.h"
#include "Postprocessor.h"
#include <map>


class BatchRunner
{
    int total_ = 0;
    int correct_ = 0;
    double time_in_ms_ = 0.0;

    Classifier& classifier;
    Preprocessor& preprocessor;
    Postprocessor& postprocessor;

    // I intended to make a Benchmark class, but it would implement virtually
    // the same methods.
    // Instead, I'll increase the complexity of this class a little.
    double min_time_ms_ = std::numeric_limits<double>::max();
    double max_time_ms_ = 0.0;

public:
    BatchRunner(Classifier& classifier, Preprocessor& preprocessor, Postprocessor& postprocessor)
    : classifier(classifier), preprocessor(preprocessor), postprocessor(postprocessor) {};

    std::map<std::string, int> prepareLabelData(const std::string& csv_path);
    void runBatch(const std::string& image_dir, const std::map<std::string, int>& label_data);
    void printSummary() const;
    void printBenchmark() const;
};


#endif //TRAFFICSIGNRECOGNITION_BATCHRUNNER_H