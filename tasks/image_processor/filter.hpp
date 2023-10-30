#ifndef PRECPPPROJECT_FILTER_H
#define PRECPPPROJECT_FILTER_H

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <cmath>
#include "matrix.hpp"

class Filter {
public:
    Filter() : pixel_array_(nullptr) {
    }
    explicit Filter(Matrix* origin) : pixel_array_(origin) {
    }
    ~Filter() {
    }
    virtual void CheckParametrsValidity(int parametrs_size, char* parametrs[]) {
    }
    virtual void ApplyFilter(int parametrs_size, char* parametrs[]) {
    }

protected:
    Matrix* pixel_array_;
};

class Crop : public Filter {
public:
    explicit Crop(Matrix* origin) {
        pixel_array_ = origin;
    }
    void CheckParametrsValidity(int parametrs_size, char* parametrs[]) override {
        if (parametrs_size != PARAMETRS_SIZE) {
            throw std::runtime_error("Should be 2 crop parametrs.");
        }
        try {
            std::stoi(parametrs[0]);
        } catch (...) {
            throw std::runtime_error("Parametrs for crop not valid.");
        }
        try {
            std::stoi(parametrs[1]);
        } catch (...) {
            throw std::runtime_error("Parametrs for crop not valid.");
        }
    }
    void ApplyFilter(int parametrs_size, char* parametrs[]) override {
        CheckParametrsValidity(parametrs_size, parametrs);
        int32_t new_height = std::stoi(parametrs[1]);
        int32_t new_width = std::stoi(parametrs[0]);
        if (new_width <= 0 || new_width > pixel_array_->GetColumnsNumber() || new_height <= 0 ||
            new_height > pixel_array_->GetRowsNumber()) {
            throw std::logic_error("Canoot be cropped to that size.");
        }
        Matrix temp(new_height, new_width);
        for (int32_t x = 0; x < new_height; ++x) {
            for (int32_t y = 0; y < new_width; ++y) {
                temp.Set(x, y, pixel_array_->Get(x, y));
            }
        }
        (*pixel_array_) = temp;
    }
    static const int PARAMETRS_SIZE = 2;
};

class GS : public Filter {
public:
    explicit GS(Matrix* origin) {
        pixel_array_ = origin;
    }
    void CheckParametrsValidity(int parametrs_size, char* parametrs[]) override {
        if (parametrs_size != PARAMETRS_SIZE) {
            throw std::runtime_error("Shouldn't be gs parametrs.");
        }
    }
    void ApplyFilter(int parametrs_size, char* parametrs[]) override {
        CheckParametrsValidity(parametrs_size, parametrs);
        int32_t width = pixel_array_->GetColumnsNumber();
        int32_t height = pixel_array_->GetRowsNumber();
        Matrix temp(height, width);
        for (int32_t x = 0; x < height; ++x) {
            for (int32_t y = 0; y < width; ++y) {
                Matrix::RGB rgb = pixel_array_->Get(x, y);
                rgb.r = rgb.g = rgb.b = static_cast<uint8_t>(static_cast<double>(rgb.r) * get<0>(PIXEL_COEFFICIENT) +
                                                             static_cast<double>(rgb.g) * get<1>(PIXEL_COEFFICIENT) +
                                                             static_cast<double>(rgb.b) * get<2>(PIXEL_COEFFICIENT));
                temp.Set(x, y, rgb);
            }
        }
        (*pixel_array_) = temp;
    }

private:
    static const int PARAMETRS_SIZE = 0;
    inline static const std::tuple<const double, const double, const double> PIXEL_COEFFICIENT{0.299, 0.587, 0.114};
};

class Neg : public Filter {
public:
    explicit Neg(Matrix* origin) {
        pixel_array_ = origin;
    }
    void CheckParametrsValidity(int parametrs_size, char* parametrs[]) override {
        if (parametrs_size != PARAMETRS_SIZE) {
            throw std::runtime_error("Shouldn't be neg parametrs.");
        }
    }
    void ApplyFilter(int parametrs_size, char* parametrs[]) override {
        CheckParametrsValidity(parametrs_size, parametrs);
        int32_t width = pixel_array_->GetColumnsNumber();
        int32_t height = pixel_array_->GetRowsNumber();
        Matrix temp(height, width);
        for (int32_t x = 0; x < height; ++x) {
            for (int32_t y = 0; y < width; ++y) {
                Matrix::RGB rgb = pixel_array_->Get(x, y);
                rgb.r = Matrix::RGB::MAX_COLOR - rgb.r;
                rgb.g = Matrix::RGB::MAX_COLOR - rgb.g;
                rgb.b = Matrix::RGB::MAX_COLOR - rgb.b;
                temp.Set(x, y, rgb);
            }
        }
        (*pixel_array_) = temp;
    }

private:
    static const int PARAMETRS_SIZE = 0;
};

class Sharp : public Filter {
public:
    explicit Sharp(Matrix* origin) {
        pixel_array_ = origin;
    }
    void ChangeCurrentPixel(Matrix& temp, const int32_t x, const int32_t y) {
        Matrix::RGB rgb = pixel_array_->Get(x, y);
        std::tuple<int32_t, int32_t, int32_t> current_pixel{0, 0, 0};
        for (int32_t pixel_num = 0; pixel_num < NEAR_PIXELS; ++pixel_num) {
            std::tuple<int32_t, int32_t, int32_t> near_pixel =
                PIXEL_COEFFICIENT[pixel_num] *
                pixel_array_->GetNearest(x + WIDTH_OFFSET[pixel_num], y + HEIGHT_OFFSET[pixel_num]);
            get<0>(current_pixel) += get<0>(near_pixel);
            get<1>(current_pixel) += get<1>(near_pixel);
            get<2>(current_pixel) += get<2>(near_pixel);
        }
        rgb = current_pixel;
        temp.Set(x, y, rgb);
    }
    void CheckParametrsValidity(int parametrs_size, char* parametrs[]) override {
        if (parametrs_size != PARAMETRS_SIZE) {
            throw std::runtime_error("Shouldn't be sharp parametrs.");
        }
    }
    void ApplyFilter(int parametrs_size, char* parametrs[]) override {
        CheckParametrsValidity(parametrs_size, parametrs);
        int32_t width = pixel_array_->GetColumnsNumber();
        int32_t height = pixel_array_->GetRowsNumber();
        Matrix temp(height, width);
        for (int32_t x = 0; x < height; ++x) {
            for (int32_t y = 0; y < width; ++y) {
                ChangeCurrentPixel(temp, x, y);
            }
        }
        (*pixel_array_) = temp;
    }

private:
    static const int PARAMETRS_SIZE = 0;
    static const int32_t NEAR_PIXELS = 9;
    inline static const int32_t PIXEL_COEFFICIENT[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
    inline static const int32_t WIDTH_OFFSET[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    inline static const int32_t HEIGHT_OFFSET[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
};

class Edge : public Filter {
public:
    explicit Edge(Matrix* origin) {
        pixel_array_ = origin;
    }
    void ThresholdPixel(Matrix::RGB& origin, const uint8_t threshold) {
        if (origin.r > threshold || origin.g > threshold || origin.b > threshold) {
            origin = Matrix::WHITE_PIXEL;
            return;
        }
        origin = Matrix::BLACK_PIXEL;
    }

    void TransformPixel(Matrix& temp, const int32_t x, const int32_t y, const int32_t threshold) {
        Matrix::RGB rgb = pixel_array_->Get(x, y);
        std::tuple<int32_t, int32_t, int32_t> current_pixel{0, 0, 0};
        for (int32_t pixel_num = 0; pixel_num < NEAR_PIXELS; ++pixel_num) {
            std::tuple<int32_t, int32_t, int32_t> near_pixel =
                PIXEL_COEFFICIENT[pixel_num] *
                pixel_array_->GetNearest(x + WIDTH_OFFSET[pixel_num], y + HEIGHT_OFFSET[pixel_num]);
            get<0>(current_pixel) += get<0>(near_pixel);
            get<1>(current_pixel) += get<1>(near_pixel);
            get<2>(current_pixel) += get<2>(near_pixel);
        }
        rgb = current_pixel;
        ThresholdPixel(rgb, threshold);
        temp.Set(x, y, rgb);
    }
    void CheckParametrsValidity(int parametrs_size, char* parametrs[]) override {
        if (parametrs_size != PARAMETRS_SIZE) {
            throw std::runtime_error("Should be 1 edge parametr.");
        }
        try {
            std::stod(parametrs[0]);
        } catch (...) {
            throw std::runtime_error("Parametr for edge not valid.");
        }
    }

    void ApplyFilter(int parametrs_size, char* parametrs[]) override {
        CheckParametrsValidity(parametrs_size, parametrs);
        uint8_t threshold = static_cast<uint8_t>(round(std::stod(parametrs[0])));
        if (threshold < 0 || threshold > Matrix::RGB::MAX_COLOR) {
            throw std::logic_error("Threshold should be in range [0, 1].");
        }
        GS gs{pixel_array_};
        gs.ApplyFilter(0, {});
        int32_t width = pixel_array_->GetColumnsNumber();
        int32_t height = pixel_array_->GetRowsNumber();
        Matrix temp(height, width);
        for (int32_t x = 0; x < height; ++x) {
            for (int32_t y = 0; y < width; ++y) {
                TransformPixel(temp, x, y, threshold);
            }
        }
        (*pixel_array_) = temp;
    }

private:
    static const int PARAMETRS_SIZE = 1;
    static const int32_t NEAR_PIXELS = 9;
    inline static const int32_t PIXEL_COEFFICIENT[9] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    inline static const int32_t WIDTH_OFFSET[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    inline static const int32_t HEIGHT_OFFSET[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
};

class Blur : public Filter {
public:
    explicit Blur(Matrix* origin) {
        pixel_array_ = origin;
    }
    void PixelContribution(std::tuple<double, double, double>& origin, const int32_t x0, const int32_t y0,
                           const int32_t x, int32_t y, double sigma_mult) {
        double coefficient = static_cast<double>(abs(x0 - x) * abs(x0 - x) + abs(y0 - y) * abs(y0 - y)) / sigma_mult;
        coefficient = 1.0 / pow(E, coefficient);
        get<0>(origin) *= coefficient;
        get<1>(origin) *= coefficient;
        get<2>(origin) *= coefficient;
    }

    void HeightCalculate(std::tuple<double, double, double>** temp, const int32_t height, const int32_t width,
                         const double common_multiplier, const double sigma_mult, const int32_t sigma) {
        for (int32_t x0 = 0; x0 < height; ++x0) {
            for (int32_t y0 = 0; y0 < width; ++y0) {
                // no sense to cosider bigger matrix
                temp[x0][y0] = std::make_tuple(0.0, 0.0, 0.0);
                for (int32_t x_offset = -MATRIX_CONSIDER_SIZE * sigma; x_offset <= MATRIX_CONSIDER_SIZE * sigma;
                     ++x_offset) {
                    int32_t x = x0 + x_offset;
                    int32_t y = y0;
                    Matrix::RGB rgb = pixel_array_->GetNearest(x, y);
                    std::tuple<double, double, double> next_pixel = std::make_tuple(
                        static_cast<double>(rgb.r), static_cast<double>(rgb.g), static_cast<double>(rgb.b));
                    PixelContribution(next_pixel, x0, y0, x, y, sigma_mult);
                    get<0>(temp[x0][y0]) += get<0>(next_pixel);
                    get<1>(temp[x0][y0]) += get<1>(next_pixel);
                    get<2>(temp[x0][y0]) += get<2>(next_pixel);
                }
            }
        }
    }

    void NearestCoordinates(int32_t& x, int32_t& y, const int32_t height, const int32_t width) const {
        x = std::max(0, std::min(height - 1, x));
        y = std::max(0, std::min(width - 1, y));
    }

    void WidthCalculate(std::tuple<double, double, double>** temp, const int32_t height, const int32_t width,
                        const double common_multiplier, const double sigma_mult, const int32_t sigma) {
        for (int32_t x0 = 0; x0 < height; ++x0) {
            for (int32_t y0 = 0; y0 < width; ++y0) {
                std::tuple<double, double, double> pixels_sum{0, 0, 0};
                // no sense to cosider bigger matrix
                for (int32_t y_offset = -MATRIX_CONSIDER_SIZE * sigma; y_offset <= MATRIX_CONSIDER_SIZE * sigma;
                     ++y_offset) {
                    int32_t x = x0;
                    int32_t y = y0 + y_offset;
                    NearestCoordinates(x, y, height, width);
                    std::tuple<double, double, double> near_pixel = temp[x][y];
                    PixelContribution(near_pixel, x0, y0, x0, y0 + y_offset, sigma_mult);
                    get<0>(pixels_sum) += get<0>(near_pixel);
                    get<1>(pixels_sum) += get<1>(near_pixel);
                    get<2>(pixels_sum) += get<2>(near_pixel);
                }
                get<0>(pixels_sum) *= common_multiplier;
                get<1>(pixels_sum) *= common_multiplier;
                get<2>(pixels_sum) *= common_multiplier;
                Matrix::RGB rgb{static_cast<uint8_t>(round(get<0>(pixels_sum))),
                                static_cast<uint8_t>(round(get<1>(pixels_sum))),
                                static_cast<uint8_t>(round(get<2>(pixels_sum)))};
                pixel_array_->Set(x0, y0, rgb);
            }
        }
    }
    void CheckParametrsValidity(int parametrs_size, char* parametrs[]) override {
        if (parametrs_size != PARAMETRS_SIZE) {
            throw std::runtime_error("Should be 1 blur parametrs.");
        }
        try {
            std::stod(parametrs[0]);
        } catch (...) {
            throw std::runtime_error("Parametr for blur not valid.");
        }
    }
    void ApplyFilter(int parametrs_size, char* parametrs[]) override {
        CheckParametrsValidity(parametrs_size, parametrs);
        double sigma = std::stod(parametrs[0]);
        if (sigma < 0) {
            throw std::logic_error("Sigma should be at least zero.");
        }
        int32_t width = pixel_array_->GetColumnsNumber();
        int32_t height = pixel_array_->GetRowsNumber();
        std::tuple<double, double, double>** double_pixels = new std::tuple<double, double, double>*[height];
        for (int x = 0; x < height; ++x) {
            double_pixels[x] = new std::tuple<double, double, double>[width];
        }
        double sigma_mult = (SIGMA_COEFFICIENT * sigma * sigma);
        double common_multiplier = 1.0 / (sigma_mult * PI);
        HeightCalculate(double_pixels, height, width, common_multiplier, sigma_mult, static_cast<int32_t>(sigma));
        WidthCalculate(double_pixels, height, width, common_multiplier, sigma_mult, static_cast<int32_t>(sigma));

        for (int x = 0; x < height; ++x) {
            delete[] double_pixels[x];
        }
        delete[] double_pixels;
    }

private:
    static const int PARAMETRS_SIZE = 1;
    static const int32_t MATRIX_CONSIDER_SIZE = 3;
    inline static const double SIGMA_COEFFICIENT = 2.0;
    inline static const double PI = std::atan(1.0) * 4.0;
    inline static const double E = std::exp(1.0);
};

#endif  // PRECPPPROJECT_FILTER_H
