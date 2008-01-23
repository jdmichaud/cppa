#include <fstream>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "logging.hpp"
#include "sqlite3.h"
#include "database.hpp"

database      database::m_database_singleton;
boost::mutex	database::m_callback_mutex;

void database::init(const std::string &database_filename, bool write)
{
  std::ifstream datafile(database_filename.c_str());
  if (!write && !datafile.is_open())
    throw "Error reading database";

  LOGLITE_LOG_L1("database::init: Checking database (" << database_filename << ") ...");
  if (SQLITE_OK != sqlite3_open(database_filename.c_str(), &m_database))
  {
    LOGLITE_LOG(LOGLITE_MASK_LEVEL_1, 
      loglite::error, "error:" << sqlite3_errmsg(m_database));
    return ;
  }
  if (!datafile.is_open())
  {
    if (!write)
      throw "Error reading database";

    LOGLITE_LOG_L1("database::init: Database " << database_filename << " does not exist, creating it ...");
    if (create_database())
    {
      LOGLITE_LOG(LOGLITE_MASK_LEVEL_1, 
        loglite::error, "error creating database");
      throw "Error creating database";
    }
  }
  else
    LOGLITE_LOG_L1("database::init: Database " << database_filename << " exists");
}

int database::create_database()
{
  sqlite3_exec(m_database, "CREATE DATABASE cppadatabase;", NULL, NULL, NULL);
  sqlite3_exec(m_database, "CREATE TABLE class (guid INTEGER PRIMARY KEY,           \
                                                identifier                varchar,  \
                                                template_specialisation   varchar,  \
                                                parent_class              varchar,  \
                                                derived_class             varchar,  \
                                                filename                  varchar,  \
                                                line                      varchar);", NULL, NULL, NULL);
  sqlite3_exec(m_database, "CREATE TABLE var (guid INTEGER PRIMARY KEY,           \
                                              identifier                varchar,  \
                                              type                      varchar,  \
                                              qualifier                 varchar,  \
                                              nature                    varchar,  \
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
