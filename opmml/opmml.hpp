#ifndef __INC_MML_HPP__
#define __INC_MML_HPP__

#include "../opparser/opcalcrule.hpp"

#define _MSTR(x) + MMLString(x) +
// #define _MAPPLY(x) + x.formatBracket(true)
#define _MAPPLY(x) + x.formatBracket(false)
#define _MPP(x) + x.formatBracket(false) +
#define _MPRE(x) x.formatBracket(false) +
#define _MPOST(x) + x.formatBracket(false)

#define _MML(x) "<math>" x "</math>"
#define _MNODE(o,x) "<m" #o ">" x "</m" #o ">"

#define _MI(x) _MNODE(i, #x)
#define _MIX(x) _MNODE(i, "&" #x ";")
#define _MIVAL(x) _MNODE(i, _MSTR(x))
#define _MN(x) _MNODE(n, #x)
#define _MNVAL(x) _MNODE(n, _MSTR(x))
#define _MO(x) _MNODE(o, #x)
#define _MOX(x) _MNODE(o, "&" #x ";")
#define _MOPRE(x) "<mo form=\"prefix\">" #x "</mo>"
#define _MOPOST(x) "<mo form=\"postfix\">" #x "</mo>"

#define _MFENCED(x) _MNODE(fenced, x)
#define _MFRAC(x) _MNODE(frac, x)
#define _MPADDED(x) _MNODE(padded, x)
#define _MSQRT(x) _MNODE(sqrt, x)
#define _MSUB(x) _MNODE(sub, x)
#define _MSUP(x) _MNODE(sup, x)

namespace OPParser {
    // MML data type
    typedef enum {mtOther, mtNum, mtBracket} MMLType;

    // String
    typedef string MMLString;

    // MML data
    struct MMLData: MMLString {
        MMLType type;
        MMLData(MMLString toValue, MMLType toType = mtBracket): MMLString(toValue), type(toType) {}

        MMLData &operator=(const MMLString value) {
            *((MMLString *) this) = value;
            type = mtOther;
            return *this;
        }

        const MMLString formatBracket(bool force) const {
            if (force || type == mtBracket) {
                // <mo>(</mo>data<mo>)</mo>
                return _MFENCED(_MPADDED(_MSTR(*this)));
            } else {
                return *this;
            }
        }
    };

    // Math ML Generator
    class MML: public Parser {
    protected:
        // Push lexers
        void addFirstLexers();

        // Push lexers
        void addLastLexers();
    public:
        // Finish parsing and return result
        MMLString finishByData();
    };
}

#endif
