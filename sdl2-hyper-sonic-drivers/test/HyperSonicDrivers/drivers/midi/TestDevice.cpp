#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <HyperSonicDrivers/drivers/midi/Device.hpp>
#include <HyperSonicDrivers/drivers/midi/devices/SpyDevice.hpp>
#include <HyperSonicDrivers/drivers/MIDDriverMock.hpp>

namespace HyperSonicDrivers::drivers::midi
{
    TEST(Device, acquire_release)
    {
        auto device = std::make_shared<devices::SpyDevice>();

        MIDDriverMock middrv(device);
        MIDDriverMock middrv2(device);
        
        EXPECT_FALSE(device->isAcquired());
        EXPECT_TRUE(device->release(&middrv));
        EXPECT_TRUE(device->release(&middrv2));
        EXPECT_TRUE(device->release(nullptr));

        EXPECT_FALSE(device->isOwned(&middrv));
        EXPECT_FALSE(device->isOwned(&middrv2));
        EXPECT_TRUE(device->isOwned(nullptr));

        EXPECT_TRUE(device->acquire(&middrv));
        EXPECT_FALSE(device->acquire(&middrv2));
        EXPECT_FALSE(device->acquire(nullptr));

        EXPECT_TRUE(device->isAcquired());
        EXPECT_TRUE(device->isOwned(&middrv));

        EXPECT_FALSE(device->release(&middrv2));
        EXPECT_FALSE(device->release(nullptr));

        EXPECT_TRUE(device->release(&middrv));
        EXPECT_TRUE(device->release(&middrv2));
        EXPECT_TRUE(device->release(nullptr));

        EXPECT_FALSE(device->isAcquired());
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
