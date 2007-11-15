#ifndef _FILE_ITERATOR_HPP_
#define _FILE_ITERATOR_HPP_

#include <string>
#include <vector>
#include <exception>
#include <logging.hpp>

#include "boost/regex.hpp"
#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations

namespace fs = boost::filesystem;

class file_iterator
{
public:
  file_iterator(const std::vector<std::string> &files,
                const std::vector<std::string> &folders,
                bool recursive) : m_files(files),
                                  m_folders(folders),
                                  m_recursive(recursive),
                                  m_file_index(0),
                                  m_folder_index(0)
  {
    if (m_files.empty())
    {
      if (m_folders.empty())
        throw "no file or folder to iterate";

    }
    else
      m_current_file = m_files[m_file_index++];
      
    tokenize(CPP_EXTENSIONS, ";", m_cpp_extensions);
  }

  std::string operator*()
  {
    if (m_current_file == end())
      operator++();

    return get_current_file();
  }

  std::string get_current_file()
  {
    return m_current_file;
  }

  void tokenize(const std::string &str, const std::string &separator, std::vector<std::string> &v)
  {
     typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
     boost::char_separator<char>  sep(separator.c_str());
     tokenizer                    tokens(str, sep);
     for (tokenizer::iterator     tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
       v.push_back(*tok_iter);
  }

  bool is_cpp_file(const std::string &filename)
  {
    for (unsigned int i = 0; i < m_cpp_extensions.size(); ++i)
    {
      boost::regex  re(std::string(".*\\.") + m_cpp_extensions[i] + "$");
      boost::smatch what;
      if (boost::regex_search(filename, what, re))
        return true;
    }
    return false;
  }

  void operator++()
  {
    if (!m_files.empty() && m_file_index < m_files.size())
    {
      m_current_file = m_files[m_file_index++];
      return ;
    }
    else if (!m_folders.empty() && m_folder_index < m_folders.size())
    {
      bool found = false;
      for ( ; !found && m_folder_index < m_folders.size(); ++m_folder_index)
      {
        if (!fs::exists(m_folders[m_folder_index])) 
        {
          BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, boost::logging::warning, "warning: " << m_folders[m_folder_index] << " unknown folder");
          continue ;
        }
        fs::directory_iterator end_itr; // default construction yields past-the-end
        for (fs::directory_iterator itr(m_folders[m_folder_index]);
             itr != end_itr;
             ++itr )
        {
          if (fs::is_directory(itr->status()) && m_recursive)
          {
            m_folders.push_back(itr->path().string());
          }
          else
          {
            if (is_cpp_file(itr->path().leaf()))
            {
              m_files.push_back(itr->path().string());
              found = true;
            }
          }
        }
      }
    }
  
    if (!m_files.empty() && m_file_index < m_files.size())
      m_current_file = m_files[m_file_index++];
    else
      m_current_file = "";
  }

  static std::string end()
  {
    return "";
  }

private:
  std::vector<std::string>  m_files;
  std::vector<std::string>  m_folders;
  bool                      m_recursive;

  unsigned int              m_file_index;
  unsigned int              m_folder_index;
  std::string               m_current_file;

  std::vector<std::string>  m_cpp_extensions;
};

#endif // ! _FILE_ITERATOR_HPP_
