//
//  DGRecord.cpp
//  DGCallTrace
//
//  Created by Harry Twan on 2019/3/15.
//  Copyright © 2019 Harry Twan. All rights reserved.
//  https://github.com/ming1016/GCDFetchFeed/blob/f9d9650b264e720fea14920a3b1a046353a12690/GCDFetchFeed/GCDFetchFeed/Lib/SMLagMonitor/SMCallTraceCore.c
//

#include "DGRecord.hpp"

using namespace std;

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
    
}
