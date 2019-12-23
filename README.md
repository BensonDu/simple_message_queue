# simple_message_queue
A simple thread-safe message queue implemented in C++17 with multi-consumer support

## Features: 
* Thread safe
* Used within a single process
* Consumers can register the topics
* Multi-Consumer: each of the consumers has a separate message quque

## Dependencies: 
* compiler support **C++17**
* If your compiler cannot support C++17, you can use C++11 with Boost(just change **std::any** with **boost::any**)
* To run the unit tests, you have to get **gtest** installed

## Build and run: 
```cpp
cmake ..
make
./simple_mq_utest.exe
```

## Example: 
```cpp
// constuct the queue object
MsgQueue quque;

// The consumer object, can be any type
int obj_a;

// value to be sent
constexpr int kInsertVal = 567;

// "obj_a" has registered the topic: "topic1"
quque.regTopic("topic1", &obj_a);

// Push data '567' to "topic1", data will be pushed to obj_a's queue
quque.enqueueData("topic1", kInsertVal);

// get the data
sTransferData tdata;
quque.dequeueData(&obj_a, tdata);

if(tdata.topic == "topic1"){
    // The data should be 567
    int got_data = std::any_cast<int>(tdata.data);
    EXPECT_EQ(got_data, kInsertVal);
}
```