#include "opmml.hpp"

namespace OPParser {
    map <Input, MMLString> MMLConst = {
        {"pi", _MIX(pi)}, {"tau", _MIX(tau)}, {"phi", _MIX(phiv)}, {"inf", _MIX(infin)}, {"nan", _MI(?)}, {"ans", _MI(#)}
    };

    class MMLToken;
    class MMLFuncToken;
    class MMLAssignToken;
    class MMLBiToken;
    class MMLMonoToken;
    class MMLLeftToken;
    class MMLRightToken;
    typedef shared_ptr <MMLToken       > PMMLToken;
    typedef shared_ptr <MMLFuncToken   > PMMLFuncToken;
    typedef shared_ptr <MMLAssignToken > PMMLAssignToken;
    typedef shared_ptr <MMLBiToken     > PMMLBiToken;
    typedef shared_ptr <MMLMonoToken   > PMMLMonoToken;
    typedef shared_ptr <MMLLeftToken   > PMMLLeftToken;
    typedef shared_ptr <MMLRightToken  > PMMLRightToken;

    // Tokens

    // MML base token and number
    class MMLToken: public Token {
    protected:
        MMLData value;
    public:
        friend class MML;
        friend class MMLFuncToken;
        friend class MMLAssignToken;
        friend class MMLBiToken;
        friend class MMLMonoToken;
        friend class MMLRightToken;

        MMLToken(MMLString toData, MMLType toType = mtNum): value(toData, toType) {}

        Level levelLeft() const {
            return levelConst;
        }

        Level levelRight() const {
            return levelConst;
        }

        void onPush(Parser &parser) {
            parser.state = stateOper;
        }

        void onPop(Parser &parser) {
            parser.outStack.push_back(shared_from_this());
        }
    };

    // Functions
    class MMLFuncToken: public Token {
    protected:
        FuncType type;
    public:
        MMLFuncToken(FuncType toType): type(toType) {}

        Level levelLeft() const {
            return levelConst;
        }

        Level levelRight() const {
            return levelFuncR;
        }

        void onPush(Parser &parser) {
            parser.state = stateNum;
        }

        void onPop(Parser &parser) {
            check(!parser.outStack.empty(), "No operand");

            // Cast the token
            // Tokens in outStack should be numbers
            PMMLToken tTarget = dynamic_pointer_cast <MMLToken> (
                parser.outStack.back()
            );
            check(tTarget != nullptr, "Unknown operand");

            // Render calculation
            switch (type) {
            case ftSin:
                tTarget->value = _MI(sin) _MAPPLY(tTarget->value);
                break;
            case ftCos:
                tTarget->value = _MI(cos) _MAPPLY(tTarget->value);
                break;
            case ftTan:
                tTarget->value = _MI(tan) _MAPPLY(tTarget->value);
                break;
            case ftASin:
                tTarget->value = _MSUP(_MI(sin) _MN(-1)) _MAPPLY(tTarget->value);
                break;
            case ftACos:
                tTarget->value = _MSUP(_MI(cos) _MN(-1)) _MAPPLY(tTarget->value);
                break;
            case ftATan:
                tTarget->value = _MSUP(_MI(tan) _MN(-1)) _MAPPLY(tTarget->value);
                break;
            case ftSinH:
                tTarget->value = _MI(sinh) _MAPPLY(tTarget->value);
                break;
            case ftCosH:
                tTarget->value = _MI(cosh) _MAPPLY(tTarget->value);
                break;
            case ftTanH:
                tTarget->value = _MI(tanh) _MAPPLY(tTarget->value);
                break;
            case ftASinH:
                tTarget->value = _MSUP(_MI(sinh) _MN(-1)) _MAPPLY(tTarget->value);
                break;
            case ftACosH:
                tTarget->value = _MSUP(_MI(cosh) _MN(-1)) _MAPPLY(tTarget->value);
                break;
            case ftATanH:
                tTarget->value = _MSUP(_MI(tanh) _MN(-1)) _MAPPLY(tTarget->value);
                break;
            case ftLog:
                tTarget->value = _MI(ln) _MAPPLY(tTarget->value);
                break;
            case ftLog10:
                tTarget->value = _MSUB(_MI(log) _MN(10)) _MAPPLY(tTarget->value);
                break;
            case ftLog2:
                tTarget->value = _MSUB(_MI(log) _MN(2)) _MAPPLY(tTarget->value);
                break;
            case ftSqr:
                tTarget->value = _MSUP(_MPADDED(_MPP(tTarget->value)) _MN(2));
                break;
            case ftSqrt:
                tTarget->value = _MSQRT(_MSTR(tTarget->value));
                break;
            case ftAbs:
                tTarget->value = _MOPRE(|) _MSTR(tTarget->value) _MOPOST(|);
                break;
            case ftSign:
                tTarget->value = _MI(sign) _MAPPLY(tTarget->value);
                break;
            case ftDeg:
                tTarget->value = _MSUP(_MPADDED(_MPP(tTarget->value)) _MPADDED(_MO(r) _MOX(rarr) _MO(d)));
                break;
            case ftRad:
                tTarget->value = _MSUP(_MPADDED(_MPP(tTarget->value)) _MPADDED(_MO(d) _MOX(rarr) _MO(r)));
                break;
            case ftErf:
                tTarget->value = _MI(erf) _MAPPLY(tTarget->value);
                break;
            case ftErfc:
                tTarget->value = _MI(erfc) _MAPPLY(tTarget->value);
                break;
            case ftGamma:
                tTarget->value = _MOX(Gamma) _MAPPLY(tTarget->value);
                break;
            case ftLGamma:
                tTarget->value = _MI(ln) _MOX(Gamma) _MAPPLY(tTarget->value);
                break;
            case ftCeil:
                tTarget->value = _MI(ceil) _MAPPLY(tTarget->value);
                break;
            case ftFloor:
                tTarget->value = _MI(floor) _MAPPLY(tTarget->value);
                break;
            case ftTrunc:
                tTarget->value = _MI(trunc) _MAPPLY(tTarget->value);
                break;
            case ftRound:
                tTarget->value = _MI(round) _MAPPLY(tTarget->value);
                break;
            case ftInt:
                tTarget->value = _MOPRE([) _MSTR(tTarget->value) _MOPRE(]);
                break;
            }
        }
    };

    // Assignation (reference)
    class MMLAssignToken: public Token {
    protected:
        Input name;
    public:
        MMLAssignToken(Input &toName): name(toName) {}

        Level levelLeft() const {
            return levelFlushAll;
        }

        Level levelRight() const {
            return levelConst;
        }

        void onPush(Parser &parser) {
            parser.state = stateOper;
        }

        void onPop(Parser &parser) {
            check(!parser.outStack.empty(), "No operand");

            // Cast the token
            // Tokens in outStack should be numbers
            PMMLToken tTarget = dynamic_pointer_cast <MMLToken> (
                parser.outStack.back()
            );
            check(tTarget != nullptr, "Unknown operand");

            // Render assignation
            tTarget->value = _MPRE(tTarget->value) _MOX(rarr) _MIVAL(name);
        }
    };

    // Bi-operators
    class MMLBiToken: public Token {
    protected:
        BiOperType type;
    public:
        MMLBiToken(BiOperType toType): type(toType) {}

        Level levelLeft() const {
            const Level toMap[] = {levelAddSubL, levelAddSubL, levelMulDivL, levelMulDivL, levelMulDivL, levelPwrL};
            return toMap[type];
        }

        Level levelRight() const {
            const Level toMap[] = {levelAddSubR, levelAddSubR, levelMulDivR, levelMulDivR, levelMulDivR, levelPwrR};
            return toMap[type];
        }

        void onPush(Parser &parser) {
            parser.state = stateNum;
        }

        void onPop(Parser &parser) {
            check(parser.outStack.size() >= 2, "No operand");

            // Cast the tokens
            // Tokens in outStack should be numbers
            PMMLToken tRight = dynamic_pointer_cast <MMLToken> (
                parser.outStack.back()
            );
            parser.outStack.pop_back();
            PMMLToken tLeft = dynamic_pointer_cast <MMLToken> (
                parser.outStack.back()
            );

            check(tRight != nullptr && tLeft != nullptr, "Unknown operand");

            // Render calculation
            switch (type) {
            case otAdd:
                tLeft->value = _MPRE(tLeft->value) _MO(+) _MPOST(tRight->value);
                break;
            case otSub:
                tLeft->value = _MPRE(tLeft->value) _MO(-) _MPOST(tRight->value);
                break;
            case otMul:
                tLeft->value = _MPRE(tLeft->value) _MO(*) _MPOST(tRight->value);
                break;
            case otDiv:
                if (tLeft->value.type == mtBracket && tRight->value.type == mtBracket) {
                    // Fraction form
                    tLeft->value = _MFRAC(_MPADDED(_MSTR(tLeft->value)) _MPADDED(_MSTR(tRight->value)));
                } else {
                    // Divide form
                    tLeft->value = _MPRE(tLeft->value) _MOX(divide) _MPOST(tRight->value);
                }
                break;
            case otMod:
                tLeft->value = _MPRE(tLeft->value) _MO(%) _MPOST(tRight->value);
                break;
            case otPwr:
                tLeft->value = _MSUP(_MPADDED(_MPP(tLeft->value)) _MPADDED(_MSTR(tRight->value)));
                break;
            }
        }
    };

    // Mono-operators
    class MMLMonoToken: public Token {
    protected:
        MonoOperType type;
    public:
        MMLMonoToken(MonoOperType toType): type(toType) {}

        Level levelLeft() const {
            const Level toMap[] = {levelConst, levelConst, levelFacL};
            return toMap[type];
        }

        Level levelRight() const {
            const Level toMap[] = {levelAddSubR, levelAddSubR, levelConst};
            return toMap[type];
        }

        void onPush(Parser &parser) {
            const State toMap[] = {stateNum, stateNum, stateOper};
            parser.state = toMap[type];
        }

        void onPop(Parser &parser) {
            check(!parser.outStack.empty(), "No operand");

            // Cast the token
            // Tokens in outStack should be numbers
            PMMLToken tTarget = dynamic_pointer_cast <MMLToken> (
                parser.outStack.back()
            );
            check(tTarget != nullptr, "Unknown operand");

            // Do calculation
            switch (type) {
            case mtPos:
                tTarget->value = _MO(+) _MPOST(tTarget->value);
                break;
            case mtNeg:
                tTarget->value = _MO(-) _MPOST(tTarget->value);
                break;
            case mtFac:
                tTarget->value = _MPRE(tTarget->value) _MO(!);
                break;
            }
        }
    };

    // Left bracket
    class MMLLeftToken: public Token {
    public:
        Level levelLeft() const {
            return levelConst;
        }

        Level levelRight() const {
            return levelAcceptAll;
        }

        void onPush(Parser &parser) {
            parser.state = stateNum;
        }

        void onPop(Parser &parser) {
        }
    };

    // Right bracket
    class MMLRightToken: public Token {
    public:
        Level levelLeft() const {
            return levelFlushAll;
        }

        Level levelRight() const {
            return levelConst;
        }

        void onPush(Parser &parser) {
            parser.state = stateOper;
        }

        void onPop(Parser &parser) {
            check(!parser.midStack.empty(), "No left bracket");

            // Cast the token to left bracket, then delete it
            PMMLLeftToken tLB = dynamic_pointer_cast <MMLLeftToken> (
                parser.midStack.back()
            );
            check(tLB != nullptr, "Bad left bracket");

            parser.midPop();

            // Cast the token
            // Tokens in outStack should be numbers
            PMMLToken tTarget = dynamic_pointer_cast <MMLToken> (
                parser.outStack.back()
            );
            check(tTarget != nullptr, "Unknown operand");

            // Render bracket pair
            tTarget->value.type = mtBracket;
        }
    };

    // Lexers

    // Numbers
    class MMLNumLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            Input buffer = "";

            if ((*now >= '0' && *now <= '9') || *now == '.') {
                // Accepted
            } else {
                // Not a number
                return 0;
            }

            // Read number to buffer
            for (; now != end; ++now) {
                if ((*now >= '0' && *now <= '9') || *now == '.') {
                    buffer += *now;
                } else {
                    break;
                }
            }

            // Generate token

            char *endPtr;
            strtod(buffer.c_str(), &endPtr);

            check(*endPtr == 0, "Wrong format of number");

            PToken token(new MMLToken(_MNVAL(buffer)));
            parser.midPush(token);
            return 1;
        }
    };

    // Functions and constants
    class MMLNameLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            Input buffer = "";

            if ((*now >= 'A' && *now <= 'Z') || (*now >= 'a' && *now <= 'z') || *now == '_') {
                // Accepted
            } else {
                // Not a name
                return 0;
            }

            // Read name to buffer
            for (; now != end; ++now) {
                if ((*now >= 'A' && *now <= 'Z') || (*now >= 'a' && *now <= 'z') || *now == '_' || (*now >= '0' && *now <= '9')) {
                    buffer += *now;
                } else {
                    break;
                }
            }

            // Generate token

            PToken token(nullptr);
            if (GetFunc.find(buffer) != GetFunc.end()) {
                token = PToken(new MMLFuncToken(GetFunc[buffer]));
            } else {
                auto iter = MMLConst.find(buffer);
                if (iter != MMLConst.end()) {
                    token = PToken(new MMLToken(iter->second));
                } else if (GetConst.find(buffer) != GetConst.end()) {
                    token = PToken(new MMLToken(_MIVAL(buffer)));
                } else {
                    error("Unknown function or constant");
                }
            }

            parser.midPush(token);
            return 1;
        }
    };

    // Constants reference (for assignation)
    class MMLNameRefLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            Input buffer = "";

            if ((*now >= 'A' && *now <= 'Z') || (*now >= 'a' && *now <= 'z') || *now == '_') {
                // Accepted
            } else {
                // Not a name
                return 0;
            }

            // Read name to buffer
            for (; now != end; ++now) {
                if ((*now >= 'A' && *now <= 'Z') || (*now >= 'a' && *now <= 'z') || *now == '_' || (*now >= '0' && *now <= '9')) {
                    buffer += *now;
                } else {
                    break;
                }
            }

            // Generate token

            PToken token(new MMLAssignToken(buffer));

            check(GetFunc.find(buffer) == GetFunc.end(), "Can not assign to a function");

            parser.midPush(token);
            return 1;
        }
    };

    // Assignations
    // 1 -> x
    class MMLAssignLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            if (*now == '-' && now + 1 != end && *(now + 1) == '>') {
                // Accepted
                now += 2;
                parser.state = stateAssign;
                return 1;
            } else {
                // Not accepted
                return 0;
            }
        }
    };

    // Operators appear after number
    class MMLAfterNumLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            PToken token(nullptr);

            // Cast and recognise token
            switch (*now) {
            case '+':
                token = PToken(new MMLBiToken(otAdd));
                break;
            case '-':
                token = PToken(new MMLBiToken(otSub));
                break;
            case '*':
                token = PToken(new MMLBiToken(otMul));
                break;
            case '/':
                token = PToken(new MMLBiToken(otDiv));
                break;
            case '%':
                token = PToken(new MMLBiToken(otMod));
                break;
            case '^':
                token = PToken(new MMLBiToken(otPwr));
                break;
            case '!':
                token = PToken(new MMLMonoToken(mtFac));
                break;
            }

            if (token != nullptr) {
                // Accepted
                ++now;
                parser.midPush(token);
                return 1;
            } else {
                return 0;
            }
        }
    };

    // Operators appear without number before
    class MMLNoNumLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            PToken token(nullptr);

            // Cast and recognise token
            switch (*now) {
            case '+':
                token = PToken(new MMLMonoToken(mtPos));
                break;
            case '-':
                token = PToken(new MMLMonoToken(mtNeg));
                break;
            }

            if (token != nullptr) {
                // Accepted
                ++now;
                parser.midPush(token);
                return 1;
            } else {
                return 0;
            }
        }
    };

    // Left bracket
    class MMLLeftLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            if (*now == '(') {
                // Accepted
                ++now;
                PToken token(new MMLLeftToken());
                parser.midPush(token);
                return 1;
            } else {
                return 0;
            }
        }
    };

    // Right bracket
    class MMLRightLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            if (*now == ')') {
                // Accepted
                ++now;
                PToken token(new MMLRightToken());
                parser.midPush(token);
                return 1;
            } else {
                return 0;
            }
        }
    };

    // Skip blank, like white spaces
    class MMLBlankLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            switch (*now) {
            case 0:
            case '\t':
            case '\n':
            case '\r':
            case ' ':
                ++now;
                return 1;
            default:
                return 0;
            }
        }
    };

    // Implicit multiplication
    // Like "3 pi = 3 * pi"
    // Should be the last one in the lexers chain
    class MMLImplicitMulLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            PToken token(new MMLBiToken(otMul));
            parser.midPush(token);
            return 1;
        }
    };

    void MML::addFirstLexers() {
        {
            PLexer lexer(new MMLNumLexer());
            lexers[stateNum].push_back(lexer);
        }
        {
            PLexer lexer(new MMLNameLexer());
            lexers[stateNum].push_back(lexer);
        }
        {
            PLexer lexer(new MMLNameRefLexer());
            lexers[stateAssign].push_back(lexer);
        }
        {
            PLexer lexer(new MMLAssignLexer());
            lexers[stateOper].push_back(lexer);
        }
        {
            PLexer lexer(new MMLAfterNumLexer());
            lexers[stateOper].push_back(lexer);
        }
        {
            PLexer lexer(new MMLNoNumLexer());
            lexers[stateNum].push_back(lexer);
        }
        {
            PLexer lexer(new MMLLeftLexer());
            lexers[stateNum].push_back(lexer);
        }
        {
            PLexer lexer(new MMLRightLexer());
            lexers[stateOper].push_back(lexer);
        }
    }

    void MML::addLastLexers() {
        {
            PLexer lexer(new MMLBlankLexer());
            lexers[stateNum].push_back(lexer);
            lexers[stateOper].push_back(lexer);
            lexers[stateAssign].push_back(lexer);
        }
        {
            PLexer lexer(new MMLImplicitMulLexer());
            lexers[stateOper].push_back(lexer);
        }
    }

    MMLString MML::finishByData() {
        vector <PToken> result;
        finish(result);

        check(result.size() == 1, "Bad result");

        // Get result
        PMMLToken tResult = dynamic_pointer_cast <MMLToken> (
            result.back()
        );

        check(tResult != nullptr, "Bad result");

        return tResult->value;
    }
}
