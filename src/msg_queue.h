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

#pragma once

#include <unordered_map>
#include <any>
#include "tqueue.h"


/*
 * The data to be transferred
 */ 
struct sTransferData
{
    std::string topic;
    std::any    data;
};


/*
 * A thread-safe message quque
 * 
 */ 
class MsgQueue
{
public:
    MsgQueue();
    ~MsgQueue();

    /**
     * Init the message queue, expected to be called after construction
     * @param queue_size the queue size, not implemented so far
     */
    void init(int queue_size = 5);

    /**
     * Register topic for a instance
     * @param topic
     * @param instance address, used to identify the consumer
     */
    bool regTopic(const std::string& topic, const void* consumer_addr);
    
    /**
     * Push data to the quque, if there is no consumer, do nothing
     * @param topic
     * @param data The data to be pushed into the queue
     */
    bool enqueueData(const std::string& topic, const std::any& data);

    /**
     * Fetch data from the consumer's queue
     * @param consumer_addr the consumer's address
     * @param data The data to be fetched from the queue (data will be popped)
     */
    bool dequeueData(const void* consumer_addr, sTransferData& data);

    /**
     * Check if there is data in the consumer's queue
     * @param consumer_addr the consumer's address
     */
    bool hasData(const void* consumer_addr){
        return m_mapInstQueue[consumer_addr].size() > 0;
    }

    // utils
    size_t getTopicMappingSize(){
        std::lock_guard<std::mutex> lock(mtx_normal_data_);
        return m_mmapTopicInst.size();
    }

private:    
    std::unordered_multimap<std::string, const void*> m_mmapTopicInst; 
    std::unordered_map<const void*, tqueue<sTransferData>> m_mapInstQueue;

    std::mutex mtx_normal_data_;
};