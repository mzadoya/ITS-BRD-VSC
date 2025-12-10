#include "math.h"
#include "sensor.h"

int tempCalcCelcius(Thermometer *data) {
    (*data).valueCelcius = (float)((*data).raw.temperature) - 0.25f + ((float)((*data).raw.countPerC - (*data).raw.countRemain)/(*data).raw.countPerC);
}