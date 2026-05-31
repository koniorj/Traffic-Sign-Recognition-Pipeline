#ifndef TRAFFICSIGNRECOGNITION_POSTPROCESSOR_H
#define TRAFFICSIGNRECOGNITION_POSTPROCESSOR_H
#include <string>
#include <vector>


class Postprocessor
{
    std::vector<std::string> labels;

public:
    Postprocessor()
    {
        labels = {
            "Speed limit (20km/h)", "Speed limit (30km/h)", "Speed limit (50km/h)",
            "Speed limit (60km/h)", "Speed limit (70km/h)", "Speed limit (80km/h)",
            "End of speed limit (80km/h)", "Speed limit (100km/h)", "Speed limit (120km/h)",
            "No passing", "No passing for vehicles over 3.5 metric tons",
            "Right-of-way at the next intersection", "Priority road", "Yield", "Stop",
            "No vehicles", "Vehicles over 3.5 metric tons prohibited", "No entry",
            "General caution", "Dangerous curve to the left", "Dangerous curve to the right",
            "Double curve", "Bumpy road", "Slippery road", "Road narrows on the right",
            "Road work", "Traffic signals", "Pedestrians", "Children crossing",
            "Bicycles crossing", "Beware of ice/snow", "Wild animals crossing",
            "End of all speed and passing limits", "Turn right ahead", "Turn left ahead",
            "Ahead only", "Go straight or right", "Go straight or left", "Keep right",
            "Keep left", "Roundabout mandatory", "End of no passing",
            "End of no passing by vehicles over 3.5 metric tons"
        };
    }
    std::string getLabelsName(int classId);

    static int topPrediction(const std::vector<float>& models_answers);

    static std::vector<std::pair<float, int>> topThreePredictions(const std::vector<float>& models_answers);

    static std::vector<float> softmax(const std::vector<float>& models_answers, int64_t dimensions);
};


#endif //TRAFFICSIGNRECOGNITION_POSTPROCESSOR_H