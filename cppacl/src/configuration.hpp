#ifndef _OPTION_HPP_
#define _OPTION_HPP_

#include <string>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>

#include "defines.h"

namespace po = boost::program_options;

class configuration;

class configuration
{
public:

  static configuration &get_instance()
  {    
#if defined(BOOST_HAS_THREADS)
    static boost::mutex         m_inst_mutex;
    boost::mutex::scoped_lock   scoped_lock(m_inst_mutex);
#endif // BOOST_HAS_THREADS

    return m_configuration;
  }

  configuration()
  {
    m_option_desc.reset(new po::options_description("Allowed options"));
    m_option_desc->add_options()
        ("help,h", "display help message")
        ("version,v", "display version")
        ("config-file,c", po::value<std::string>()->default_value("cppacl.rc"), "specify the configuration file")
        ("db-file,d", po::value<std::string>(), "database file")
        ("load-file,l", "load database file at start (long to start, but fast to use)")
    ;
  }

  bool parse_option(int argc, char **argv)
  {
    BOOST_LOG_L1("parse_option: argc " << argc << " argv " << argv);
    static boost::program_options::variables_map vm;

    try 
    {
      po::store(po::parse_command_line(argc, argv, *m_option_desc), vm);
      po::notify(vm);

      /*
       * Preliminary checks
       */
      if (vm.count("version")) // print version and quit
      {
        std::cout << argv[0] << " version " << VERSION \
                  << " - Copyright 2007 Jean-Daniel Michaud" << std::endl;
        return true;
      }

      if (vm.count("help")) // display help and quit
        return false;

      if (!vm.count("db-file"))
      {
        BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
                  boost::logging::error, 
                  "error: please specify a database file");
        return false;
      }

      /*
       * Fill in the configuration object with the options parsed
       */
      m_config_file = vm["config-file"].as<std::string>();
      m_database_file = (vm.count("db-file")) ? vm["db-file"].as<std::string>() : DEFAULT_DB_FILE;
      m_load_at_startup = (vm.count("load-file")) ? true : false;
      return true;
    }
    catch(std::exception& e) 
    {
      BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
                boost::logging::error, 
                "error: " << e.what());
      return false;
    }
#ifndef _DEBUG
    catch(...) 
    {
      BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
                boost::logging::error, 
                "error: Exception of unknown type!");
      return false;
    }
#endif
    return true;
  }

  void usage()
  {
    std::cout << "usage: cppa" << std::endl;
    std::cout << *m_option_desc << std::endl;
  }

private:
  static configuration m_configuration;
  boost::shared_ptr<po::options_description> m_option_desc;

public: // option members
  std::string               m_config_file;
  std::string               m_database_file;
  bool                      m_load_at_startup;
};



#endif // ! _OPTION_HPP_