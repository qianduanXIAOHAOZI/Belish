#include <iostream>
#include <cstring>
#include "compiler.h"
#include "vm.h"
#include "trans.h"
#include "trans.cpp"
#include "values.h"

void Belish::BVM::run() {
    Byte byte;
    Dbyte dbyte;
    Qbyte qbyte;
    Ebyte ebyte;
    ULL i = 0;
#define GETBYTE byte = bytecode[i++]
#define GETDBYTE \
    GETBYTE; \
    dbyte = 0; \
    dbyte = dbyte << 8 | byte; \
    GETBYTE; \
    dbyte = dbyte << 8 | byte;
#define GETQBYTE \
    qbyte = 0; \
    GETBYTE; \
    qbyte = qbyte << 8 | byte; \
    GETBYTE; \
    qbyte = qbyte << 8 | byte; \
    GETBYTE; \
    qbyte = qbyte << 8 | byte; \
    GETBYTE; \
    qbyte = qbyte << 8 | byte;
#define GETEBYTE \
    ebyte = 0; \
    GETBYTE; \
    ebyte = ebyte << 8 | byte; \
    GETBYTE; \
    ebyte = ebyte << 8 | byte; \
    GETBYTE; \
    ebyte = ebyte << 8 | byte; \
    GETBYTE; \
    ebyte = ebyte << 8 | byte; \
    GETBYTE; \
    ebyte = ebyte << 8 | byte; \
    GETBYTE; \
    ebyte = ebyte << 8 | byte; \
    GETBYTE; \
    ebyte = ebyte << 8 | byte; \
    GETBYTE; \
    ebyte = ebyte << 8 | byte;
    GETQBYTE
    if (qbyte != 0x9ad0755c) {
        std::cerr << "Wrong magic code" << std::endl;
        return;
    }
    GETDBYTE
    GETEBYTE
    Stack stk;
    for (; i < len; ) {
        GETBYTE;
        auto op = (OPID)byte;
        auto a = stk.get(stk.length() - 2);
        auto b = stk.get(stk.length() - 1);
        switch (op) {
            case ADD: {
                if (a->type() == b->type()) a->add(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case SUB: {
                if (a->type() == b->type()) a->sub(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case MUL: {
                if (a->type() == b->type() || (b->type() == NUMBER && a->type() == STRING)) a->mul(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case DIV: {
                if (a->type() == b->type()) a->div(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case MOD: {
                if (a->type() == b->type()) a->mod(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case EQ: {
                if (a->type() == b->type()) a->eq(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case NEQ: {
                if (a->type() == b->type()) a->neq(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case LEQ: {
                if (a->type() == b->type()) a->leq(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case MEQ: {
                if (a->type() == b->type()) a->meq(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case LESS: {
                if (a->type() == b->type()) a->less(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case MORE: {
                if (a->type() == b->type()) a->more(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case MAND: {
                if (a->type() == b->type()) a->mand(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case MOR: {
                if (a->type() == b->type()) a->mor(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case MXOR: {
                if (a->type() == b->type()) a->mxor(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case LAND: {
                if (a->type() == b->type()) a->land(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case LOR: {
                if (a->type() == b->type()) a->lor(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case POW: {
                if (a->type() == b->type()) a->pow(b);
                else { std::cerr << "Wrong type" << std::endl; return; }
                stk.pop(1);
                break;
            }
            case PUSH_NUM:
                GETEBYTE
                stk.push(new Number(transI64D_bin(ebyte)));
                break;
            case PUSH_STR: {
                GETQBYTE
                UL strlen = qbyte;
                string str;
                for (auto j = 0; j < strlen; j++, i++) {
                    str += bytecode[i];
                }
                stk.push(new String(str));
                break;
            }
            case PUSH_UND: {
                stk.push(new Undefined);
                break;
            }
            case DEB:
                stk.dbg();
                break;
        }
    }
}