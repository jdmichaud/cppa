#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include <string>
#include <vector>

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
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

  void init(const std::string &database_filename, bool write = true);
  int create_database();  
  int execute(const std::string &statement);
  int execute(const std::string &statement, sqlite3_callback callback, void *argument, char **errmsg);
  const char *get_last_error();

  typedef std::map< std::string, std::vector<std::string> > query_result_t;
  static int generic_callback(void *object, int argc, char **argv, char **azColName)
  {
    assert(object);
    query_result_t *results = (query_result_t *) object;
    boost::mutex::scoped_lock scoped_lock(database::m_callback_mutex);
    
    for (int i = 0; i < argc; ++i)
      (*results)[azColName[i]].push_back(argv[i]);

    return 0;
  }

private:
  static database m_database_singleton;

private:
  sqlite3             *m_database;
  static boost::mutex	 m_callback_mutex;
};

#endif // ! _DATABASE_HPP_
