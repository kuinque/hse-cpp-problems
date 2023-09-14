#include "chocolate.h"

bool CanBreakSlices(int height, int width, int slices_amount) {
    if (height * width == slices_amount) {
        return false;
    } else if (slices_amount % height == 0 || slices_amount % width == 0) {
        return true;
    }
    return false;
};
