#include "chocolate.h"

bool CanBreakSlices(int height, int width, int slices_amount) {
    if ((slices_amount % height == 0 || slices_amount % width == 0) && height * width != slices_amount) {
        return true;
    }
    return false;
};
