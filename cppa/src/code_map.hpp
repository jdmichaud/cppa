#ifndef _CODE_MAP_HPP_
#define _CODE_MAP_HPP_

class class_repr
{
public:
  class_repr(const std::string &class_name, 
             const std::string &template_specialisation) :
              m_class_name(m_class_name), 
              m_template_specialisation(template_specialisation)
  {
  }

  class_repr &operator=(const class_repr &rhs)
  {
    m_class_name = rhs.m_class_name;
    m_template_specialisation = rhs.m_template_specialisation;
    return *this;
  }

  std::string m_class_name;
  std::string m_template_specialisation;
};

class code_map
{
public:
  void add_class(const std::string identifier,
                 const std::string template_specialisation)
  {
    m_class_vector.push_back(class_repr(identifier, template_specialisation));
  }

private:
  std::vector<class_repr>  m_class_vector;
};

#endif // ! _CODE_MAP_HPP_