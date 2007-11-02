#include <fstream>

#include "logging.hpp"
#include "sqlite3.h"
#include "database.hpp"

database database::m_database_singleton;

void database::init(const std::string &database_filename)
{
  std::ifstream datafile(database_filename.c_str());
  BOOST_LOG_L1("database::init: Checking database (" << database_filename << ") ...");
  if (SQLITE_OK != sqlite3_open(database_filename.c_str(), &m_database))
  {
    BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
      boost::logging::error, "error:" << sqlite3_errmsg(m_database));
    return ;
  }
  if (!datafile.is_open())
  {
    BOOST_LOG_L1("database::init: Database " << database_filename << " does not exist, creating it ...");
    if (create_database())
    {
      BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
        boost::logging::error, "error creating database");
      std::exception e("Error creating database");
      throw e;
    }
  }
  else
    BOOST_LOG_L1("database::init: Database " << database_filename << " exists");
}

int database::create_database()
{
  sqlite3_exec(m_database, "CREATE DATABASE cppadatabase;", NULL, NULL, NULL);
  sqlite3_exec(m_database, "CREATE TABLE class (identifier                varchar,  \
                                                template_specialisation   varchar,  \
                                                filename                  varchar,  \
                                                line                      varchar);", NULL, NULL, NULL);
  return 0;
}

int database::execute(const std::string &statement)
{
  assert(m_database);
  return sqlite3_exec(m_database, statement.c_str(), NULL, NULL, NULL);
}

int database::execute(const std::string &statement, sqlite3_callback callback, void *argument, char **errmsg)
{
  assert(m_database);
  return sqlite3_exec(m_database, statement.c_str(), callback, argument, errmsg);
}

const char *database::get_last_error()
{
  assert(m_database);
  return sqlite3_errmsg(m_database);
}
