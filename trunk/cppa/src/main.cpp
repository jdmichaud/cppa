#include <string>
#include <fstream>
#include <logging.hpp>

#include "configuration.hpp"
#include "file_iterator.hpp"
#include "code_map.hpp"
#include "parser.hpp"


configuration configuration::m_configuration;

void init_log(const std::string &filename)
{
  boost::logging::logger *l = boost::logging::logger::get_instance();

  boost::logging::format display_format(boost::logging::trace >> boost::logging::eol);
  l->add_format(display_format);

#ifdef _DEBUG
  boost::logging::format file_format("[" >> boost::logging::mask >> "],"
                                         >> boost::logging::filename >> "("
                                         >> boost::logging::line >> "),"
                                         >> boost::logging::time >> ", "
                                         >> boost::logging::trace
                                         >> boost::logging::eol); // log format
  l->add_format(file_format);

  boost::logging::sink file_sink(new std::ofstream(filename.c_str(), 
                                                   std::ios_base::app), 
                                 BOOST_LOG_MASK_LEVEL_3);
  file_sink.attach_qualifier(boost::logging::log);
  file_sink.attach_qualifier(boost::logging::warning);
  file_sink.attach_qualifier(boost::logging::error);
  l->add_sink(file_sink, file_format);
#endif

  boost::logging::sink display_sink(&std::cout, BOOST_LOG_MASK_LEVEL_1);
  display_sink.attach_qualifier(boost::logging::warning);
  display_sink.attach_qualifier(boost::logging::notice);
  l->add_sink(display_sink, display_format);

  boost::logging::sink display_error_sink(&std::cerr, BOOST_LOG_MASK_LEVEL_1);
  display_error_sink.attach_qualifier(boost::logging::error);
  l->add_sink(display_error_sink, display_format);
}

int main(int argc, char **argv)
{
  init_log("output.log");
  BOOST_LOG_L1("cppa up");
  
  if (!configuration::get_instance().parse_option(argc, argv))
  {
    configuration::get_instance().usage();
    return 1;
  }

  database::get_instance().init(configuration::get_instance().m_database_file);

  file_iterator fi(configuration::get_instance().m_input_file_vector,
                   configuration::get_instance().m_input_folder_vector,
                   configuration::get_instance().m_recursive);

  code_map      cm;
  parser        p;
  unsigned long count = 0;
  while (*fi != file_iterator::end())
  {
    std::cout << *fi << std::endl;
    p.parse(*fi, cm);
    ++fi; ++count;
  }

  BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
            boost::logging::notice, count << " file(s) parsed");

  if (configuration::get_instance().m_delayed_db)
  {
    BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
              boost::logging::notice, "writing database...");
    cm.insert_in_database();
  }

  return 0;
}