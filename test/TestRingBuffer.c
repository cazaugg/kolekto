// ────────────────────────────────────────────────────────────────────────────
// TestRingBuffer — Unity tests for the RingBuffer module.
// ────────────────────────────────────────────────────────────────────────────

#include "unity.h"
#include "../src/RingBuffer.h"

void setUp(void)
{
}

void tearDown(void)
{
}

static void Test_RingBuffer_InitialState(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);

    TEST_ASSERT_EQUAL_size_t(0, RingBuffer_Count(&buffer));
    TEST_ASSERT_EQUAL_size_t(8, RingBuffer_Capacity(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_IsEmpty(&buffer));
    TEST_ASSERT_FALSE(RingBuffer_IsFull(&buffer));
}

static void Test_RingBuffer_WriteOne_Valid(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 value = 0;

    TEST_ASSERT_TRUE(RingBuffer_WriteOne(&buffer, 'a'));
    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_Count(&buffer));
    TEST_ASSERT_FALSE(RingBuffer_IsEmpty(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 0, &value));
    TEST_ASSERT_EQUAL_UINT8('a', value);
}

static void Test_RingBuffer_WriteOne_Full(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(4);

    TEST_ASSERT_TRUE(RingBuffer_WriteOne(&buffer, 1));
    TEST_ASSERT_TRUE(RingBuffer_WriteOne(&buffer, 2));
    TEST_ASSERT_TRUE(RingBuffer_WriteOne(&buffer, 3));
    TEST_ASSERT_TRUE(RingBuffer_WriteOne(&buffer, 4));
    TEST_ASSERT_TRUE(RingBuffer_IsFull(&buffer));
    TEST_ASSERT_EQUAL_size_t(4, RingBuffer_Count(&buffer));
    TEST_ASSERT_FALSE(RingBuffer_WriteOne(&buffer, 5));
}

static void Test_RingBuffer_ReadOne_Valid(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 value = 0;

    RingBuffer_WriteOne(&buffer, 'a');
    RingBuffer_WriteOne(&buffer, 'b');

    TEST_ASSERT_TRUE(RingBuffer_ReadOne(&buffer, &value));
    TEST_ASSERT_EQUAL_UINT8('a', value);
    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_Count(&buffer));

    TEST_ASSERT_TRUE(RingBuffer_ReadOne(&buffer, &value));
    TEST_ASSERT_EQUAL_UINT8('b', value);
    TEST_ASSERT_TRUE(RingBuffer_IsEmpty(&buffer));
}

static void Test_RingBuffer_ReadOne_Empty(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 value = 7;

    TEST_ASSERT_FALSE(RingBuffer_ReadOne(&buffer, &value));
    TEST_ASSERT_EQUAL_UINT8(7, value);
}

static void Test_RingBuffer_FifoOrder(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(4);
    u8 value = 0;

    for(u8 i = 0; i < 4; i++)
    {
        TEST_ASSERT_TRUE(RingBuffer_WriteOne(&buffer, (u8)(i + 1)));
    }

    for(u8 i = 0; i < 4; i++)
    {
        TEST_ASSERT_TRUE(RingBuffer_ReadOne(&buffer, &value));
        TEST_ASSERT_EQUAL_UINT8((u8)(i + 1), value);
    }
}

static void Test_RingBuffer_WrapAround(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(4);
    u8 value = 0;

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);
    RingBuffer_WriteOne(&buffer, 3);
    RingBuffer_ReadOne(&buffer, &value);
    RingBuffer_ReadOne(&buffer, &value);

    TEST_ASSERT_TRUE(RingBuffer_WriteOne(&buffer, 4));
    TEST_ASSERT_TRUE(RingBuffer_WriteOne(&buffer, 5));
    TEST_ASSERT_TRUE(RingBuffer_WriteOne(&buffer, 6));
    TEST_ASSERT_TRUE(RingBuffer_IsFull(&buffer));

    const u8 expected[4] = {3, 4, 5, 6};
    for(u8 i = 0; i < 4; i++)
    {
        TEST_ASSERT_TRUE(RingBuffer_ReadOne(&buffer, &value));
        TEST_ASSERT_EQUAL_UINT8(expected[i], value);
    }
}

static void Test_RingBuffer_PeekOne_Valid(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 value = 0;

    RingBuffer_WriteOne(&buffer, 10);
    RingBuffer_WriteOne(&buffer, 20);
    RingBuffer_WriteOne(&buffer, 30);

    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 0, &value));
    TEST_ASSERT_EQUAL_UINT8(10, value);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 1, &value));
    TEST_ASSERT_EQUAL_UINT8(20, value);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 2, &value));
    TEST_ASSERT_EQUAL_UINT8(30, value);
    TEST_ASSERT_EQUAL_size_t(3, RingBuffer_Count(&buffer));
}

static void Test_RingBuffer_PeekOne_OutOfRange(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 value = 0;

    RingBuffer_WriteOne(&buffer, 10);
    RingBuffer_WriteOne(&buffer, 20);

    TEST_ASSERT_FALSE(RingBuffer_PeekOne(&buffer, 2, &value));
    TEST_ASSERT_FALSE(RingBuffer_PeekOne(&buffer, 99, &value));
}

static void Test_RingBuffer_Write_Bulk(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    const u8 input[3] = {1, 2, 3};

    TEST_ASSERT_EQUAL_size_t(3, RingBuffer_Write(&buffer, 3, input));
    TEST_ASSERT_EQUAL_size_t(3, RingBuffer_Count(&buffer));
}

static void Test_RingBuffer_Write_Overflow(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(4);
    const u8 input[6] = {1, 2, 3, 4, 5, 6};

    TEST_ASSERT_EQUAL_size_t(4, RingBuffer_Write(&buffer, 6, input));
    TEST_ASSERT_TRUE(RingBuffer_IsFull(&buffer));
    TEST_ASSERT_EQUAL_size_t(4, RingBuffer_Count(&buffer));
}

static void Test_RingBuffer_Read_Bulk(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 output[3] = {0};

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);
    RingBuffer_WriteOne(&buffer, 3);

    TEST_ASSERT_EQUAL_size_t(3, RingBuffer_Read(&buffer, 3, output));
    TEST_ASSERT_EQUAL_UINT8(1, output[0]);
    TEST_ASSERT_EQUAL_UINT8(2, output[1]);
    TEST_ASSERT_EQUAL_UINT8(3, output[2]);
    TEST_ASSERT_TRUE(RingBuffer_IsEmpty(&buffer));
}

static void Test_RingBuffer_Read_Underflow(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 output[5] = {0};

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);

    TEST_ASSERT_EQUAL_size_t(2, RingBuffer_Read(&buffer, 5, output));
    TEST_ASSERT_EQUAL_UINT8(1, output[0]);
    TEST_ASSERT_EQUAL_UINT8(2, output[1]);
    TEST_ASSERT_TRUE(RingBuffer_IsEmpty(&buffer));
}

static void Test_RingBuffer_Clear(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);
    RingBuffer_WriteOne(&buffer, 3);

    RingBuffer_Clear(&buffer);
    TEST_ASSERT_TRUE(RingBuffer_IsEmpty(&buffer));
    TEST_ASSERT_EQUAL_size_t(0, RingBuffer_Count(&buffer));
}

static void Test_RingBuffer_Peek_Bulk(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 output[3] = {0};

    RingBuffer_WriteOne(&buffer, 10);
    RingBuffer_WriteOne(&buffer, 20);
    RingBuffer_WriteOne(&buffer, 30);

    TEST_ASSERT_TRUE(RingBuffer_Peek(&buffer, 0, 3, output));
    TEST_ASSERT_EQUAL_UINT8(10, output[0]);
    TEST_ASSERT_EQUAL_UINT8(20, output[1]);
    TEST_ASSERT_EQUAL_UINT8(30, output[2]);
    TEST_ASSERT_EQUAL_size_t(3, RingBuffer_Count(&buffer));

    TEST_ASSERT_TRUE(RingBuffer_Peek(&buffer, 1, 2, output));
    TEST_ASSERT_EQUAL_UINT8(20, output[0]);
    TEST_ASSERT_EQUAL_UINT8(30, output[1]);
}

static void Test_RingBuffer_Peek_PartialAvailable(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 output[5] = {0};

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);

    TEST_ASSERT_TRUE(RingBuffer_Peek(&buffer, 1, 5, output));
    TEST_ASSERT_EQUAL_UINT8(2, output[0]);
    TEST_ASSERT_EQUAL_UINT8(0, output[1]);
    TEST_ASSERT_EQUAL_size_t(2, RingBuffer_Count(&buffer));
}

static void Test_RingBuffer_Peek_BulkOutOfRange(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 output[2] = {0};

    RingBuffer_WriteOne(&buffer, 1);

    TEST_ASSERT_FALSE(RingBuffer_Peek(&buffer, 1, 1, output));
    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_Count(&buffer));
}

static void Test_RingBuffer_Peek_BulkWrapAround(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(4);
    u8 value = 0;
    u8 output[3] = {0};

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);
    RingBuffer_WriteOne(&buffer, 3);
    RingBuffer_ReadOne(&buffer, &value);
    RingBuffer_WriteOne(&buffer, 4);

    TEST_ASSERT_TRUE(RingBuffer_Peek(&buffer, 0, 3, output));
    TEST_ASSERT_EQUAL_UINT8(2, output[0]);
    TEST_ASSERT_EQUAL_UINT8(3, output[1]);
    TEST_ASSERT_EQUAL_UINT8(4, output[2]);
}

static void Test_RingBuffer_RemoveOne_Front(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 data = 0;
    u8 peeked = 0;

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);
    RingBuffer_WriteOne(&buffer, 3);

    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_RemoveOne(&buffer, 0, &data));
    TEST_ASSERT_EQUAL_UINT8(1, data);
    TEST_ASSERT_EQUAL_size_t(2, RingBuffer_Count(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 0, &peeked));
    TEST_ASSERT_EQUAL_UINT8(2, peeked);
}

static void Test_RingBuffer_RemoveOne_Middle(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 data = 0;
    u8 peeked = 0;

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);
    RingBuffer_WriteOne(&buffer, 3);
    RingBuffer_WriteOne(&buffer, 4);

    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_RemoveOne(&buffer, 1, &data));
    TEST_ASSERT_EQUAL_UINT8(2, data);
    TEST_ASSERT_EQUAL_size_t(3, RingBuffer_Count(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 0, &peeked));
    TEST_ASSERT_EQUAL_UINT8(1, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 1, &peeked));
    TEST_ASSERT_EQUAL_UINT8(3, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 2, &peeked));
    TEST_ASSERT_EQUAL_UINT8(4, peeked);
}

static void Test_RingBuffer_RemoveOne_End(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 data = 0;
    u8 peeked = 0;

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);
    RingBuffer_WriteOne(&buffer, 3);

    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_RemoveOne(&buffer, 2, &data));
    TEST_ASSERT_EQUAL_UINT8(3, data);
    TEST_ASSERT_EQUAL_size_t(2, RingBuffer_Count(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 1, &peeked));
    TEST_ASSERT_EQUAL_UINT8(2, peeked);
}

static void Test_RingBuffer_RemoveOne_OutOfRange(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 data = 0;

    RingBuffer_WriteOne(&buffer, 1);

    TEST_ASSERT_EQUAL_size_t(0, RingBuffer_RemoveOne(&buffer, 1, &data));
    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_Count(&buffer));
}

static void Test_RingBuffer_Remove_Bulk(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 output[3] = {0};
    u8 peeked = 0;

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);
    RingBuffer_WriteOne(&buffer, 3);
    RingBuffer_WriteOne(&buffer, 4);
    RingBuffer_WriteOne(&buffer, 5);

    TEST_ASSERT_EQUAL_size_t(3, RingBuffer_Remove(&buffer, 1, 3, output));
    TEST_ASSERT_EQUAL_UINT8(2, output[0]);
    TEST_ASSERT_EQUAL_UINT8(3, output[1]);
    TEST_ASSERT_EQUAL_UINT8(4, output[2]);
    TEST_ASSERT_EQUAL_size_t(2, RingBuffer_Count(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 0, &peeked));
    TEST_ASSERT_EQUAL_UINT8(1, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 1, &peeked));
    TEST_ASSERT_EQUAL_UINT8(5, peeked);
}

static void Test_RingBuffer_Remove_Underflow(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 output[5] = {0};

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);

    TEST_ASSERT_EQUAL_size_t(2, RingBuffer_Remove(&buffer, 0, 5, output));
    TEST_ASSERT_EQUAL_UINT8(1, output[0]);
    TEST_ASSERT_EQUAL_UINT8(2, output[1]);
    TEST_ASSERT_TRUE(RingBuffer_IsEmpty(&buffer));
}

static void Test_RingBuffer_Remove_OutOfRange(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    u8 output[2] = {0};

    RingBuffer_WriteOne(&buffer, 1);

    TEST_ASSERT_EQUAL_size_t(0, RingBuffer_Remove(&buffer, 1, 1, output));
    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_Count(&buffer));
}

static void Test_RingBuffer_Remove_WrapAround(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(4);
    u8 value = 0;
    u8 output[2] = {0};
    u8 peeked = 0;

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);
    RingBuffer_WriteOne(&buffer, 3);
    RingBuffer_ReadOne(&buffer, &value);
    RingBuffer_ReadOne(&buffer, &value);
    RingBuffer_WriteOne(&buffer, 4);
    RingBuffer_WriteOne(&buffer, 5);

    TEST_ASSERT_EQUAL_size_t(2, RingBuffer_Remove(&buffer, 1, 2, output));
    TEST_ASSERT_EQUAL_UINT8(4, output[0]);
    TEST_ASSERT_EQUAL_UINT8(5, output[1]);
    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_Count(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 0, &peeked));
    TEST_ASSERT_EQUAL_UINT8(3, peeked);
}

static void Test_RingBuffer_InsertOne_Front(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    const u8 value = 9;
    u8 peeked = 0;

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);

    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_InsertOne(&buffer, 0, &value));
    TEST_ASSERT_EQUAL_size_t(3, RingBuffer_Count(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 0, &peeked));
    TEST_ASSERT_EQUAL_UINT8(9, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 1, &peeked));
    TEST_ASSERT_EQUAL_UINT8(1, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 2, &peeked));
    TEST_ASSERT_EQUAL_UINT8(2, peeked);
}

static void Test_RingBuffer_InsertOne_Middle(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    const u8 value = 2;
    u8 peeked = 0;

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 3);

    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_InsertOne(&buffer, 1, &value));
    TEST_ASSERT_EQUAL_size_t(3, RingBuffer_Count(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 0, &peeked));
    TEST_ASSERT_EQUAL_UINT8(1, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 1, &peeked));
    TEST_ASSERT_EQUAL_UINT8(2, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 2, &peeked));
    TEST_ASSERT_EQUAL_UINT8(3, peeked);
}

static void Test_RingBuffer_InsertOne_Append(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    const u8 value = 2;
    u8 peeked = 0;

    RingBuffer_WriteOne(&buffer, 1);

    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_InsertOne(&buffer, 1, &value));
    TEST_ASSERT_EQUAL_size_t(2, RingBuffer_Count(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 0, &peeked));
    TEST_ASSERT_EQUAL_UINT8(1, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 1, &peeked));
    TEST_ASSERT_EQUAL_UINT8(2, peeked);
}

static void Test_RingBuffer_InsertOne_Full(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(2);
    const u8 value = 9;

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);

    TEST_ASSERT_EQUAL_size_t(0, RingBuffer_InsertOne(&buffer, 0, &value));
    TEST_ASSERT_EQUAL_size_t(2, RingBuffer_Count(&buffer));
}

static void Test_RingBuffer_Insert_Bulk(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    const u8 input[3] = {7, 8, 9};
    u8 peeked = 0;

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);
    RingBuffer_WriteOne(&buffer, 3);

    TEST_ASSERT_EQUAL_size_t(3, RingBuffer_Insert(&buffer, 1, 3, input));
    TEST_ASSERT_EQUAL_size_t(6, RingBuffer_Count(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 0, &peeked));
    TEST_ASSERT_EQUAL_UINT8(1, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 1, &peeked));
    TEST_ASSERT_EQUAL_UINT8(7, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 2, &peeked));
    TEST_ASSERT_EQUAL_UINT8(8, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 3, &peeked));
    TEST_ASSERT_EQUAL_UINT8(9, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 4, &peeked));
    TEST_ASSERT_EQUAL_UINT8(2, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 5, &peeked));
    TEST_ASSERT_EQUAL_UINT8(3, peeked);
}

static void Test_RingBuffer_Insert_Overflow(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(4);
    const u8 input[4] = {7, 8, 9, 10};
    u8 peeked = 0;

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);

    TEST_ASSERT_EQUAL_size_t(2, RingBuffer_Insert(&buffer, 0, 4, input));
    TEST_ASSERT_EQUAL_size_t(4, RingBuffer_Count(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 0, &peeked));
    TEST_ASSERT_EQUAL_UINT8(7, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 1, &peeked));
    TEST_ASSERT_EQUAL_UINT8(8, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 2, &peeked));
    TEST_ASSERT_EQUAL_UINT8(1, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 3, &peeked));
    TEST_ASSERT_EQUAL_UINT8(2, peeked);
}

static void Test_RingBuffer_Insert_OutOfRange(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(8);
    const u8 input[1] = {9};

    RingBuffer_WriteOne(&buffer, 1);

    TEST_ASSERT_EQUAL_size_t(0, RingBuffer_Insert(&buffer, 2, 1, input));
    TEST_ASSERT_EQUAL_size_t(1, RingBuffer_Count(&buffer));
}

static void Test_RingBuffer_Insert_WrapAround(void)
{
    RingBuffer buffer = NEW_RING_BUFFER(4);
    u8 value = 0;
    const u8 input[2] = {8, 9};
    u8 peeked = 0;

    RingBuffer_WriteOne(&buffer, 1);
    RingBuffer_WriteOne(&buffer, 2);
    RingBuffer_WriteOne(&buffer, 3);
    RingBuffer_ReadOne(&buffer, &value);
    RingBuffer_ReadOne(&buffer, &value);
    RingBuffer_WriteOne(&buffer, 4);

    TEST_ASSERT_EQUAL_size_t(2, RingBuffer_Insert(&buffer, 1, 2, input));
    TEST_ASSERT_EQUAL_size_t(4, RingBuffer_Count(&buffer));
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 0, &peeked));
    TEST_ASSERT_EQUAL_UINT8(3, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 1, &peeked));
    TEST_ASSERT_EQUAL_UINT8(8, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 2, &peeked));
    TEST_ASSERT_EQUAL_UINT8(9, peeked);
    TEST_ASSERT_TRUE(RingBuffer_PeekOne(&buffer, 3, &peeked));
    TEST_ASSERT_EQUAL_UINT8(4, peeked);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_RingBuffer_InitialState);
    RUN_TEST(Test_RingBuffer_WriteOne_Valid);
    RUN_TEST(Test_RingBuffer_WriteOne_Full);
    RUN_TEST(Test_RingBuffer_ReadOne_Valid);
    RUN_TEST(Test_RingBuffer_ReadOne_Empty);
    RUN_TEST(Test_RingBuffer_FifoOrder);
    RUN_TEST(Test_RingBuffer_WrapAround);
    RUN_TEST(Test_RingBuffer_PeekOne_Valid);
    RUN_TEST(Test_RingBuffer_PeekOne_OutOfRange);
    RUN_TEST(Test_RingBuffer_Write_Bulk);
    RUN_TEST(Test_RingBuffer_Write_Overflow);
    RUN_TEST(Test_RingBuffer_Read_Bulk);
    RUN_TEST(Test_RingBuffer_Read_Underflow);
    RUN_TEST(Test_RingBuffer_Clear);
    RUN_TEST(Test_RingBuffer_Peek_Bulk);
    RUN_TEST(Test_RingBuffer_Peek_PartialAvailable);
    RUN_TEST(Test_RingBuffer_Peek_BulkOutOfRange);
    RUN_TEST(Test_RingBuffer_Peek_BulkWrapAround);
    RUN_TEST(Test_RingBuffer_RemoveOne_Front);
    RUN_TEST(Test_RingBuffer_RemoveOne_Middle);
    RUN_TEST(Test_RingBuffer_RemoveOne_End);
    RUN_TEST(Test_RingBuffer_RemoveOne_OutOfRange);
    RUN_TEST(Test_RingBuffer_Remove_Bulk);
    RUN_TEST(Test_RingBuffer_Remove_Underflow);
    RUN_TEST(Test_RingBuffer_Remove_OutOfRange);
    RUN_TEST(Test_RingBuffer_Remove_WrapAround);
    RUN_TEST(Test_RingBuffer_InsertOne_Front);
    RUN_TEST(Test_RingBuffer_InsertOne_Middle);
    RUN_TEST(Test_RingBuffer_InsertOne_Append);
    RUN_TEST(Test_RingBuffer_InsertOne_Full);
    RUN_TEST(Test_RingBuffer_Insert_Bulk);
    RUN_TEST(Test_RingBuffer_Insert_Overflow);
    RUN_TEST(Test_RingBuffer_Insert_OutOfRange);
    RUN_TEST(Test_RingBuffer_Insert_WrapAround);
    return UNITY_END();
}
