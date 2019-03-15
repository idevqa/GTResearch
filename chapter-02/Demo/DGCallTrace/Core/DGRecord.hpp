//
//  DGRecord.hpp
//  DGCallTrace
//
//  Created by Harry Twan on 2019/3/15.
//  Copyright © 2019 Harry Twan. All rights reserved.
//

#ifndef DGRecord_hpp
#define DGRecord_hpp

#include <stdio.h>
#include <stack>
#include <cstddef>
#include <cstdint>
#include <objc/runtime.h>
#include <pthread.h>
#include <stack>

class ThreadCallRecord {
public:
    id self;
    Class cls;
    SEL cmd;
    uint64_t time;
    uintptr_t lr;
    
    ThreadCallRecord(ThreadCallRecord const &cp);
    ThreadCallRecord(id _s, Class _cls, SEL _cmd, uintptr_t _lr): self(_s), cls(_cls), cmd(_cmd), lr(_lr){};
};

class ThreadNode {
public:
    ThreadCallRecord record;
    bool is_main_thread;
    
    ThreadNode(ThreadCallRecord rec, bool imt): record(rec), is_main_thread(imt) {};
};

class DGRecord {
public:
    std::stack<ThreadNode> st;
    
    DGRecord();
    std::stack<ThreadNode> get_thread_call_stack();
    void release_thread_call_stack();
    void push_node(ThreadNode node);
    void push_call_record(ThreadCallRecord record);
    ThreadNode pop_call_record();
    
private:
    static DGRecord* instance;

private:
    void before_objc_msgSend(id self, SEL _cmd, uintptr_t);
    uintptr_t after_objc_msgSend();
};

void before_objc_msgSend(id self, SEL _cmd, uintptr_t lr);
uintptr_t after_objc_msgSend();

#endif /* DGRecord_hpp */
