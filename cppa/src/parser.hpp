#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <iostream> 
#include <sstream>
#include <boost/regex.hpp> 
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class

#include "code_map.hpp"

class parser
{
private:
  typedef boost::wave::cpplexer::lex_token<>                              token_type;
  typedef boost::wave::cpplexer::lex_iterator<token_type>                 lex_iterator_type;
  typedef boost::wave::context<std::string::iterator, lex_iterator_type>  context_type;


public:

  void index_classes(const std::string &file_content, 
                     const std::string &filename,
                     code_map &cm) 
  { 
    boost::regex class_regex(CLASS_REGEX);
    std::string::const_iterator start, end; 
    start = file_content.begin(); 
    end = file_content.end(); 
    boost::match_results<std::string::const_iterator> what; 
    boost::match_flag_type flags = boost::match_default; 
    while(regex_search(start, end, what, class_regex, flags)) 
    { 
      // what[0] contains the whole string 
      // what[5] contains the class name. 
      // what[6] contains the template specialisation if any. 
      // add class name and position to map: 
      cm.add_class(std::string(what[5].first, what[5].second),
                   std::string(what[3].first, what[3].second),
                   filename, what[5].first - file_content.begin());

      // update search position: 
      start = what[0].second; 

      // update flags: 
      flags |= boost::match_prev_avail; 
      flags |= boost::match_not_bob; 
    } 
  }


  int parse(const std::string &filename, code_map &cm)
  {
    BOOST_LOG_L1("parsing " << filename);
    boost::wave::util::file_position_type current_position;
    std::ifstream input_file(filename.c_str());
    std::string input(
        std::istreambuf_iterator<char>(input_file.rdbuf()),
        std::istreambuf_iterator<char>());
    std::stringstream input_stream;

    context_type ctx(input.begin(), input.end(), filename.c_str());
    context_type::iterator_type first = ctx.begin();
    context_type::iterator_type last = ctx.end();

    try
    {
      while (first != last) 
      {
        current_position = (*first).get_position();
        input_stream << (*first).get_value();
        ++first;
      }
    }
    catch (boost::wave::cpp_exception const& e) 
    {
      // some preprocessing error
      BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
                boost::logging::warning,
                "warning: " << e.file_name() << "(" << e.line_no() << "): " << e.description() << std::endl);
    }
    catch (std::exception const& e) {
      // use last recognized token to retrieve the error position
      BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
                boost::logging::error,
                "error:" << current_position.get_file() << "(" << current_position.get_line() << "): " \
                << "exception caught: " << e.what() << std::endl);
        return 3;
    }
    catch (...) {
      // use last recognized token to retrieve the error position
      BOOST_LOG(BOOST_LOG_MASK_LEVEL_1, 
                boost::logging::error,
                "error:" << current_position.get_file() << "(" << current_position.get_line() << "): " \
                << "unexpected exception caught." << std::endl);
        return 4;
    }

    index_classes(input_stream.str(), filename, cm);
    return 0;
  }
};

#endif // !_PARSER_HPP_