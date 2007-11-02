#ifndef _CODE_MAP_HPP_
#define _CODE_MAP_HPP_

#include <string>
#include <vector>

#include "configuration.hpp"
#include "database.hpp"

class class_repr
{
public:
  class_repr(const std::string &class_name, 
             const std::string &template_specialisation,
             const std::string &filename,
             unsigned long &line) :
              m_class_name(class_name), 
              m_template_specialisation(template_specialisation),
              m_filename(filename),
              m_line(line)
  {
  }

  class_repr &operator=(const class_repr &rhs)
  {
    m_class_name = rhs.m_class_name;
    m_template_specialisation = rhs.m_template_specialisation;
    m_filename = rhs.m_filename;
    m_line = rhs.m_line;
    return *this;
  }

  int insert_in_database()
  {
    std::ostringstream query;

    query << "INSERT INTO class (identifier, template_specialisation, filename, line) VALUES (";
    query << "'" << m_class_name << "', ";
    query << "'" << m_template_specialisation << "', ";
    query << "'" << m_filename << "', ";
    query << "'" << m_line << "');";

    BOOST_LOG_L1("class_repr::insert_in_database: query -->");
    BOOST_LOG_L4(query.str());

    if (SQLITE_OK != database::get_instance().execute(query.str()))
    {
      BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
        boost::logging::error, 
        "class_repr::insert_in_database: Insertion failed: " << database::get_instance().get_last_error());
      return -1;
    }
    else
      BOOST_LOG_L1("class_repr::insert_in_database: Insertion done");

    return 0;
  }

  std::string m_class_name;
  std::string m_template_specialisation;
  std::string m_filename;
  unsigned long m_line;

};

class code_map
{
public:
  void add_class(const std::string &identifier,
                 const std::string &template_specialisation,
                 const std::string &filename,
                 unsigned long     line)
  {
    m_class_vector.push_back(class_repr(identifier, 
                                        template_specialisation,
                                        filename,
                                        line));

    if (!configuration::get_instance().m_delayed_db)
      m_class_vector.back().insert_in_database();
  }

private:
  std::vector<class_repr>  m_class_vector;
};

#endif // ! _CODE_MAP_HPP_