#include "multipurpose_time_measurer.hpp"
#include <chrono>
#include <iostream>
#include <future>
using namespace std;

void await() {
  using namespace chrono_literals;
  this_thread::sleep_for(2s);
}

int main() {
  // This need to be too verbose, sorry if there's too much cout's
  nk125::measurer ms;
  cout << "Test 1 (get_ms_taken, no initialization before): " << ms.get_ms_taken() << endl;
  cout << "Test 2 (end_timestamp, no initialization before)\n";
  ms.end_timestamp();
  cout << "Test 2 Finished\n";
  while (true) {
    cout << "Test 3 (Async start)\n";
    async(launch::async, [&ms]{ms.start_timestamp();});
    cout << "Test 3 Finished\n";
    cout << "Awaiting...\n";
    await();
    cout << "Awaited.\n";
    cout << "Test 4 (Async ending)\n";
    async(launch::async, [&ms]{ms.end_timestamp();});
    cout << "Test 4 Finished\n";
    cout << "Test 5 (Double ending timestamps)\n";
    ms.end_timestamp();
    cout << "Test 5 Finished\n";
    cout << "Awaited " << ms.get_ms_taken() << " milliseconds\n";
    cout << "Awaited " << ms.get_sec_taken() << " milliseconds\n";
    cout << "Operations per microsecond: " << ms.op_per_micro() << endl;
    cout << "Operations per millisecond: " << ms.op_per_ms() << endl;
    cout << "Operations per second: " << ms.op_per_sec() << "\n\n";
  }
}
