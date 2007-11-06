#ifndef _COMMAND_HANDLER_HPP_
#define _COMMAND_HANDLER_HPP_

#include <string>
#include <vector>
#include <iostream>

#include <boost/bind.hpp>

#include "database_explorer.hpp"
#include "command_line_interpreter.hpp"

class command_handler
{
public:
  command_handler(database_explorer &database_explorer) : m_database_explorer(database_explorer)
  {
    m_command_desc.add_options()
      ("help", po::value< std::vector<std::string> >()->zero_tokens()->notifier(boost::bind(&command_handler::help, this, _1)), "get help")
      ("info", po::value< std::vector<std::string> >()->notifier(boost::bind(&command_handler::info, this, _1))->multitoken(), "get info on a symbol")
      ("exit", po::value< std::vector<std::string> >()->zero_tokens()->notifier(boost::bind(&command_handler::exit, this, _1)), "exit cppacl")
      ;
  }
  
  void help(const std::vector<std::string> &parameters)
  {
    std::cout << m_command_desc;
  }

  void info(const std::vector<std::string> &parameters)
  {
    for (std::vector<std::string>::const_iterator it = parameters.begin();
         it != parameters.end();
         ++it)
      std::cout << *it << std::endl;
  }

  void exit(const std::vector<std::string> &)
  {
    ::exit(0);
  }

  void launch_command_line(std::istream &input)
  {
    std::string db_filename = configuration::get_instance().m_database_file;
    
    unsigned int pos = db_filename.find_last_of("\\");
    if (pos == std::string::npos)
    {
      pos = db_filename.find_last_of("/");
      if (pos == std::string::npos)
        pos = 0;
      else
        ++pos;
    }
    else
      ++pos;

    db_filename = db_filename.substr(pos);
    
    boost::cli::command_line_interpreter cli(m_command_desc, db_filename + "> ");
    cli.interpret(input);
  }

private:
  database_explorer               &m_database_explorer;
  boost::cli::commands_description m_command_desc;
};

#endif // ! _COMMAND_HANDLER_HPP_