#ifndef _CODE_MAP_HPP_
#define _CODE_MAP_HPP_

#include <string>
#include <vector>

#include "configuration.hpp"
#include "database.hpp"

class representation
{
public:
  virtual int insert_in_database() = 0;
};

class class_repr : public representation
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

class var_declr_repr : public representation
{
public:
  var_declr_repr(const std::string &identifier,
                 const std::string &type,
                 const std::string &nature,
                 const std::string &qualifier,
                 const std::string &filename,
                 unsigned long &line) :
                  m_identifier(identifier),
                  m_type(type), m_nature(nature),
                  m_qualifier(qualifier),
                  m_filename(filename),
                  m_line(line)
  {
  }

  int insert_in_database()
  {
    std::ostringstream query;

    query << "INSERT INTO var (identifier, type, qualifier, nature, filename, line) VALUES (";
    query << "'" << m_identifier << "', ";
    query << "'" << m_type << "', ";
    query << "'" << m_qualifier << "', ";
    query << "'" << m_nature << "', ";
    query << "'" << m_filename << "', ";
    query << "'" << m_line << "');";

    BOOST_LOG_L1("var_declr_repr::insert_in_database: query -->");
    BOOST_LOG_L4(query.str());

    if (SQLITE_OK != database::get_instance().execute(query.str()))
    {
      BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
        boost::logging::error, 
        "var_declr_repr::insert_in_database: Insertion failed: " << database::get_instance().get_last_error());
      return -1;
    }
    else
      BOOST_LOG_L1("var_declr_repr::insert_in_database: Insertion done");

    return 0;
  }

  std::string m_identifier;
  std::string m_type;
  std::string m_nature;
  std::string m_qualifier;
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

    if (configuration::get_instance().m_write_db_by_record)
      m_class_vector.back().insert_in_database();
  }

  void add_var(const std::string &identifier,
               const std::string &type,
               const std::string &qualifier,
               const std::string &nature,
               const std::string &filename,
               unsigned long     line)
  {
    m_variable_vector.push_back(var_declr_repr(identifier, type,
                                               qualifier, nature,
                                               filename, line));

    if (configuration::get_instance().m_write_db_by_record)
      m_variable_vector.back().insert_in_database();
  }

  void insert_in_database()
  {
    BOOST_LOG_L2("writing database...");
    
    database::get_instance().execute("BEGIN TRANSACTION;");
    for (unsigned int i = 0; i < m_class_vector.size(); ++i)
      m_class_vector[i].insert_in_database();
    for (unsigned int i = 0; i < m_variable_vector.size(); ++i)
      m_variable_vector[i].insert_in_database();
    database::get_instance().execute("COMMIT TRANSACTION;");
    m_class_vector.clear();
    m_variable_vector.clear();
  }

private:
  std::vector<class_repr>       m_class_vector;
  std::vector<var_declr_repr>   m_variable_vector;
};

#endif // ! _CODE_MAP_HPP_