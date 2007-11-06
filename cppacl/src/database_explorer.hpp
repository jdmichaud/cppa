#ifndef _DATABASE_EXPLORER_HPP_
#define _DATABASE_EXPLORER_HPP_

#include <string>
#include <vector>

class database_explorer
{
public:
  database_explorer() 
  {
    m_loaded = false;
    if (configuration::get_instance().m_load_at_startup)
    {
      class_factory cf;
      cf.extract_classes(m_classes);
      var_factory vf;
      vf.extract_variables(m_vars);
      m_loaded = true;
    }
  }

  

private:
  std::vector<class_repr>     m_classes;
  std::vector<var_declr_repr> m_vars;
  bool                        m_loaded;
};

#endif // !_DATABASE_EXPLORER_HPP_