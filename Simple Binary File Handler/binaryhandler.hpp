#include <fstream>
#include <stdexcept>
#include <string>

namespace nk125 {
  class binary_file_handler {
    private:
      std::string read_error = "The file specified cannot be opened.";
      std::string write_error = read_error;
      long long m_size;
    // End Od Private
    
    public:
      std::string read_file(std::string file_path) {
        std::ifstream in_file(file_path, std::ios::binary);
        std::string m_str_buff;
        char m_ch_buff;
        if (in_file.is_open()) {
          while (in_file >> std::noskipws >> m_ch_buff) {
            m_str_buff += m_ch_buff;
          }
          m_ch_buff = '\0';
          in_file.close();
          return m_str_buff;
        }
        else {
          throw std::runtime_error(read_error);
        }
      }
      
      long long size_file(std::string file_path) {
        m_size = 0;
        std::ifstream in_file(file_path, std::ios::binary);
        char m_ch_buff;
        if (in_file.is_open()) {
          while (in_file >> std::noskipws >> m_ch_buff) {
            ++m_size;
          }
          m_ch_buff = '\0';
          in_file.close();
          return m_size;
        }
        else {
          throw std::runtime_error(read_error);
        }
      }

      void write_file(std::string file_path, std::string content) {
        std::ofstream out_file(file_path, std::ios::binary);
        if (out_file.is_open()) {
          out_file.write(content.c_str(), content.size());
          out_file.close();
          return;
        }
        else {
          throw std::runtime_error(write_error);
        }
        return;
      }
      
      void append_file(std::string file_path, std::string content) {
        std::ofstream out_file(file_path, std::ios::binary | std::ios::app);
        if (out_file.is_open()) {
          out_file.write(content.c_str(), content.size());
          out_file.close();
          return;
        }
        else {
          throw std::runtime_error(write_error);
        }
        return;
      }

      void copy_file(std::string origin, std::string end) {
        std::string buffer = read_file(origin);
        write_file(end, buffer);
        return;
      }
    // End of Public
  };
}
