#ifndef _CODE_MAP_HPP_
#define _CODE_MAP_HPP_

#include <string>
#include <vector>
#include <iostream>

#include "configuration.hpp"
#include "database.hpp"

class representation
{
public:
  virtual std::ostream &print(std::ostream &output) = 0;
};

class class_repr : public representation
{
public:
  class_repr(const std::string &class_name, 
             const std::string &template_specialisation,
             const std::string &filename,
             unsigned long line) :
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

  std::ostream &print(std::ostream &output)
  {
    output << "class" << std::endl;
    output << " name: " << m_class_name << std::endl;
    output << " template specialisation: " << m_template_specialisation << std::endl;
    output << " filename: " << m_filename << std::endl;
    output << " line: " << m_line << std::endl;
    return output;
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
                 unsigned long line) :
                  m_identifier(identifier),
                  m_type(type), m_nature(nature),
                  m_qualifier(qualifier),
                  m_filename(filename),
                  m_line(line)
  {
  }

  std::ostream &print(std::ostream &output)
  {
    output << "variable" << std::endl;
    output << " id: " << m_identifier << std::endl;
    output << " type: " << m_type << std::endl;
    output << " qualifier: " << m_qualifier << std::endl;
    output << " nature: " << m_nature << std::endl;
    output << " filename: " << m_filename << std::endl;
    output << " line: " << m_line << std::endl;
    return output;
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
  std::vector<class_repr>       m_class_vector;
  std::vector<var_declr_repr>   m_variable_vector;
};

#endif // ! _CODE_MAP_HPP_