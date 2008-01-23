#ifndef _COMMAND_HANDLER_HPP_
#define _COMMAND_HANDLER_HPP_

#include <string>
#include <vector>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include "logging.hpp"

#include <editline/readline.h>

#include "code_map_factory.hpp"
#include "database_explorer.hpp"
#include "command_line_interpreter.hpp"

// Forward declaration
class command_handler;
extern command_handler *g_command_handler;

char **complete_command(const char *c, int s, int l);

/* Generator function for command completion.  STATE lets us
 *  know whether to start from scratch; without any state
 *  (i.e. STATE == 0), then we start at the top of the list.
 */
char *completion_generator(const char *c, int state);

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

    g_command_handler = this;
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
    {
      std::string class_output;
      int class_found = m_database_explorer.get_info_on_class(*it, class_output);
      std::string vars_output;
      int vars_found  = m_database_explorer.get_info_on_var(*it, vars_output);

      if (!class_found && !vars_found)
      {
        std::cout << class_output << std::endl;
        std::cout << vars_output << std::endl;
      }

      if (class_found)
        std::cout << class_output << std::endl;
      if (vars_found)
        std::cout << vars_output << std::endl;
    }
  }

  void exit(const std::vector<std::string> &)
  {
    ::exit(0);
  }

  static char *readline_with_history(const char *prompt)
  {
    char *line = readline(prompt);
    if (line)
      add_history(line);

    return line;
  }

  char *completion_generator(const char *c, int state)
  {
    LOGLITE_LOG_L4("completion_generator called with c: " << c << " state: " << state);
    
    if (state == 0)
    {
      m_matching_ids_index = 0;
      m_matching_ids.clear();
      m_database_explorer.get_class_id_starts_with(c, m_matching_ids);
      m_database_explorer.get_var_id_starts_with(c, m_matching_ids);
    }

    if (m_matching_ids_index < m_matching_ids.size())
    {
      LOGLITE_LOG_L4("completion_generator returns " << m_matching_ids[m_matching_ids_index]);
      return strdup(m_matching_ids[m_matching_ids_index++].c_str());
    }

    LOGLITE_LOG_L4("completion_generator no more completion possible");
    return NULL;
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

    rl_attempted_completion_function = complete_command; // redirect completion function of editline

    cli.interpret_(input, &command_handler::readline_with_history);
  }

private:
  database_explorer               &m_database_explorer;
  boost::cli::commands_description m_command_desc;

  // for completion
  std::vector<std::string>         m_matching_ids;
  unsigned int                     m_matching_ids_index;
};

#endif // ! _COMMAND_HANDLER_HPP_
