/*
 * Copyright (c) 2019 Minjie.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */


#include "msg_queue.h"


MsgQueue::MsgQueue()
{

}

MsgQueue::~MsgQueue()
{

}

void MsgQueue::init(int queue_size)
{
    
}


bool MsgQueue::regTopic(const std::string& topic, const void* consumer_addr)
{
    std::lock_guard<std::mutex> lock(mtx_normal_data_);
    // cout<<"--------------------------"<<endl;

    if(m_mmapTopicInst.end() == m_mmapTopicInst.find(topic)){
        m_mmapTopicInst.emplace(topic, consumer_addr);
        // cout<<" +++ 0 "<<topic<<", "<<consumer_addr<<endl;

    }else{

        auto pair_ret = m_mmapTopicInst.equal_range(topic);
        for(auto it = pair_ret.first; it != m_mmapTopicInst.end(); ++it)
        {
            // cout<<" ccc "<<it->first<<", "<<it->second<<endl;
            if((topic == it->first) && (consumer_addr == it->second)){
                // cout<<" !!! "<<" same item"<<endl;
                return false;
            }
        }
        
        m_mmapTopicInst.emplace(topic, consumer_addr);
        // cout<<" +++ 1 "<<topic<<", "<<consumer_addr<<endl;
    }
    // cout<<" m_mmapTopicInst.size(): "<<m_mmapTopicInst.size()<<endl;

    return true;
}


bool MsgQueue::enqueueData(const std::string& topic, const std::any& data)
{
    std::lock_guard<std::mutex> lock(mtx_normal_data_);
    if(m_mmapTopicInst.end() == m_mmapTopicInst.find(topic)){
        // cout<<" topic not registered ! "<<endl;
        return false;
    }

    auto pair_ret = m_mmapTopicInst.equal_range(topic);
    for(auto it = pair_ret.first; it != m_mmapTopicInst.end(); ++it)
    {
        sTransferData tdata;
        tdata.topic = topic;
        tdata.data  = data;

        if(m_mapInstQueue.end() == m_mapInstQueue.find(it->second)){
            static std::mutex s_mtx;
            std::lock_guard<std::mutex> lock(s_mtx);
            if(m_mapInstQueue.end() == m_mapInstQueue.find(it->second)){
                m_mapInstQueue[it->second].push(tdata);
            }

        }else{
            m_mapInstQueue[it->second].push(tdata);
        }
        // cout<<" push data: "<<topic<<", "<<it->second<<endl;
    }

    return true;
}

bool MsgQueue::dequeueData(const void* consumer_addr, sTransferData& data){
    if(0 == m_mapInstQueue[consumer_addr].size()){
        return false;
    }
    
    data = m_mapInstQueue[consumer_addr].front();
    m_mapInstQueue[consumer_addr].pop();
    
    return true;
}