// app.cpp
#include "mathlib.h"        // the extern "C" guard does the work
#include <iostream>
#include <memory>
#include <vector>

int main()
{
    std::cout << "ml_add(3, 4) = " << ml_add(3, 4) << '\n';

    std::vector<double> data{ 1.0, 2.0, 3.0, 4.0, 5.0 };
    MlStatus status;
    double mean = ml_mean(data.data(), data.size(), &status);
    std::cout << "ml_mean = " << mean
              << " (" << ml_status_string(status) << ")\n";

    // Wrap the C handle so the destructor cannot be forgotten.
    auto acc = std::unique_ptr<MlAccumulator, decltype(&ml_acc_destroy)>(
                   ml_acc_create(), &ml_acc_destroy);
    for (double v : { 10.0, 20.0, 30.0 }) {
        ml_acc_add(acc.get(), v);
    }
    std::cout << "accumulator mean = " << ml_acc_mean(acc.get()) << '\n';

    return 0;
}
