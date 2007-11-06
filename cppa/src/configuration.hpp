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
        ("config-file,c", po::value<std::string>()->default_value("cppa.rc"), "specify the configuration file")
        ("input-file,i", po::value< std::vector< std::string > >(), "c++ header and/or source file(s) to parse")
        ("db-file,d", po::value<std::string>(), "database file where code map shall be injected")
        ("input-folder,f", po::value< std::vector< std::string > >(), "folder containing files to parse")
        ("recursive,r", "recursively parse files in folder")
        ("delayed-db,e", "delayed the database writing to the end")
        ("write-db-for-each-record,w", "write the database each time a regexp is matched")
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

      if (!vm.count("input-file") && !vm.count("input-folder"))
      // We are not displaying version nor help? Well we need a file to parse!
      {
        BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
                  boost::logging::error, 
                  "error: no input file or folder");
        return false;
      }

      if (!vm.count("input-folder") && vm.count("recursive"))
      // Recursive option set, but no folder!?
      {
        BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
                  boost::logging::error, 
                  "error: the recursive option shall be used when (a) folder(s) is/are specified");
        return false;
      }

      if (!vm.count("db-file") && vm.count("delayed-db"))
      {
        BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
                  boost::logging::warning, 
                  "warning: db write delayed asked but no database file specified");
      }

      /*
       * Fill in the configuration object with the options parsed
       */
      m_config_file = vm["config-file"].as<std::string>();
      m_database_file = (vm.count("db-file")) ? vm["db-file"].as<std::string>() : DEFAULT_DB_FILE;
      if (vm.count("input-file")) 
        m_input_file_vector = vm["input-file"].as< std::vector< std::string > >();
      if (vm.count("input-folder")) 
        m_input_folder_vector = vm["input-folder"].as< std::vector< std::string > >();
      m_recursive = (vm.count("recursive")) ? true : false;
      m_delayed_db = (vm.count("delayed-db")) ? true : false;
      m_write_db_by_record = (vm.count("write-db-for-each-record") && !m_delayed_db) ? true : false;
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
  std::vector<std::string>  m_input_file_vector;
  std::vector<std::string>  m_input_folder_vector;
  bool                      m_recursive;
  bool                      m_delayed_db;
  bool                      m_write_db_by_record;
};



#endif // ! _OPTION_HPP_