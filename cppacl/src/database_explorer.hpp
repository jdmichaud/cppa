#ifndef _DATABASE_EXPLORER_HPP_
#define _DATABASE_EXPLORER_HPP_

#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <logging.hpp>

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
      LOGLITE_LOG(LOGLITE_MASK_LEVEL_1, 
                  loglite::notice, m_classes.size() << " classes loaded.");

      var_factory vf;
      vf.extract_variables(m_vars);
      LOGLITE_LOG(LOGLITE_MASK_LEVEL_1, 
                  loglite::notice, m_vars.size() << " variables loaded.");

      m_loaded = true;
    }
  }

  int get_class(std::string id, std::vector<class_repr>& classes)
  {
    if (m_loaded)
    {
      for (unsigned int i = 0; i < m_classes.size(); ++i)
        if (m_classes[i].m_class_name == id)
          classes.push_back(m_classes[i]);
    }
    else
    {
      class_factory cf;
      return cf.get_class(id, classes);
    }
    return 0;
  }

  int get_var(std::string id, std::vector<var_declr_repr>& vars)
  {
    if (m_loaded)
    {
      for (unsigned int i = 0; i < m_vars.size(); ++i)
        if (m_vars[i].m_identifier == id)
          vars.push_back(m_vars[i]);
    }
    else
    {
      var_factory vf;
      return vf.get_var(id, vars);
    }
    return 0;
  }

  int get_info_on_class(std::string id, std::string &res)
  {
    std::vector<class_repr> classes;
    get_class(id, classes);

    res = "";
    if (classes.empty())
    {
      res = id + ": no such class";
    }
    else if (classes.size() < 3) // If less than 3 items found, display them nicely
      for (unsigned int i = 0; i < classes.size(); ++i)
      {
        res += "+ identifier: " + classes[i].m_class_name + "\n";
        res += "  template specialisation: " + classes[i].m_template_specialisation + "\n";
        res += "  filename : " + classes[i].m_filename + "\n";
        res += "  line: " + boost::lexical_cast<std::string>(classes[i].m_line) + ((i + 1 == classes.size()) ? "" : "\n");
      }
    else // otherwise, everybody in line!
    {
      res += "identifier: " + classes[0].m_class_name + "\n";
      for (unsigned int i = 0; i < classes.size(); ++i)
      {
        res += "[" + boost::lexical_cast<std::string>(i) + "]";
        res += "  template specialisation: " + classes[i].m_template_specialisation;
        res += " filename : " + classes[i].m_filename;
        res += " line: " + boost::lexical_cast<std::string>(classes[i].m_line) + ((i + 1 == classes.size()) ? "" : "\n");
      }
    }

    return classes.size();
  }

  int get_info_on_var(std::string id, std::string& res)
  {
    std::vector<var_declr_repr> vars;
    get_var(id, vars);

    res = "";
    if (vars.empty())
    {
      res = id + ": no such variable";
    }
    else if (vars.size() < 3) // If less than 3 items found, display them nicely
      for (unsigned int i = 0; i < vars.size(); ++i)
      {
        res += "+ identifier: " + vars[i].m_identifier + "\n";
        res += "  type: " + vars[i].m_type + "\n";
        res += "  qualifier: " + vars[i].m_qualifier + "\n";
        res += "  nature: " + vars[i].m_nature + "\n";
        res += "  filename : " + vars[i].m_filename + "\n";
        res += "  line: " + boost::lexical_cast<std::string>(vars[i].m_line) + ((i + 1 == vars.size()) ? "" : "\n");
      }
    else // otherwise, everybody in line!
    {
      res += "identifier: " + vars[0].m_identifier + "\n";
      for (unsigned int i = 0; i < vars.size(); ++i)
      {
        res += "[" + boost::lexical_cast<std::string>(i) + "]";
        res += " type: " + vars[i].m_type;
        res += " qualifier: " + vars[i].m_qualifier;
        res += " nature: " + vars[i].m_nature;
        res += " filename : " + vars[i].m_filename;
        res += " line: " + boost::lexical_cast<std::string>(vars[i].m_line) + ((i + 1 == vars.size()) ? "" : "\n");
      }
    }

    return vars.size();
  }

private:
  std::vector<class_repr>     m_classes;
  std::vector<var_declr_repr> m_vars;
  bool                        m_loaded;
};

#endif // !_DATABASE_EXPLORER_HPP_
