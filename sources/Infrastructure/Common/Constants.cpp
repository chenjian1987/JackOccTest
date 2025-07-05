#include "Constants.h"

using namespace JackC;

const double Constants::DOUBLE_EPS = 0.000001;

const double Constants::FLOAT_EPS = 0.001f;

const double Constants::LENGTH_EPS = 1E-2;     // 0.01mm

const double Constants::ANGLE_EPS = 1E-4;

const double Constants::MIN_MODEL_SIZE = 1.0;  // 1mm

const double Constants::MAX_MODEL_SIZE = 1E6;   // 1km

const double Constants::MIN_NEG_MODEL_SIZE = -1E6; // 1km

const double Constants::MAX_MODEL_SPACE_SIZE = 1E7; // 10km

const double Constants::DISTANCE_ITERATION_EPS = 1E-4; // 0.0001mm

const double Constants::MATH_INFINITY = 1E100;     // 全局的正无穷大值
const double Constants::MATH_NEGINFINITY = -1E100; // 全局的负无穷大值

const double Constants::MATH_PI_6 = 0.52359877559829887307; // PI/6
const double Constants::MATH_PI_4 = 0.78539816339744830961;  // PI/4
const double Constants::MATH_PI_2 = 1.57079632679489661923;  // PI/2
const double Constants::MATH_PI = 3.14159265358979323846;  // PI
const double Constants::MATH_3PI_2 = 4.71238898038468985769;  // 3PI/2
const double Constants::MATH_2PI = 6.28318530717958647692;  // 2PI

const double Constants::INCH_TO_MM = 25.4;