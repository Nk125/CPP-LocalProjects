#include "multipurpose_time_measurer.hpp"
#include <chrono>
#include <iostream>
using namespace std;

void await() {
  using namespace chrono_literals;
  this_thread::sleep_for(2s);
}

int main() {
  nk125::measurer msr(10);
  while (true) {
    msr.start_timestamp();
    await();
    msr.end_timestamp();
    cout << "Awaited " << msr.get_ms_taken() << " milliseconds\n";
    cout << "Operations per microsecond: " << msr.op_per_micro() << endl;
    cout << "Operations per millisecond: " << msr.op_per_ms() << endl;
    cout << "Operations per second: " << msr.op_per_sec() << endl;
  }
}
