#include <some_indexistent_file.hpp>
#include "some_indexistent_file.hpp"

class toto
{
// un ptit commentaire qui fait mal
};

class
titi {};

template <  type_a_la_con> class tutu {};

//forward declaration
class forward_declaration;

class ouch : private aie {};

class ouch1 : private aie(MACRO) {};

class ouch2 : private aie(MACRO), public oula {};

class ouch3 : private aie(MACRO),
              public oula
{
};

class ouch4 : private aie(MACRO), // commentaire a la con
              public oula
{
};

class XRDataMgrImpl : public CAL_BASE_CLASS(CfiXRDataMgr), //## Inherits: <unnamed>%36F3B30F0222
                      	public DLGenericActor,  //## Inherits: <unnamed>%3766056D004E
                        public XRImProcPropertyHandler  //## Inherits: <unnamed>%3E953CCB0075
{
};
