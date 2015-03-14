Parse c++ files and create a relational database with all the identifier present in the code.

Example:
```
jd@desktop:~/$ cppa -f ~/c++/cppa/cppa/test/ -r -d test.db
/home/jd/c++/cppa/cppa/test/test1.cpp
/home/jd/c++/cppa/cppa/test/var_decl.cpp
warning: /home/jd/c++/cppa/cppa/test/var_decl.cpp(16): warning: last line of file ends without a newline
/home/jd/c++/cppa/cppa/test/a folder/another one/and another one/test2.cpp
3 file(s) parsed
jd@desktop:~/$ cppacl -l -d ../../cppa/gcc/test.db   
12 classes loaded.
8 variables loaded.
test.db> info ouch5
+ identifier: ouch5
  template specialisation: 
  filename : /home/michaudjd/c++/cppa/cppa/test/test1.cpp
  line: 474
test.db> 
```
