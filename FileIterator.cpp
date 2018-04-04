#include <vector>
#include <iostream>
#include <set>
#include <tuple>
#include <algorithm>
#include <cstring>
#include <chrono>
#include <fstream>

struct FileIterator
{
   using difference_type = int;
   using value_type = const std::string&;
   using reference_type = void;
   using pointer_type = void;

   FileIterator() = default;

   explicit FileIterator(std::string sFile) : m_sFile{std::move(sFile)}
   {
      std::cout << "default construct" << std::endl;
      Open();
   }

   FileIterator(const FileIterator& rFileIterator)
   {
      std::cout << "copy construct" << std::endl;
      m_sFile = rFileIterator.m_sFile;
      Open();
      while(m_nLine < rFileIterator.m_nLine)
      {
         ++*this;
      }
   }

   FileIterator(FileIterator&& rFileIterator) noexcept
   {
      std::cout << "move construct" << std::endl;
      m_File = std::move(rFileIterator.m_File);
      m_sFile = std::move(rFileIterator.m_sFile);
      m_sLine = std::move(rFileIterator.m_sLine);
      m_nLine = rFileIterator.m_nLine;
   }

   FileIterator& operator=(const FileIterator& rFileIterator)
   {
      std::cout << "copy assignment" << std::endl;
      Close();
      m_sFile = rFileIterator.m_sFile;
      Open();
      while(m_nLine < rFileIterator.m_nLine)
      {
         ++*this;
      }
   }

   FileIterator& operator=(FileIterator&& rFileIterator) noexcept
   {
      std::cout << "move assignment" << std::endl;
      Close();
      m_File = std::move(rFileIterator.m_File);
      m_sFile = std::move(rFileIterator.m_sFile);
      m_sLine = std::move(rFileIterator.m_sLine);
      m_nLine = rFileIterator.m_nLine;
   }

   ~FileIterator()
   {
      Close();
   }

   void Open()
   {
      m_sLine = "";
      m_nLine = -1;

      if (m_sFile.empty())
      {
         std::cout << "File is empty." << std::endl;
         return;
      }

      m_File.open(m_sFile.c_str());
      if (m_File.fail())
      {
         std::cout << "File is fail." << std::endl;
         return;
      }

      if (m_File.eof())
      {
         std::cout << "File is eof." << std::endl;
         return;
      }

      std::getline(m_File, m_sLine);
      m_nLine = 1;
   }

   void Close()
   {
      m_sLine = "";
      m_nLine = -1;
      if (m_File)
      {
         m_File.close();
      }
   }

   const std::string& operator*() const
   {
      return m_sLine;
   }

   bool operator==(const FileIterator& rFileIterator) const
   {
      return rFileIterator.m_nLine == m_nLine;
   }

   bool operator!=(const FileIterator& rFileIterator) const
   {
      return !operator==(rFileIterator);
   }

   bool operator++()
   {
      if (!m_File.eof())
      {
         std::getline(m_File, m_sLine);
         ++m_nLine;
      }
      else
      {
         m_sLine = "";
         m_nLine = -1;
      }
   }

   std::ifstream m_File;
   std::string m_sFile{};
   std::string m_sLine{};
   int m_nLine{-1};
};

struct FileStream
{
   explicit FileStream(std::string sFile)
         : m_sFile{std::move(sFile)}
   {
   }

   using iterator = FileIterator;

   FileIterator begin() const
   {
      return iterator{m_sFile};
   }

   FileIterator end() const
   {
      return iterator{};
   }

   std::string m_sFile;
};


int main()
{
   FileStream fs{"/home/hkwok/tmp/temp.txt"};
   for(const auto& it : fs)
   {
      std::cout << it << std::endl;
   }
   std::cout << "end line" << std::endl;
}
