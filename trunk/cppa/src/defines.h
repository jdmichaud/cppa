#ifndef _DEFINES_H_
#define _DEFINES_H_

#define VERSION         "0.0.3"

#define DEFAULT_DB_FILE "output.db"

#define CPP_EXTENSIONS  "h;hh;hpp;hxx;c;cc;cxx;cpp"
#define CLASS_REGEX     "^(template[[:space:]]*<[^;:{]+>[[:space:]]*)?(class|struct)[[:space:]]*(\\<\\w+\\>([[:blank:]]*\\([^)]*\\))?[[:space:]]*)*(\\<\\w*\\>)[[:space:]]*(<[^;:{]+>[[:space:]]*)?(\\{|:[^;\\{]*\\{)"
//#define PARENT_CLASS_REGEX  "[[:space:]]*(public|private)?[[:space:]]*([a-zA-Z_][a-zA-Z_0-9]*)[[:space:]]*(?:\\([[:space:]]*([a-zA-Z_][a-zA-Z_0-9]*)[[:space:]]*\\))?"
#define PARENT_CLASS_REGEX  "[[:space:]]*(public|private)?[[:space:]]*([a-zA-Z_][a-zA-Z_0-9]*)[[:space:]]*(?:\\<([a-zA-Z_][a-zA-Z_0-9,[[:space:]]]*)\\>)?[[:space:]]*(?:\\([[:space:]]*([a-zA-Z_][a-zA-Z_0-9,[[:space:]]]*)[[:space:]]*\\))?"
#define ID_REGEX        "[a-zA-Z_][a-zA-Z_0-9]*"
#define VAR_DECL_REGEX  "^((?:const |static |virtual )*[[:space:]]*)([a-zA-Z_][a-zA-Z_0-9]*)(?:[[:space:]]*([\\*&]+)[[:space:]]*|[[:space:]]+)([a-zA-Z_][a-zA-Z_0-9]*)(\[[0-9]*\\])?[[:space:]]*;"

#endif // ! _DEFINES_H_
