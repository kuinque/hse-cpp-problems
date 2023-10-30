#include <cassert>
#include <cctype>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include "input_controller.hpp"

int main(int argc, char* argv[]) {
    // std::string input_path = "/Users/krivetskyy/hse_work/image_processor/filter_images/lenna.bmp";
    // std::string output_path = "/Users/krivetskyy/hse_work/image_processor/filter_images/lenna_crop_blur.bmp";
    // std::map<std::string, int> filter_params = {{"-crop", 2},  {"-gs", 0},   {"-neg", 0},
    //                                             {"-sharp", 0}, {"-edge", 1}, {"-blur", 1}};
    // BMPImage bmp_image;
    // bmp_image.Open(input_path);
    // bmp_image.ReadBmp();
    //
    // if (argc == 1) {
    //     std::cout << "{name of program} {input path} {output path} [-{name of filter 1} [filter param 1] [filter
    //     param "
    //                  "2] ...] [-{name of filter 2} [filter param 1] [filter param 2] ...] ..."
    //               << std::endl;
    //     std::cout << "[h] filters:" << std::endl;
    //     std::cout << "[-crop] [new width] [new height] - crop image to left upper corner" << std::endl;
    //     std::cout << "[-gs] - shades of gray" << std::endl;
    //     std::cout << "[-neg] - negtive image" << std::endl;
    //     std::cout << "[-sharp] - improves sharpness" << std::endl;
    //     std::cout << "[-edge] [threshold] - edge detection" << std::endl;
    //     std::cout << "[-blur] [sigma] - gaussian blur" << std::endl;
    //     Crop crop(bmp_image.GetMatrixReference());
    //     Filter* filter = &crop;
    //     static const int64_t SZ1 = 1600;
    //     static const int64_t SZ2 = 1000;
    //     int64_t param[2] = {SZ1, SZ2};
    //     filter->ApplyFilter(param);
    //     Blur blur(bmp_image.GetMatrixReference());
    //     static const int64_t THRESHOLD = 4;
    //     filter = &blur;
    //     param[0] = {THRESHOLD};
    //     filter->ApplyFilter(param);
    //     bmp_image.SaveBmp(output_path);
    //     return 0;
    // }
    // input_path += argv[1];
    // output_path += argv[2];
    // static const int NUMBER_BASE = 10;
    // for (int argc_num = 3; argc_num < argc; ++argc_num) {
    //     if (filter_params.find(argv[argc_num]) == filter_params.end()) {
    //         for (int param_num = 1; param_num <= filter_params[argv[argc_num]]; ++param_num) {
    //             int64_t parameters[filter_params[argv[argc_num]]];
    //             if (argc_num + param_num >= argc) {
    //                 throw std::logic_error("Not enought parameters for filter " +
    //                                        static_cast<std::string>(argv[argc_num]));
    //             }
    //             char* param_pointer = nullptr;
    //             parameters[argc_num + param_num] = std::strtol(argv[argc_num + param_num], &param_pointer,
    //             NUMBER_BASE); if (!*param_pointer) {
    //                 throw std::logic_error("Wrong " + std::to_string(param_num) + " parameter for filter " +
    //                                        static_cast<std::string>(argv[argc_num]) + ".");
    //             }
    //         }
    //     }
    // }
    // bmp_image.SaveBmp(output_path);
    // std::cout << "::end of program\n";
    InputController input_controller;
    input_controller.InputFlow(argc, argv);
    return 0;
}
