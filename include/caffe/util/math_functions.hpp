#ifndef CAFFE_UTIL_MATH_FUNCTIONS_H_
#define CAFFE_UTIL_MATH_FUNCTIONS_H_

#include <stdint.h>
#include <cmath>  // for std::fabs and std::signbit

#include "glog/logging.h"

#include "caffe/common.hpp"
#include "caffe/util/device_alternate.hpp"
// #include "caffe/util/mkl_alternate.hpp"

namespace caffe {

unsigned int caffe_rng_rand();

}  // namespace caffe

#endif  // CAFFE_UTIL_MATH_FUNCTIONS_H_
