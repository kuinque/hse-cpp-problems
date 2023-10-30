#ifndef PRECPPPROJECT_INPUT_CONTROLLER_H
#define PRECPPPROJECT_INPUT_CONTROLLER_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <map>

#include "bmp_image.hpp"
#include "filter.hpp"

class InputController {
public:
    void InputFlow(const int argc, char* argv[]) {
        // std::string input_path = "/Users/krivetskyy/hse_work/image_processor/filter_images/lenna.bmp";
        // std::string output_path = "/Users/krivetskyy/hse_work/image_processor/filter_images/lenna_crop_blur.bmp";

        if (argc == 1) {
            InputInfo();
            return;
        }
        if (argc == 2) {
            throw std::runtime_error("Output path didn't specifyed.");
        }
        std::string input_path = argv[1];
        std::string output_path = argv[2];
        BMPImage image;
        image.Open(input_path);
        pixel_array_ = image.GetMatrixReference();
        throw std::logic_error(std::to_string(pixel_array_->GetColumnsNumber()));
        InputAnalyze(argc, argv);
        image.SaveBmp(output_path);
    }

private:
    void InputInfo() {
        std::cout << "{name of program} {input path} {output path} [-{name of filter 1} [filter param 1] [filter param "
                     "2] ...] [-{name of filter 2} [filter param 1] [filter param 2] ...] ..."
                  << std::endl;
        std::cout << "[h] filters:" << std::endl;
        std::cout << "[-crop] [new width] [new height] - crop image to left upper corner" << std::endl;
        std::cout << "[-gs] - shades of gray" << std::endl;
        std::cout << "[-neg] - negtive image" << std::endl;
        std::cout << "[-sharp] - improves sharpness" << std::endl;
        std::cout << "[-edge] [threshold] - edge detection" << std::endl;
        std::cout << "[-blur] [sigma] - gaussian blur" << std::endl;
    }

    void InputAnalyze(const int argc, char* argv[]) {
        for (int argc_num = 3; argc_num < argc; ++argc_num) {
            if (argv[argc_num][0] != '-') {
                throw std::runtime_error("Filter name should begin from -.");
            }
            if (filter_map_.find(argv[argc_num]) == filter_map_.end()) {
                throw std::runtime_error("Wrong filter name.");
            }
            int parametr_num = 0;
            while (argc_num + parametr_num + 1 < argc && argv[argc_num + parametr_num + 1][0] != '-') {
                ++parametr_num;
            }
            std::string filter_name = argv[argc_num];
            if (filter_name == "-crop") {
                Crop crop{pixel_array_};
                crop.ApplyFilter(parametr_num, argv + argc_num + 1);
            }
            if (filter_name == "-gs") {
                GS gs{pixel_array_};
                gs.ApplyFilter(parametr_num, argv + argc_num + 1);
            }
            if (filter_name == "-neg") {
                Neg neg{pixel_array_};
                neg.ApplyFilter(parametr_num, argv + argc_num + 1);
            }
            if (filter_name == "-sharp") {
                Sharp sharp{pixel_array_};
                sharp.ApplyFilter(parametr_num, argv + argc_num + 1);
            }
            if (filter_name == "-edge") {
                Edge edge{pixel_array_};
                edge.ApplyFilter(parametr_num, argv + argc_num + 1);
            }
            if (filter_name == "-blur") {
                Blur blur{pixel_array_};
                blur.ApplyFilter(parametr_num, argv + argc_num + 1);
            }
            argc_num += parametr_num;
        }
    }

private:
    Matrix* pixel_array_;
    std::map<std::string, int> filter_map_ = {{"-crop", 2},  {"-gs", 0},   {"-neg", 0},
                                              {"-sharp", 0}, {"-edge", 1}, {"-blur", 1}};
};

#endif  // PRECPPPROJECT_IMAGE_CONTROLLER_H
