#include "values.h"

void Belish::Stack::push(Belish::Value *v) {
    if (len < val.size()) val[len++] = v;
    else {
        val.push_back(v);
        len++;
    }
    if (v->type() == OBJECT) {
        ((Object*)v)->stk = this;
    } else if (v->type() == ARRAY) {
        ((Array*)v)->stk = this;
    }
    v->bind();
}

void Belish::Stack::dbg() {
    std::cout << "[STACK(" << len << ")]" << std::endl;
    for (ULL i = 0; i < len; i++)
        std::cout << "$" << i << "(" << val[i] << ")\t" << val[i]->toStringHL() << std::endl;
    std::cout << "===========" << std::endl;
}
Belish::Object::~Object() noexcept  {// 只有普通的没有循环引用的Object才会直接调析构函数
//            std::clog << "d" << this << "\n";
    for (auto& i : prop) {
        i.second->linked--;
        if (i.second->linked == 0) {
            if (stk && i.second->type() == OBJECT) {
                stk->deathObjects.erase((Object*)i.second);
                stk->objects.erase(i.second);
                ((Object*)i.second)->stk = stk;
            }
            delete i.second;
        }
    }
}

void Belish::Object::set(Belish::Value *n)  {
    auto obj = (Object*)n;
    for (auto & i : prop) {
        i.second->linked--;
        if (i.second->linked == 0) {
            if (stk && i.second->type() == OBJECT) {
                stk->deathObjects.erase((Object*)i.second);
                stk->objects.erase(i.second);
                ((Object*)i.second)->stk = stk;
            }
            delete i.second;
        }
    }
    for (auto & i : obj->prop) {
        i.second->bind();
        prop[i.first] = i.second;
    }
}

void Belish::Object::set(const string &k, Belish::Value *val) {
    auto i = prop.find(k);
    if (i != prop.end() && i->second) {
        i->second->linked--;
        if (i->second->linked == 0) {
            if (stk && i->second->type() == OBJECT) {
                stk->deathObjects.erase((Object*)i->second);
                stk->objects.erase(i->second);
                ((Object*)i->second)->stk = stk;
            }
            delete i->second;
        }
    }
    prop[k] = val;
    val->bind();
    if (val->type() == OBJECT && stk) ((Object*)val)->stk = stk;
}


void Belish::Stack::set(UL offset, Value *v) {
    auto& value = val[offset];
    if (!(--val[offset]->linked)) {
        if (value->type() == OBJECT) {
            objects.erase(value);
            deathObjects.erase((Object*)value);
            ((Object*)value)->stk = this;
        }
        delete value;
    } else if (value->type() == OBJECT && !(--objects[value])) {
        deathObjects.insert((Object*)value);
        objects.erase(value);
    }
    value = v;
    v->bind();
    if (v->type() == OBJECT) ((Object*)v)->stk = this;
    else if (v->type() == ARRAY) ((Array*)v)->stk = this;
}