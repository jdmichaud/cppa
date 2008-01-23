#ifndef _CODE_MAP_FACTORY_HPP_
#define _CODE_MAP_FACTORY_HPP_

#include <string>
#include <vector>

#include "code_map.hpp"
#include "database.hpp"

class class_factory
{
public:
  int extract_classes(std::vector<class_repr> &classes)
  {
    std::ostringstream query;

    query << "SELECT identifier, template_specialisation, parent_class, filename, line FROM class;";
    LOGLITE_LOG_L1("class_factory::extract_classes: query -->");
    LOGLITE_LOG_L1(query.str());

    database::query_result_t result;
    if (SQLITE_OK != database::get_instance().execute(query.str(), database::generic_callback, &result, NULL))
    {
      LOGLITE_LOG(LOGLITE_MASK_LEVEL_1, 
                  loglite::error, 
                  "class_factory::extract_classes: query failed: " << database::get_instance().get_last_error());
      return -1;      
    }
    
    for (unsigned int i = 0; i < result["identifier"].size(); ++i)
    {
      classes.push_back(class_repr(result["identifier"][i],
                                   result["template_specialisation"][i],
                                   result["parent_class"][i],
                                   result["filename"][i],
                                   ::atoi(result["line"][i].c_str())));
    }

    return 0;
  }

  int get_class(std::string id, std::vector<class_repr> &classes)
  {
    std::ostringstream query;

    query << "SELECT identifier, template_specialisation, parent_class, filename, line FROM class WHERE identifier = \"" << id << "\" ;";
    LOGLITE_LOG_L1("class_factory::get_class: query -->");
    LOGLITE_LOG_L1(query.str());
    
    database::query_result_t result;
    if (SQLITE_OK != database::get_instance().execute(query.str(), database::generic_callback, &result, NULL))
    {
      LOGLITE_LOG(LOGLITE_MASK_LEVEL_1, 
                  loglite::error, 
                  "var_factory::get_class: query failed: " << database::get_instance().get_last_error());
      return -1;      
    }

    for (unsigned int i = 0; i < result["identifier"].size(); ++i)
    {
      classes.push_back(class_repr(result["identifier"][i],
                                   result["template_specialisation"][i],
                                   result["parent_class"][i],
                                   result["filename"][i],
                                   ::atoi(result["line"][i].c_str())));
    }
    
    return 0;
  }
};

class var_factory
{
public:
  int extract_variables(std::vector<var_declr_repr> &vars)
  {
    std::ostringstream query;

    query << "SELECT identifier, type, qualifier, nature, filename, line FROM var;";
    LOGLITE_LOG_L1("var_factory::extract_variables: query -->");
    LOGLITE_LOG_L1(query.str());

    database::query_result_t result;
    if (SQLITE_OK != database::get_instance().execute(query.str(), database::generic_callback, &result, NULL))
    {
      LOGLITE_LOG(LOGLITE_MASK_LEVEL_1, 
                  loglite::error, 
                  "var_factory::extract_variables: query failed: " << database::get_instance().get_last_error());
      return -1;      
    }

    for (unsigned int i = 0; i < result["identifier"].size(); ++i)
    {
      vars.push_back(var_declr_repr(result["identifier"][i],
                                    result["type"][i],
                                    result["nature"][i],
                                    result["qualifier"][i],
                                    result["filename"][i],
                                    ::atoi(result["line"][i].c_str())));
    }

    return 0;
  }

  int get_var(std::string id, std::vector<var_declr_repr> &vars)
  {
    std::ostringstream query;

    query << "SELECT identifier, type, qualifier, nature, filename, line FROM var WHERE identifier = \"" << id << "\" ;";
    LOGLITE_LOG_L1("var_factory::get_var: query -->");
    LOGLITE_LOG_L1(query.str());
    
    database::query_result_t result;
    if (SQLITE_OK != database::get_instance().execute(query.str(), database::generic_callback, &result, NULL))
    {
      LOGLITE_LOG(LOGLITE_MASK_LEVEL_1, 
                  loglite::error, 
                  "var_factory::get_var: query failed: " << database::get_instance().get_last_error());
      return -1;      
    }

    for (unsigned int i = 0; i < result["identifier"].size(); ++i)
    {
      vars.push_back(var_declr_repr(result["identifier"][i],
                                    result["type"][i],
                                    result["nature"][i],
                                    result["qualifier"][i],
                                    result["filename"][i],
                                    ::atoi(result["line"][i].c_str())));
    }
    
    return 0;
  }
};

#endif // ! _CODE_MAP_FACTORY_HPP_
