#ifndef BELISH_VM_H
#define BELISH_VM_H

#include <string>
#include <map>
#include "values.h"
#include "compiler.h"
#include "dylib.h"
#include "GC.h"
#include "arg.h"
using std::string;

namespace Belish {
    class BVM {
    public:
        BVM(const string& fn, char* bc, ULL l) : filename(fn), bytecode(bc), len(l), gc(this) { modules.reserve(16);frames.reserve(16); }
        void run(const Arg&);
        void Throw(UL errID, const string& message) {
            std::cerr << "BLE" << errID << ": " << message;
            auto iter = --callingLineStk.cend();
            std::cerr << " at <" << filename << ">:" << *(iter--) << "\n";
            while (iter != callingLineStk.cend()) std::cerr << " at <" << filename << ">:" << *(iter--) << "\n";
            error = true;
        }
        ~BVM() {
            delete stk;
        }
    private:
        bool error = false;
        bool child = false;
        bool callModuleMethod = false;
        UL functionLen;
        UL footerAdr;
        UL i;
        UL inFun = 0;
        ULL len;
        char* bytecode;
        GC gc;
        map<string, bool>* importedTab = nullptr;
        Stack* stk = nullptr;
        Value* regs[REG_COUNT] = { nullptr };
        std::map<void*, UL> objects;
        std::set<Object*> deathObjects;
        vector<BVM*> modules;
        vector<Dylib*> exlibs;
        vector<Stack*> frames;
        vector<UL> functions;
        std::list<UL> callingLineStk;
        string filename;
        friend class GC;
    };
    typedef Object* (*ModuleSetup)();
}


#endif //BELISH_VM_H
