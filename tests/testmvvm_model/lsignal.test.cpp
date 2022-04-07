/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/signals/lsignal.h"

#include <gtest/gtest.h>

#include <memory>

//! Testing external lsignal. All tests are borrowed from
//! https://github.com/cpp11nullptr/lsignal

class LsignalTest : public ::testing::Test
{
public:
  struct SignalOwner : public lsignal::slot
  {
  };
};

TEST_F(LsignalTest, CreateSignalSignalShouldBeUnlocked)
{
  lsignal::signal<void()> sg;
  EXPECT_FALSE(sg.is_locked());
}

TEST_F(LsignalTest, LockSignalSignalShouldBeLocked)
{
  lsignal::signal<void()> sg;
  sg.set_lock(true);
  EXPECT_TRUE(sg.is_locked());
}

TEST_F(LsignalTest, UnlockSignalSignalShouldBeUnlocked)
{
  lsignal::signal<void()> sg;
  sg.set_lock(false);
  EXPECT_FALSE(sg.is_locked());
}

TEST_F(LsignalTest, CallSignalWithSingleConnectionSignalShouldBeCalled)
{
  lsignal::signal<void(int, bool)> sg;

  int paramOne = 7;
  bool paramTwo = true;

  bool receiverCalled = false;

  std::function<void(int, bool)> receiver = [=, &receiverCalled](int p0, bool p1)
  {
    receiverCalled = true;
    EXPECT_EQ(p0, paramOne);
    EXPECT_EQ(p1, paramTwo);
  };

  sg.connect(receiver);

  sg(paramOne, paramTwo);
  EXPECT_TRUE(receiverCalled);
}

TEST_F(LsignalTest, CallSignalWithMultipleConnectionsSignalShouldBeCalled)
{
  lsignal::signal<void(int, bool)> sg;

  int paramOne = 7;
  bool paramTwo = true;

  unsigned char receiverCalledTimes = 0;

  std::function<void(int, bool)> receiver = [=, &receiverCalledTimes](int p0, bool p1)
  {
    ++receiverCalledTimes;
    EXPECT_EQ(p0, paramOne);
    EXPECT_EQ(p1, paramTwo);
  };

  sg.connect(receiver);
  sg.connect(receiver);

  sg(paramOne, paramTwo);
  EXPECT_EQ(receiverCalledTimes, 2);
}

TEST_F(LsignalTest, SetSameOwnerToSeveralSignalsAllSignalsShouldBeNotifiedAboutOwnerDestruction)
{
  lsignal::signal<void()> sigOne;
  lsignal::signal<void()> sigTwo;

  bool receiverOneCalled = false;
  bool receiverTwoCalled = false;

  std::function<void()> receiverOne = [&receiverOneCalled]() { receiverOneCalled = true; };

  std::function<void()> receiverTwo = [&receiverTwoCalled]() { receiverTwoCalled = true; };

  {
    SignalOwner signalOwner;

    sigOne.connect(receiverOne, &signalOwner);
    sigTwo.connect(receiverTwo, &signalOwner);

    sigOne();
    sigTwo();

    EXPECT_TRUE(receiverOneCalled);
    EXPECT_TRUE(receiverTwoCalled);
  }

  receiverOneCalled = false;
  receiverTwoCalled = false;

  sigOne();
  sigTwo();

  EXPECT_FALSE(receiverOneCalled);
  EXPECT_FALSE(receiverTwoCalled);
}

TEST_F(LsignalTest,
       CreateSignalToSignalConnection_WhenFirstSignalIsDestroyed_SecondSignalShouldBeNotifed)
{
  bool receiverOneCalled = false;
  bool receiverTwoCalled = false;

  std::function<void()> receiverOne = [&receiverOneCalled]() { receiverOneCalled = true; };

  std::function<void()> receiverTwo = [&receiverTwoCalled]() { receiverTwoCalled = true; };

  lsignal::signal<void()> sigTwo;

  {
    lsignal::signal<void()> sigOne;

    sigOne.connect(receiverOne);
    sigTwo.connect(receiverTwo);

    sigOne.connect(&sigTwo);

    sigOne();

    EXPECT_TRUE(receiverOneCalled);
    EXPECT_TRUE(receiverTwoCalled);
  }

  sigTwo();

  EXPECT_TRUE(receiverTwoCalled);
}

TEST_F(LsignalTest,
       CreateSignalToSignalConnection_WhenSecondSignalIsDestryoed_FirstSignalShouldBeNotifed)
{
  bool receiverOneCalled = false;
  bool receiverTwoCalled = false;

  std::function<void()> receiverOne = [&receiverOneCalled]()
  {
    receiverOneCalled = true;
  };

  std::function<void()> receiverTwo = [&receiverTwoCalled]()
  {
    receiverTwoCalled = true;
  };

  lsignal::signal<void()> sigOne;

  {
    lsignal::signal<void()> sigTwo;

    sigOne.connect(receiverOne);
    sigTwo.connect(receiverTwo);

    sigOne.connect(&sigTwo);

    sigOne();

    EXPECT_TRUE(receiverOneCalled);
    EXPECT_TRUE(receiverTwoCalled);
  }

  receiverOneCalled = false;
  receiverTwoCalled = false;

  sigOne();

  EXPECT_TRUE(receiverOneCalled);
  EXPECT_FALSE(receiverTwoCalled);
}
