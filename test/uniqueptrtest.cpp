#include "uniqueptrtest.h"
#include <utility>
#include <vector>

#include "gmock/gmock.h"
#include "src/bc_unique_ptr.hpp"

using gl::cpp_basecamp::unique_ptr;

TEST_F(UniquePtrTest, DummyTest) {
    EXPECT_TRUE(true);
}



TEST_F(UniquePtrTest, DefaultConstruction) {
    unique_ptr<int> ptr;
    EXPECT_TRUE(ptr.get() == nullptr);
}

TEST_F(UniquePtrTest, ConstructFromPtr) {
    auto rawPtr = new int;
    unique_ptr<int> ptr(rawPtr);
    EXPECT_EQ(ptr.get(), rawPtr);
}

TEST_F(UniquePtrTest, MoveCopy) {
    auto rawPtr = new int;
    unique_ptr<int> ptr1(rawPtr);
    unique_ptr<int> ptr2(std::move(ptr1));
    EXPECT_EQ(ptr2.get(), rawPtr);
    EXPECT_EQ(ptr1.get(), nullptr);
}

TEST_F(UniquePtrTest, MoveAssignment) {
    auto rawPtr = new int;
    unique_ptr<int> ptr1(rawPtr);
    unique_ptr<int> ptr2;
    ptr2 = std::move(ptr1);
    EXPECT_EQ(ptr2.get(), rawPtr);
    EXPECT_EQ(ptr1.get(), nullptr);
}

class Foo {
public:
    virtual ~Foo() {}
    virtual void doSomething() {}
};

class MockFoo : public Foo {
public:
    MOCK_METHOD0(die, void());
    MOCK_METHOD0(doSomething, void());
    virtual ~MockFoo() { die(); }
};

TEST_F(UniquePtrTest, DestructorCalled) {
    MockFoo* foo = new MockFoo;
    EXPECT_CALL(*foo, die());
    {
        unique_ptr<MockFoo> ptr(foo);
    }
    testing::Mock::VerifyAndClearExpectations(foo);
}

TEST_F(UniquePtrTest, ReleaseTest) {
    auto rawPtr = new float;
    unique_ptr<float> ptr(rawPtr);
    EXPECT_EQ(ptr.get(), rawPtr);
    auto releasedPtr = ptr.release();
    EXPECT_EQ(releasedPtr, rawPtr);
    EXPECT_EQ(ptr.get(), nullptr);
}

TEST_F(UniquePtrTest, ContainerTest) {
    constexpr size_t ContainerSize = 100;
    std::vector<unique_ptr<int>> cont(ContainerSize);
    for (size_t i = 0; i < ContainerSize; ++i) {
        unique_ptr<int> ptr(new int);
        *ptr = i;
        cont[i] = std::move(ptr);
        EXPECT_EQ(ptr.get(), nullptr); // ownership released
    }
    for (size_t i = 0; i < ContainerSize; ++i) {
        EXPECT_EQ(*cont[i], i);
    }
}

TEST_F(UniquePtrTest, AccessMethod) {
    using NiceMockFoo = ::testing::NiceMock<MockFoo>;
    NiceMockFoo* foo = new NiceMockFoo;
    EXPECT_CALL(*foo, doSomething());
    EXPECT_CALL(*foo, die());
    unique_ptr<NiceMockFoo> ptr(foo);
    ptr->doSomething();
}

struct CustomDeleter {
    void operator()(Foo* ptr) const {
        delete ptr;
    }
};

TEST_F(UniquePtrTest, CustomDeleterTest) {
   MockFoo* foo = new MockFoo;
   EXPECT_CALL(*foo, die());
   {
       unique_ptr<MockFoo, CustomDeleter> ptr(foo);
   }
   testing::Mock::VerifyAndClearExpectations(foo);
}

TEST_F(UniquePtrTest, EqualityOperatorTest) {
    int *raw_ptr1 = new int;
    int *raw_ptr2 = new int;
    unique_ptr<int> ptr1(raw_ptr1);
    unique_ptr<int> ptr2(raw_ptr2);
    EXPECT_EQ(ptr1 == ptr1, true);
    EXPECT_EQ(ptr1 == ptr2, false);
}
