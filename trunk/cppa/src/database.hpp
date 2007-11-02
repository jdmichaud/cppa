#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include <string>
#include <vector>

#include "sqlite3.h"

class database;

class database
{
public:
  static database &get_instance()
  {
#if defined(BOOST_HAS_THREADS)
    static boost::mutex m_inst_mutex;
    boost::mutex::scoped_lock scoped_lock(m_inst_mutex);
#endif // BOOST_HAS_THREADS

    return m_database_singleton;
  }

  void init(const std::string &database_filename);
  int create_database();  
  int execute(const std::string &statement);
  int execute(const std::string &statement, sqlite3_callback callback, void *argument, char **errmsg);
  const char *get_last_error();

private:
  static database m_database_singleton;

private:
  sqlite3             *m_database;
  static boost::mutex	 m_callback_mutex;
};

#endif // ! _DATABASE_HPP_