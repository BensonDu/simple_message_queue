
#include <gtest/gtest.h>

#include "msg_queue.h"


TEST(MsgQueueTest, RegTest)
{
    MsgQueue quque;
    quque.init(3);

    int a;
    int b;

    quque.regTopic("topic1", &a);
    quque.regTopic("topic1", &b);
    quque.regTopic("topic1", &a);                // duplicated, should not work

    EXPECT_EQ(quque.getTopicMappingSize(), 2);   // queue size should = 2
}


TEST(MsgQueueTest, enqueueTest)
{
    MsgQueue quque;

    int a;
    int b;

    quque.regTopic("topic1", &a);
    quque.enqueueData("topic1", 10);

    // quque.regTopic("topic1", &a);
    EXPECT_EQ(quque.hasData(&a), true);
    EXPECT_EQ(quque.hasData(&b), false);
}


TEST(MsgQueueTest, dequeueTest)
{
    MsgQueue quque;

    int a;
    constexpr int kInsertVal = 567;

    quque.regTopic("topic1", &a);
    quque.enqueueData("topic1", kInsertVal);

    EXPECT_EQ(quque.hasData(&a), true);

    sTransferData tdata;
    quque.dequeueData(&a, tdata);

    if(tdata.topic == "topic1"){
        int got_data = std::any_cast<int>(tdata.data);
        EXPECT_EQ(got_data, kInsertVal);
    }
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}