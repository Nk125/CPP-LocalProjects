#include <fstream>
#include <stdexcept>
#include <string>

namespace nk125 {
  class binary_file_handler {
    private:
      std::string notallowed = "*?<>|\n"; // Windows Default Disabled Chars

      std::string sanitize(std::string file_name) {
          int index = 0;
          std::string buffer = file_name;
          for (char c : buffer) {
              if (notallowed.find(c) != std::string::npos) {
                  buffer.erase(index, 1);
              }
              index++;
          }
          return buffer;
      }

      std::string read_error = "The file specified cannot be opened.";
      std::string write_error = read_error;
      long long m_size;
    // End Of Private

    public:
      void set_not_allowed_chars(std::string chars) {
          notallowed = chars;
      }

      std::string read_file(std::string file_path) {
        file_path = sanitize(file_path);
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

      std::string fast_read_file(std::string file_path) {
        // I recommend it when you have to quickly read mid-size files with 5+ MB (or super big files with GB's of size)
        // Instead you should use the safe method read_file
        file_path = sanitize(file_path);
        std::ifstream in_file(file_path, std::ios::binary);
        std::string m_str_buff;
        if (in_file.is_open()) {
          in_file.seekg(0, std::ios_base::end);
          long long sz = in_file.tellg();
          char* m_ch_buff = new char[sz];
          in_file.seekg(0, std::ios_base::beg);
          in_file.read(m_ch_buff, sz);
          m_str_buff.assign(m_ch_buff, sz);
          in_file.close();
          delete[] m_ch_buff;
          return m_str_buff;
        }
        else {
          throw std::runtime_error(read_error);
        }
      }

      long long size_file(std::string file_path) {
        file_path = sanitize(file_path);
        std::ifstream sz_file(file_path, std::ios::binary);
        if (sz_file.is_open()) {
          sz_file.seekg(0, std::ios_base::end);
          long long m_fsize = sz_file.tellg();
          sz_file.close();
          return m_fsize;
        }
        else {
          throw std::runtime_error(read_error);
        }
      }

      void write_file(std::string file_path, std::string content) {
        file_path = sanitize(file_path);
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
        file_path = sanitize(file_path);
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

      void fast_copy_file(std::string origin, std::string end) {
        std::string buffer = fast_read_file(origin);
        write_file(end, buffer);
        return;
      }
    // End of Public
  };
}
