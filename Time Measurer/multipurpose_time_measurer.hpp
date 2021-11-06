#include <chrono>
#include <cmath>
#include <mutex>
#include <thread>
#include <vector>

namespace nk125 {
  class measurer {
    private:
      int vsize_limit;
      std::mutex m_mtx_handler;

      // Calculates the average of time taken for each execution
      long long m_calc_micro_avrg() {
        if (m_timestamps.size() <= 0) {
          return 0;
        }
        m_mtx_handler.lock();
        m_tm_total = 0;
        if (vsize_limit <= m_timestamps.size()) {
          m_timestamps.erase(m_timestamps.begin());
        }
        m_mtx_handler.unlock();

        for (long long tm : m_timestamps) {
          m_tm_total += tm;
        }

        m_mtx_handler.lock();
        long long average = m_tm_total / m_timestamps.size();
        m_mtx_handler.unlock();
        return average;
      }

      float m_wrap_opt(int div = 0) {
        long long mcma = m_calc_micro_avrg();
        if (mcma == 0) {return 0;}
        else if (div == 0) {
          return float(1 / float(mcma));
        }
        else {
          return float(1 / float(mcma / float(div)));
        }
      }

      std::vector<long long> m_timestamps;
      std::chrono::time_point<std::chrono::high_resolution_clock> m_s_timestamp, m_e_timestamp;
      long long m_tm_total, m_micro_taken = 0;
      bool lock, st_bef = false;

    public:
      measurer(int vector_limit = 50) {
        if (vector_limit < 0) {
          vector_limit = 0;
        }
        vsize_limit = vector_limit;
      }

      long long get_ms_taken() {
        if (m_micro_taken) { // If m_micro_taken is not equal to 0 basically.
          return m_micro_taken / 1000;
        }
        else {
          return 0;
        }
      }

      long long get_sec_taken() {
        long long t = get_ms_taken();
        return (t == 0 ? t : t / 1000);
      }

      float op_per_micro() {
        return m_wrap_opt();
      }

      float op_per_ms() {
        return m_wrap_opt(1000);
      }

      float op_per_sec() {
        return m_wrap_opt(1000 * 1000);
      }

      void start_timestamp() {
        m_mtx_handler.lock();
        lock = false;
        st_bef = true;
        m_mtx_handler.unlock();
        m_s_timestamp = std::chrono::high_resolution_clock::now();
      }

      // start_timestamp should be called before end_timestamp
      void end_timestamp() {
        if (lock || !st_bef) return;
        m_e_timestamp = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds micro = std::chrono::duration_cast<std::chrono::microseconds>(m_e_timestamp - m_s_timestamp);
        m_micro_taken = micro.count();
        m_mtx_handler.lock();
        if (vsize_limit != 0) { 
          m_timestamps.push_back(m_micro_taken);
        }
        lock = true;
        m_mtx_handler.unlock();
      }
  };
}
