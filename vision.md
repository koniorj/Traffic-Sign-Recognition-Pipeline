AI UŻYTO DO:

- planowanie projektu (roadmap)
- debugging

# Traffic Sign Recognition — PyTorch Training + C++ Inference Pipeline

> A full machine learning pipeline for traffic sign classification:
>
> model trained in Python (PyTorch), deployed in C++ via ONNX Runtime.
>
> Built as a systems programming project to explore production-grade ML deployment.

---

## Project Overview

This project implements an end-to-end traffic sign recognition system inspired by real-world automotive AI pipelines used in ADAS (Advanced Driver Assistance Systems).

The workflow mirrors how ML models are developed and deployed in the automotive industry:

- **Python** is used for data preparation and model training — the fast, iterative part
- **C++** handles everything from image loading to inference and evaluation — the production-grade, performance-critical part

The model is trained on the **GTSRB dataset** (German Traffic Sign Recognition Benchmark), which contains over 50,000 images across 43 traffic sign classes. After training, the model is exported to the **ONNX** (Open Neural Network Exchange) format — an open standard used by Bosch, NVIDIA, Intel, and others for cross-platform model deployment.

---

## What C++ Does in This Project (~60–70% of codebase)

| Component       | What it does                                                                                     |
| --------------- | ------------------------------------------------------------------------------------------------ |
| `ImageLoader`   | Parses raw PPM/BMP files without external libraries — binary file I/O, buffer management         |
| `Preprocessor`  | Resizes images (bilinear interpolation), normalizes pixel values, converts to NCHW tensor format |
| `Classifier`    | Loads ONNX model, runs inference via ONNX Runtime API, manages input/output tensors              |
| `Postprocessor` | Computes softmax, extracts top-K predictions with confidence scores                              |
| `Evaluator`     | Builds confusion matrix, computes accuracy / precision / recall per class, writes results to CSV |
| `BatchRunner`   | Processes entire test folders, collects statistics, benchmarks inference time with `std::chrono` |
| Visualization   | Draws predicted label and confidence on image using OpenCV                                       |

---

## Repository Structure

```
traffic-sign-recognition/
├── python/
│   ├── train.py               # model training (PyTorch)
│   ├── export_onnx.py         # export trained model to ONNX
│   └── requirements.txt
├── cpp/
│   ├── CMakeLists.txt
│   ├── src/
│   │   ├── main.cpp
│   │   ├── image_loader.cpp   # PPM parser, raw binary I/O
│   │   ├── preprocessor.cpp   # resize, normalize, NCHW conversion
│   │   ├── classifier.cpp     # ONNX Runtime inference
│   │   ├── postprocessor.cpp  # softmax, top-K
│   │   ├── evaluator.cpp      # metrics, confusion matrix, CSV export
│   │   └── batch_runner.cpp   # full test set pipeline + benchmark
│   └── include/
│       ├── image_loader.hpp
│       ├── preprocessor.hpp
│       ├── classifier.hpp
│       ├── postprocessor.hpp
│       ├── evaluator.hpp
│       └── batch_runner.hpp
├── models/
│   └── traffic_sign_classifier.onnx
├── results/
│   └── evaluation_results.csv
└── README.md
```

---

## Tech Stack

| Tool              | Role                                             |
| ----------------- | ------------------------------------------------ |
| Python 3.10+      | Training environment                             |
| PyTorch 2.x       | Model definition and training                    |
| torchvision       | Data augmentation, pretrained weights            |
| ONNX              | Model serialization format                       |
| C++17             | Entire inference and evaluation pipeline         |
| ONNX Runtime 1.17 | C++ inference engine                             |
| OpenCV 4.x        | Image loading (JPG/PNG) and result visualization |
| CMake 3.20+       | C++ build system                                 |

---

## Roadmap

### Phase 1 — Python: Data & Training (3–4 hours)

### Step 1 — Environment setup (30 min)

- Install Python dependencies: `pip install torch torchvision onnx`
- Download GTSRB dataset from Kaggle or PyTorch datasets
- Explore dataset structure: 43 folders (one per class), PPM images, CSV annotations

**Resources:**

- GTSRB on Kaggle: https://www.kaggle.com/datasets/meowmeowmeow/gtsrb-german-traffic-sign
- PyTorch official docs — torchvision.datasets: https://pytorch.org/vision/stable/datasets.html

### Step 2 — DataLoader & augmentation (45 min)

- Write custom `Dataset` class or use `ImageFolder`
- Apply transforms: `Resize(32x32)`, `RandomHorizontalFlip`, `Normalize(mean, std)`
- Verify class balance with histogram — GTSRB is imbalanced

**Resources:**

- PyTorch DataLoader tutorial: https://pytorch.org/tutorials/beginner/data_loading_tutorial.html
- YouTube: _"Custom Dataset in PyTorch"_ — sentdex or Daniel Bourke

### Step 3 — Model definition & training (1.5–2 hours)

- Option A (recommended): MobileNetV2 with transfer learning — replace final classifier layer
- Option B: small custom CNN from scratch (~4 conv layers) — more C++-friendly understanding
- Train for 10–20 epochs, track loss and accuracy
- Save best model: `torch.save(model.state_dict(), 'best_model.pth')`

**Resources:**

- Transfer learning tutorial (PyTorch official): https://pytorch.org/tutorials/beginner/transfer_learning_tutorial.html
- YouTube: _"MobileNetV2 transfer learning PyTorch"_ — Aladdin Persson
- YouTube: _"GTSRB classification PyTorch"_

### Step 4 — Export to ONNX (30 min)

- Load trained weights, set model to eval mode
- Call `torch.onnx.export()` with a dummy input tensor
- Verify exported model with `onnx.checker.check_model()`
- Inspect model graph with Netron (free visual tool): https://netron.app

**Resources:**

- PyTorch ONNX export docs: https://pytorch.org/docs/stable/onnx.html
- YouTube: _"PyTorch export ONNX tutorial"_ — Nicolai Nielsen

---

### Phase 2 — C++: Build System & Dependencies (1–2 hours)

### Step 5 — CMake project setup (45 min)

- Write `CMakeLists.txt`: set C++17, find OpenCV, link ONNX Runtime
- Download ONNX Runtime prebuilt binaries (no compilation needed): https://github.com/microsoft/onnxruntime/releases
- Test that an empty `main.cpp` compiles and links correctly

**Resources:**

- CMake tutorial (official): https://cmake.org/cmake/help/latest/guide/tutorial/
- YouTube: _"CMake Tutorial for Beginners"_ — Code Tech
- ONNX Runtime C++ API docs: https://onnxruntime.ai/docs/api/c/

### Step 6 — First ONNX Runtime inference (45 min)

- Create `Ort::Env` and `Ort::Session`
- Load model from file path
- Print input/output tensor names and shapes
- Run inference on a zeroed dummy tensor — confirm it doesn't crash

**Resources:**

- ONNX Runtime C++ examples (official GitHub): https://github.com/microsoft/onnxruntime/tree/main/samples
- YouTube: _"ONNX Runtime C++ inference tutorial"_ — Nicolai Nielsen (search his channel directly)

---

### Phase 3 — C++: Core Pipeline (5–6 hours)

### Step 7 — ImageLoader: raw PPM parser (1 hour)

- Open file with `std::ifstream` in binary mode
- Parse ASCII header: magic number (`P6`), width, height, max value
- Read raw RGB bytes into `std::vector<uint8_t>`
- Handle edge cases: comments in header (`#`), different max values

**What you learn:** binary file I/O, `std::ifstream::read()`, buffer ownership, the difference between text and binary mode — things Python hides from you.

**Resources:**

- PPM format specification: http://netpbm.sourceforge.net/doc/ppm.html
- C++ file I/O reference: https://en.cppreference.com/w/cpp/io/basic_ifstream

### Step 8 — Preprocessor: resize & normalize (1.5 hours)

- Implement bilinear interpolation resize to 32×32
- Normalize: `pixel = (value/255.0 - mean) / std` — use same mean/std as Python training
- Convert HWC layout (OpenCV default) to NCHW (PyTorch/ONNX default)
- Store result as `std::vector<float>` — the tensor you feed to ONNX Runtime

**What you learn:** 2D array indexing math, float precision, the importance of matching preprocessing between training and inference (a common real-world bug source).

**Resources:**

- Bilinear interpolation explained: https://en.wikipedia.org/wiki/Bilinear_interpolation
- NCHW vs NHWC explained: https://oneapi-src.github.io/oneDNN/dev_guide_understanding_memory_formats.html

### Step 9 — Classifier: full inference (1 hour)

- Wrap ONNX Runtime session in a clean `Classifier` class
- Constructor loads model; destructor frees session (RAII pattern)
- `predict(std::vector<float>& tensor)` returns raw logits as `std::vector<float>`
- Use `std::unique_ptr<Ort::Session>` — practice with smart pointers

**What you learn:** RAII, smart pointers, wrapping C APIs in C++ classes, reading library documentation.

### Step 10 — Postprocessor: softmax & top-K (45 min)

- Implement softmax from scratch: `exp(x_i) / sum(exp(x_j))` — careful with numerical overflow, subtract max first
- Return top-3 predictions as `std::vector<std::pair<int, float>>` (class index, confidence)
- Map class index to human-readable label using `std::array<std::string, 43>`

**What you learn:** numerical stability in floating point, `std::pair`, `std::partial_sort`.

---

### Phase 4 — C++: Evaluation & Benchmark (2–3 hours)

### Step 11 — Evaluator: metrics & confusion matrix (1.5 hours)

- Build `std::vector<std::vector<int>> confusionMatrix(43, std::vector<int>(43, 0))`
- Compute per-class: precision, recall, F1-score
- Compute overall accuracy
- Write results to CSV with `std::ofstream`

**What you learn:** 2D vectors, nested loops over structured data, file output formatting.

**Resources:**

- Confusion matrix explained: https://scikit-learn.org/stable/modules/generated/sklearn.metrics.confusion_matrix.html

### Step 12 — BatchRunner: full test set pipeline (45 min)

- Read all image paths from directory using `std::filesystem::directory_iterator` (C++17)
- For each image: load → preprocess → classify → postprocess → record result
- Measure total and per-image inference time with `std::chrono::high_resolution_clock`
- Print summary: accuracy, avg inference time in ms, total images processed

**What you learn:** `std::filesystem`, `std::chrono`, structuring a processing loop cleanly.

### Step 13 — Visualization with OpenCV (30 min)

- Load image with `cv::imread`
- Draw predicted class label and confidence score with `cv::putText`
- Draw green bounding box with `cv::rectangle`
- Save result with `cv::imwrite` or display with `cv::imshow`

**Resources:**

- OpenCV C++ docs: https://docs.opencv.org/4.x/
- YouTube: _"OpenCV C++ tutorial beginners"_

---

### Phase 5 — Results & Documentation (1 hour)

### Step 14 — Benchmark comparison

- Compare Python inference time vs C++ inference time (measure both with the same 1000 test images)
- Expected result: C++ is 3–10× faster at batch=1 due to no interpreter overhead
- Log results in `results/evaluation_results.csv`

### Step 15 — README and GitHub

- Fill in this README with your actual results
- Add example output image (sign photo with predicted label overlaid)
- Add confusion matrix heatmap (generate in Python with matplotlib/seaborn)

---

## Key Concepts You Will Learn

**From C++ specifically:**

- Binary file parsing (`std::ifstream` in binary mode)
- Manual tensor memory layout (NCHW, contiguous float arrays)
- RAII and smart pointers (`std::unique_ptr`, destructors)
- `std::filesystem` for directory traversal (C++17)
- `std::chrono` for high-resolution benchmarking
- Linking external C libraries (ONNX Runtime) via CMake
- Numerical stability in floating point (softmax overflow trick)

**From the ML/automotive domain:**

- Why ONNX exists and what problem it solves
- The train/deploy split — why you can't always use Python in production
- Preprocessing consistency — the silent bug that kills accuracy at inference time
- Per-class evaluation metrics — global accuracy alone is misleading on imbalanced datasets

---

## Expected Results

| Metric                       | Expected value                         |
| ---------------------------- | -------------------------------------- |
| Test accuracy (Python eval)  | ~95–98%                                |
| Test accuracy (C++ pipeline) | same as Python (validates correctness) |
| Inference time — Python      | ~5–15 ms/image                         |
| Inference time — C++         | ~1–5 ms/image                          |
| C++ speedup                  | ~3–10×                                 |

---

## References

- GTSRB dataset paper: Stallkamp et al., 2012
- ONNX Runtime GitHub: https://github.com/microsoft/onnxruntime
- Nicolai Nielsen YouTube (ONNX + C++ tutorials): search "Nicolai Nielsen ONNX C++"
- PyTorch ONNX export: https://pytorch.org/docs/stable/onnx.html
- OpenCV documentation: https://docs.opencv.org/4.x/
- Netron model visualizer: https://netron.app
