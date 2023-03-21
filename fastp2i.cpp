#include <algorithm>
#include <vector>
#include <iostream>
#include <numeric>
#include <cstdlib>
#include <cmath>

#ifndef P2ITEST
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Python.h"
#endif

using namespace std;

static const int MAX_PROBS = 50;

constexpr long factorial(const int n) {
    long f = 1;
    for (int i = 1; i <= n; ++i) {
      f = f * i;
    }
    return f;
}

vector<long> factorial_lookup = [] {
  vector<long> ret;
  for(int n = 0; n < MAX_PROBS; n++) {
    ret.push_back(factorial(n));
  }
  return ret;
}();

static const vector<vector<int>> indices_without_i_lookup = [] {
  vector<vector<int>> lookup;
  for(int i = 0; i < MAX_PROBS; i++) {
    vector<int> indices(MAX_PROBS);
    iota(indices.begin(), indices.end(), 0);
    indices.erase(indices.begin() + i);
    lookup.push_back(indices);
  }

  return lookup;
}();

long combos(int n, int r) { return factorial(n) / (factorial(r) * factorial(n - r)); }

double p2i(const vector<double>& probs, int excludeIndex, const vector<double>& pni) {

  double ret = 0.0;
  int totalSize = probs.size();
  int n = totalSize - 1;
  const vector<int>& offsetIndices = indices_without_i_lookup[excludeIndex];
  static vector<int> indices_(MAX_PROBS);

  for(int comboSize = totalSize - 1; comboSize > 2; comboSize--) {

    double product = 1;
    std::iota(indices_.begin(), indices_.begin() + comboSize, 0);
    for (int i = 0; i < comboSize; ++i) {
      product = product * probs[offsetIndices[indices_[i]]];
    }

    double sum = product;

    while(true) {

      int i;
      for (i = comboSize - 1; i >= 0; --i) {
        if (indices_[i] != i + n - comboSize) {
          break;
        }
        if (i == 0) {
          goto done;
        }
      }
      indices_[i] += 1;
      for (int j = i + 1; j < comboSize; ++j) {
        indices_[j] = indices_[j - 1] + 1;
      }

      double product = 1.0;
      for(int c = 0; c < comboSize; c++) {
        product = product * probs[offsetIndices[indices_[c]]];
      }

      sum = sum + product;
    };
done:
    ret += factorial_lookup[comboSize + 1] * sum;
  }

  double sumXWithoutI = -probs[excludeIndex];
  for(double p : probs) {
    sumXWithoutI += p;
  }

  ret += 2 * sumXWithoutI + 1;

  return ret * pow(probs[excludeIndex], 2) - pni[excludeIndex];
}

#ifndef P2ITEST
namespace py = pybind11;

PYBIND11_MODULE(fastp2i, m) {
    m.def("p2i", &p2i, R"pbdoc(
        Compute the p2i function using fast C++
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

#endif

#ifdef P2ITEST
int main(int argc, char** argv) {

  if(argc == 2 && string(argv[1]) == "p2i") {
    vector<double> x {1,2,3,4,5,6,7};
    cout << p2i(x, 1, x) << endl;
    return 0;
  } else if(argc == 2 && string(argv[1]) == "p2i_timed") {
    vector<double> x {1,2,3,4,5,6,7};

    for(int t = 0; t < 1000000; t++) {
      p2i(x, 1, x);
    }
    return 0;
  }

  return 0;
}
#endif