#include <boost/bind.hpp>
#include <logging.hpp>

#include "command_handler.hpp"

command_handler *g_command_handler = NULL;

char **complete_command(const char *c, int s, int l)
{
  LOGLITE_LOG_L4("complete_command: c = " << c << ", s = " << s << ", l = " << l);
  return rl_completion_matches(c, completion_generator);
}

char *completion_generator(const char *c, int state)
{
  return g_command_handler->completion_generator(c, state);
}
