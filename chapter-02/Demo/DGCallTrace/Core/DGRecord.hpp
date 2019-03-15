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
};

class ThreadNode {
public:
    ThreadCallRecord record;
    int allocted_length;
    int index;
    bool is_main_thread;
};

class DGRecord {
public:
    std::stack<ThreadNode> st;
    std::stack<ThreadNode> get_thread_call_stack();
    void release_thread_call_stack();
    void push_node(ThreadNode node);
    void push_call_record(ThreadCallRecord record);
    ThreadNode pop_call_record();
    
private:
    void before_objc_msgSend(id self, SEL _cmd, uintptr_t);
    uintptr_t after_objc_msgSend();
};

#endif /* DGRecord_hpp */
