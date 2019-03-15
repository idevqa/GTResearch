//
//  DGRecord.cpp
//  DGCallTrace
//
//  Created by Harry Twan on 2019/3/15.
//  Copyright © 2019 Harry Twan. All rights reserved.
//  https://github.com/ming1016/GCDFetchFeed/blob/f9d9650b264e720fea14920a3b1a046353a12690/GCDFetchFeed/GCDFetchFeed/Lib/SMLagMonitor/SMCallTraceCore.c
//

#include "DGRecord.hpp"
#include <sys/time.h>

using namespace std;

static bool _call_record_enabled = true;
static uint64_t _min_time_cost = 1000; //us
static int _max_call_depth = 3;

ThreadCallRecord::ThreadCallRecord(ThreadCallRecord const &cp) {
    this -> self = cp.self;
    this -> cls = cp.cls;
    this -> cmd = cp.cmd;
    this -> time = cp.time;
    this -> lr = cp.lr;
}

stack<ThreadNode> DGRecord::get_thread_call_stack() {
    return this -> st;
}

void DGRecord::release_thread_call_stack() {
    while (!st.empty()) {
        st.pop();
    }
}

void DGRecord::push_node(ThreadNode node) {
    st.push(node);
}

void DGRecord::push_call_record(ThreadCallRecord record) {
    bool is_main_thread = pthread_main_np();
    ThreadCallRecord *new_record = new ThreadCallRecord(record);
    if (is_main_thread && _call_record_enabled) {
        ThreadNode *node = new ThreadNode(*new_record, is_main_thread);
        push_node(*node);
    }
}

ThreadNode DGRecord::pop_call_record() {
    ThreadNode node = st.top();
    ThreadCallRecord record = node.record;
    if (node.is_main_thread && _call_record_enabled) {
        timeval now;
        gettimeofday(&now, NULL);
        uint64_t time = (now.tv_sec % 100) * 100000 + now.tv_usec;
        if (time < record.time) {
            time += 100 * 1e6;
        }
        uint64_t cost = time - record.time;
        if (cost > _min_time_cost && st.size() < _max_call_depth) {
            st.pop();
        }
    }
    return node;
}

//void before_objc_msgSend(id self, SEL _cmd, uintptr_t lr) {
//    Class _cls = object_getClass(self);
//    ThreadCallRecord *record = new ThreadCallRecord(self, _cls, _cmd, lr);
//    _record.push_call_record(*record);
//}
//
//uintptr_t after_objc_msgSend() {
//    ThreadNode node = _record.pop_call_record();
//    uintptr_t ptr = node.record.lr;
//    return ptr;
//}

