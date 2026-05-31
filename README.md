This project implements an end-to-end traffic sign recognition system inspired by real-world automotive AI pipelines used in ADAS (Advanced Driver Assistance Systems).

The workflow mirrors how ML models are developed and deployed in the automotive industry:

- **Python** is used for data preparation and model training - the fast, iterative part
- **C++** handles everything from image loading to inference and evaluation - the production-grade, performance-critical part

The model is trained on the **GTSRB dataset** (German Traffic Sign Recognition Benchmark), which contains over 50,000 images across 43 traffic sign classes. After training, the model is exported to the **ONNX** (Open Neural Network Exchange) format.