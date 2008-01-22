#include <string>
#include <fstream>
#include <logging.hpp>

#include "configuration.hpp"
#include "file_iterator.hpp"
#include "code_map.hpp"
#include "code_map_factory.hpp"
#include "database_explorer.hpp"
#include "command_handler.hpp"
#include "database.hpp"

namespace po = boost::program_options;

configuration configuration::m_configuration;

void init_log(const std::string &filename)
{
  loglite::logger_p l = loglite::logger::get_instance();

  loglite::format display_format(loglite::trace >> loglite::eol);
  l->add_format(display_format);

#ifdef _DEBUG
  loglite::format file_format("[" >> loglite::mask >> "],"
                                  >> loglite::filename >> "("
                                  >> loglite::line >> "),"
                                  >> loglite::time >> ", "
                                  >> loglite::trace
                                  >> loglite::eol); // log format
  l->add_format(file_format);

  loglite::sink file_sink(new std::ofstream(filename.c_str(), 
                                            std::ios_base::app), 
                                 LOGLITE_MASK_LEVEL_3);
  file_sink.attach_qualifier(loglite::log);
  file_sink.attach_qualifier(loglite::warning);
  file_sink.attach_qualifier(loglite::error);
  l->add_sink(file_sink, file_format);
#endif

  loglite::sink display_sink(&std::cout, LOGLITE_MASK_LEVEL_1);
  display_sink.attach_qualifier(loglite::warning);
  display_sink.attach_qualifier(loglite::notice);
  l->add_sink(display_sink, display_format);

  loglite::sink display_error_sink(&std::cerr, LOGLITE_MASK_LEVEL_1);
  display_error_sink.attach_qualifier(loglite::error);
  l->add_sink(display_error_sink, display_format);
}


int main(int argc, char **argv)
{
  init_log("cppacl.log");
  LOGLITE_LOG_L1("cppacl up");
  
  if (!configuration::get_instance().parse_option(argc, argv))
  {
    configuration::get_instance().usage();
    return 1;
  }

  database::get_instance().init(configuration::get_instance().m_database_file, false);
  
  database_explorer de;
  command_handler ch(de);
  ch.launch_command_line(std::cin);

  return 0;
}
