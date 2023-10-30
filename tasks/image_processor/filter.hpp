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

// crop filter
class Crop : public Filter {
public:
    explicit Crop(Matrix* origin) {
        pixel_array_ = origin;
    }
    // check if all parametrs are valid
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
    // apply filter
    void ApplyFilter(int parametrs_size, char* parametrs[]) override {
        CheckParametrsValidity(parametrs_size, parametrs);
        int32_t new_height = std::stoi(parametrs[1]);
        int32_t new_width = std::stoi(parametrs[0]);
        // check if new values are lower than 0
        if (new_width <= 0 || new_height <= 0) {
            throw std::logic_error("Canoot be cropped to that size.");
        }
        new_height = std::min(pixel_array_->GetRowsNumber(), new_height);
        new_width = std::min(pixel_array_->GetColumnsNumber(), new_width);
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

// grey scale filter
class GS : public Filter {
public:
    explicit GS(Matrix* origin) {
        pixel_array_ = origin;
    }
    // check if parametrs are valid
    void CheckParametrsValidity(int parametrs_size, char* parametrs[]) override {
        if (parametrs_size != PARAMETRS_SIZE) {
            throw std::runtime_error("Shouldn't be gs parametrs.");
        }
    }
    // apply filter
    void ApplyFilter(int parametrs_size, char* parametrs[]) override {
        CheckParametrsValidity(parametrs_size, parametrs);
        int32_t width = pixel_array_->GetColumnsNumber();
        int32_t height = pixel_array_->GetRowsNumber();
        Matrix temp(height, width);
        for (int32_t x = 0; x < height; ++x) {
            for (int32_t y = 0; y < width; ++y) {
                // calculating new value for pixel
                Matrix::RGB rgb = pixel_array_->Get(x, y);
                uint8_t new_color = static_cast<uint8_t>(round(get<0>(PIXEL_COEFFICIENT) * static_cast<double>(rgb.r) +
                                                               get<1>(PIXEL_COEFFICIENT) * static_cast<double>(rgb.g) +
                                                               get<2>(PIXEL_COEFFICIENT) * static_cast<double>(rgb.b)));
                rgb.r = new_color;
                rgb.g = new_color;
                rgb.b = new_color;
                temp.Set(x, y, rgb);
            }
        }
        (*pixel_array_) = temp;
    }

private:
    static const int PARAMETRS_SIZE = 0;
    inline static const std::tuple<const double, const double, const double> PIXEL_COEFFICIENT{0.299, 0.587, 0.114};
};

// negative filter
class Neg : public Filter {
public:
    explicit Neg(Matrix* origin) {
        pixel_array_ = origin;
    }
    // check if parametrs are valid
    void CheckParametrsValidity(int parametrs_size, char* parametrs[]) override {
        if (parametrs_size != PARAMETRS_SIZE) {
            throw std::runtime_error("Shouldn't be neg parametrs.");
        }
    }
    // apply filter
    void ApplyFilter(int parametrs_size, char* parametrs[]) override {
        CheckParametrsValidity(parametrs_size, parametrs);
        int32_t width = pixel_array_->GetColumnsNumber();
        int32_t height = pixel_array_->GetRowsNumber();
        Matrix temp(height, width);
        for (int32_t x = 0; x < height; ++x) {
            for (int32_t y = 0; y < width; ++y) {
                // calculating new values
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

// sharping filter
class Sharp : public Filter {
public:
    explicit Sharp(Matrix* origin) {
        pixel_array_ = origin;
    }
    // culculates new pixel culors
    void ChangeCurrentPixel(Matrix& temp, const int32_t x, const int32_t y) {
        Matrix::RGB rgb = pixel_array_->Get(x, y);
        std::tuple<int32_t, int32_t, int32_t> current_pixel{0, 0, 0};
        for (int32_t pixel_num = 0; pixel_num < NEAR_PIXELS; ++pixel_num) {
            // constants due to pixel matrix
            std::tuple<int32_t, int32_t, int32_t> near_pixel =
                PIXEL_COEFFICIENT[pixel_num] *
                pixel_array_->GetNearest(x + HEIGHT_OFFSET[pixel_num], y + WIDTH_OFFSET[pixel_num]);
            get<0>(current_pixel) += get<0>(near_pixel);
            get<1>(current_pixel) += get<1>(near_pixel);
            get<2>(current_pixel) += get<2>(near_pixel);
        }
        rgb = current_pixel;
        temp.Set(x, y, rgb);
    }
    // checks if parametrs are valid
    void CheckParametrsValidity(int parametrs_size, char* parametrs[]) override {
        if (parametrs_size != PARAMETRS_SIZE) {
            throw std::runtime_error("Shouldn't be sharp parametrs.");
        }
    }
    // apply filter
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

// edge filter
class Edge : public Filter {
public:
    explicit Edge(Matrix* origin) {
        pixel_array_ = origin;
    }
    // applying pixel with threshold
    void ThresholdPixel(Matrix::RGB& origin, const uint8_t threshold) {
        if (origin.r > threshold) {
            origin = Matrix::WHITE_PIXEL;
            return;
        }
        origin = Matrix::BLACK_PIXEL;
    }
    // transforms pixel
    void TransformPixel(Matrix& temp, const int32_t x, const int32_t y, const int32_t threshold) {
        Matrix::RGB rgb = pixel_array_->Get(x, y);
        std::tuple<int32_t, int32_t, int32_t> current_pixel{0, 0, 0};
        for (int32_t pixel_num = 0; pixel_num < NEAR_PIXELS; ++pixel_num) {
            // constants sue to pixel matrix
            std::tuple<int32_t, int32_t, int32_t> near_pixel =
                PIXEL_COEFFICIENT[pixel_num] *
                pixel_array_->GetNearest(x + HEIGHT_OFFSET[pixel_num], y + WIDTH_OFFSET[pixel_num]);
            get<0>(current_pixel) += get<0>(near_pixel);
            get<1>(current_pixel) += get<1>(near_pixel);
            get<2>(current_pixel) += get<2>(near_pixel);
        }
        rgb = current_pixel;
        ThresholdPixel(rgb, threshold);
        temp.Set(x, y, rgb);
    }
    // checks if parametrs are valid
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
    // apply filter
    void ApplyFilter(int parametrs_size, char* parametrs[]) override {
        CheckParametrsValidity(parametrs_size, parametrs);
        uint8_t threshold =
            static_cast<uint8_t>(round(std::stod(parametrs[0]) * static_cast<double>(Matrix::RGB::MAX_COLOR)));
        if (threshold < 0 || threshold > Matrix::RGB::MAX_COLOR) {
            throw std::logic_error("Threshold should be in range [0, 1].");
        }
        // applying grey scales
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
    // calculates pixel contribution
    void PixelContribution(std::tuple<double, double, double>& origin, const int32_t xy_offset,
                           double* offset_coefficient) {
        double coefficient = offset_coefficient[xy_offset];
        get<0>(origin) *= coefficient;
        get<1>(origin) *= coefficient;
        get<2>(origin) *= coefficient;
    }
    // calculates vertically
    void HeightCalculate(std::tuple<double, double, double>** temp, const int32_t height, const int32_t width,
                         const double common_multiplier, const double sigma_mult, const int32_t sigma,
                         double* offset_coefficient) {
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
                    PixelContribution(next_pixel, MATRIX_CONSIDER_SIZE * sigma + x_offset, offset_coefficient);
                    get<0>(temp[x0][y0]) += get<0>(next_pixel);
                    get<1>(temp[x0][y0]) += get<1>(next_pixel);
                    get<2>(temp[x0][y0]) += get<2>(next_pixel);
                }
            }
        }
    }
    // finds neares coordinates for x and y
    void NearestCoordinates(int32_t& x, int32_t& y, const int32_t height, const int32_t width) const {
        x = std::max(0, std::min(height - 1, x));
        y = std::max(0, std::min(width - 1, y));
    }
    // calculates horizontally
    void WidthCalculate(std::tuple<double, double, double>** temp, const int32_t height, const int32_t width,
                        const double common_multiplier, const double sigma_mult, const int32_t sigma,
                        double* offset_coefficient) {
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
                    PixelContribution(near_pixel, MATRIX_CONSIDER_SIZE * sigma + y_offset, offset_coefficient);
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
    // checks if parametrs are valid
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
    // apply filter
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
        // calculates necessary multipliers
        double sigma_mult = (SIGMA_COEFFICIENT * sigma * sigma);
        double common_multiplier = 1.0 / (sigma_mult * PI);
        int32_t sigma_rounded = static_cast<int32_t>(round(sigma));
        // pre-calc of horizontal/vertical offset coefficients
        double* offset_coefficient = new double[MATRIX_CONSIDER_SIZE * sigma_rounded * 2 + 2];
        for (int32_t xy_offset = -MATRIX_CONSIDER_SIZE * sigma_rounded;
             xy_offset <= MATRIX_CONSIDER_SIZE * sigma_rounded; ++xy_offset) {
            double coefficient = static_cast<double>(xy_offset * xy_offset) / sigma_mult;
            coefficient = 1.0 / pow(E, coefficient);
            offset_coefficient[MATRIX_CONSIDER_SIZE * sigma_rounded + xy_offset] = coefficient;
        }

        HeightCalculate(double_pixels, height, width, common_multiplier, sigma_mult, sigma_rounded, offset_coefficient);
        WidthCalculate(double_pixels, height, width, common_multiplier, sigma_mult, sigma_rounded, offset_coefficient);

        delete[] offset_coefficient;
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

// retro filter
class Retro : public Filter {
public:
    explicit Retro(Matrix* origin) {
        pixel_array_ = origin;
    }
    // checks if parametrs are valid
    void CheckParametrsValidity(int parametrs_size, char* parametrs[]) override {
        if (parametrs_size != PARAMETRS_SIZE) {
            throw std::runtime_error("Shouldn't be neg parametrs.");
        }
    }
    // apply filter
    void ApplyFilter(int parametrs_size, char* parametrs[]) override {
        CheckParametrsValidity(parametrs_size, parametrs);
        int32_t width = pixel_array_->GetColumnsNumber();
        int32_t height = pixel_array_->GetRowsNumber();
        Matrix temp(height, width);
        for (int32_t x = 0; x < height; ++x) {
            for (int32_t y = 0; y < width; ++y) {
                Matrix::RGB rgb = pixel_array_->Get(x, y);
                std::tuple<int32_t, int32_t, int32_t> pixel = {static_cast<int32_t>(rgb.r), static_cast<int32_t>(rgb.g),
                                                               static_cast<int32_t>(rgb.b)};
                // reduces brightness
                get<0>(pixel) -= REDUCE_BRIGHTNESS;
                get<1>(pixel) -= REDUCE_BRIGHTNESS;
                get<2>(pixel) -= REDUCE_BRIGHTNESS;
                // contrasts picture
                get<0>(pixel) = static_cast<int32_t>(round(static_cast<double>(get<0>(pixel)) * CONTRAST_COEFFICIENT));
                get<1>(pixel) = static_cast<int32_t>(round(static_cast<double>(get<1>(pixel)) * CONTRAST_COEFFICIENT));
                get<2>(pixel) = static_cast<int32_t>(round(static_cast<double>(get<1>(pixel)) * CONTRAST_COEFFICIENT));
                // reconfiguration of colors
                get<0>(pixel) += COLOR_CORRECTION;
                get<1>(pixel) -= COLOR_CORRECTION;
                get<2>(pixel) += COLOR_CORRECTION;
                // random noise on picture
                int32_t noise_r = rand() % (RANDOM_NOISE * 2);
                int32_t noise_g = rand() % (RANDOM_NOISE * 2);
                int32_t noise_b = rand() % (RANDOM_NOISE * 2);
                get<0>(pixel) += noise_r - RANDOM_NOISE;
                get<1>(pixel) += noise_g - RANDOM_NOISE;
                get<2>(pixel) += noise_b - RANDOM_NOISE;

                rgb = pixel;
                temp.Set(x, y, rgb);
            }
        }
        (*pixel_array_) = temp;
    }

private:
    static const int PARAMETRS_SIZE = 0;
    static const int32_t REDUCE_BRIGHTNESS = -30;
    inline static const double CONTRAST_COEFFICIENT = 1.2;
    static const int32_t COLOR_CORRECTION = 10;
    static const int32_t RANDOM_NOISE = 10;
};

#endif  // PRECPPPROJECT_FILTER_H
