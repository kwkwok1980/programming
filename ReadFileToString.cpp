#include <vector>
#include <iostream>
#include <set>
#include <tuple>
#include <algorithm>
#include <cstring>
#include <chrono>
#include <fstream>
#include <sstream>
#include <memory>
#include <fcntl.h>
#include <sys/mman.h>
#include <zconf.h>

/*
--- memmap ---
220435968
19270439
                                                            D�@5           �:      0�            
--- seek + vector char ---
220435968
99655584
                                                            D�@5           �:      0�            
--- seek + string ---
220435968
100011536
                                                            D�@5           �:      0�            
--- seek + char buffer ---
220435968
174909269
                                                            D�@5           �:      0�            
--- string stream ---
220435968
329810900
                                                            D�@5           �:      0�            
--- istreambuf iterator ---
220435968
9859478833
                                                            D�@5           �:      0�          
*/

int main()
{
   size_t size{0};
   std::string sFileName = "/home/hkwok/dev03/tmp/in_xnse_order_420_sgahi_csh_std_fdc_df.exec_book.dat";
   {
      std::ifstream fs{sFileName};
      fs.seekg(0, fs.end);
      size = static_cast<size_t>(fs.tellg());
   }

   {
      std::cout << "--- memmap ---" << std::endl;
      auto start = std::chrono::high_resolution_clock::now();

      int fd = open(sFileName.c_str(), O_RDONLY, 0);
      auto* mmapData = mmap(nullptr, size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
      auto* pBuffer = reinterpret_cast<char *>(mmapData);
      std::string sLine {pBuffer + size - 100, pBuffer + size};
      munmap(mmapData, size);
      close(fd);

      auto end = std::chrono::high_resolution_clock::now();
      std::cout << size << std::endl;
      std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << std::endl;
      std::cout << sLine << std::endl;
   }

   {
      std::cout << "--- seek + vector char ---" << std::endl;
      auto start = std::chrono::high_resolution_clock::now();

      std::vector<char> buffer (size);
      std::ifstream fs{sFileName};
      fs.seekg(0, fs.beg);
      fs.read(&buffer[0], size);
      std::string line{&buffer[0] + size - 100, &buffer[0] + size};

      auto end = std::chrono::high_resolution_clock::now();
      std::cout << size << std::endl;
      std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << std::endl;
      std::cout << line << std::endl;
   }

   {
      std::cout << "--- seek + string ---" << std::endl;
      auto start = std::chrono::high_resolution_clock::now();

      std::string sBuffer;
      sBuffer.resize(size);

      std::ifstream fs{sFileName};
      fs.seekg(0, fs.beg);
      fs.read(&sBuffer[0], size);
      std::string line = sBuffer.substr(sBuffer.size()-100);

      auto end = std::chrono::high_resolution_clock::now();
      std::cout << sBuffer.size() << std::endl;
      std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << std::endl;
      std::cout << line << std::endl;
   }

   {
      std::cout << "--- seek + char buffer ---" << std::endl;
      auto start = std::chrono::high_resolution_clock::now();

      std::unique_ptr<char[]> bufferPtr= std::make_unique<char[]>(size);
      std::ifstream fs{sFileName};
      fs.seekg(0, fs.beg);
      fs.read(&bufferPtr[0], size);
      std::string line{&bufferPtr[0] + size - 100, &bufferPtr[0] + size};

      auto end = std::chrono::high_resolution_clock::now();
      std::cout << size << std::endl;
      std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << std::endl;
      std::cout << line << std::endl;
   }

   {
      std::cout << "--- string stream ---" << std::endl;
      auto start = std::chrono::high_resolution_clock::now();

      std::stringstream ssBuffer;
      ssBuffer.str().resize(size);

      std::ifstream fs{sFileName};
      ssBuffer << fs.rdbuf();
      std::string sLine = ssBuffer.str().substr(ssBuffer.str().size()-100);

      auto end = std::chrono::high_resolution_clock::now();
      std::cout << ssBuffer.str().size() << std::endl;
      std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << std::endl;
      std::cout << sLine << std::endl;
   }

   {
      std::cout << "--- istreambuf iterator ---" << std::endl;
      auto start = std::chrono::high_resolution_clock::now();

      std::string sBuffer;
      //sBuffer.resize(size);

      std::ifstream fs{sFileName};
      sBuffer.assign(std::istreambuf_iterator<char>{fs}, std::istreambuf_iterator<char>{});
      std::string sLine = sBuffer.substr(sBuffer.size()-100);

      auto end = std::chrono::high_resolution_clock::now();
      std::cout << sBuffer.size() << std::endl;
      std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << std::endl;
      std::cout << sLine << std::endl;
   }

}
