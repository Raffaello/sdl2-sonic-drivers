#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <drivers/westwood/ADLDriver.hpp>
#include <memory>
#include "../test/audio/stubs/StubMixer.hpp"
#include <files/westwood/ADLFile.hpp>
#include <hardware/opl/scummvm/mame/mame.hpp>

namespace drivers
{
    namespace westwood
    {
        // TODO: Review the tests
        using audio::stubs::StubMixer;
        using hardware::opl::OplType;

        TEST(ADLDriver, cstor)
        {
            auto mixer = std::make_shared<StubMixer>();
            EXPECT_EQ(mixer.use_count(), 1);

            auto adlFile = std::make_shared<files::westwood::ADLFile>("fixtures/DUNE19.ADL");
            EXPECT_EQ(adlFile.use_count(), 1);

            auto opl = std::make_shared<hardware::opl::scummvm::mame::OPL>(OplType::OPL2, mixer);
            EXPECT_EQ(opl.use_count(), 1);
            EXPECT_EQ(mixer.use_count(), 2);

            ADLDriver adlDrv(opl, adlFile);
            EXPECT_EQ(opl.use_count(), 2);
            EXPECT_EQ(adlFile.use_count(), 2);
        }

        TEST(ADLDriver, shared_ptr)
        {
            auto mixer = std::make_shared<StubMixer>();
            EXPECT_EQ(mixer.use_count(), 1);

            auto adlFile = std::make_shared<files::westwood::ADLFile>("fixtures/DUNE19.ADL");
            EXPECT_EQ(adlFile.use_count(), 1);

            auto opl = std::make_shared<hardware::opl::scummvm::mame::OPL>(OplType::OPL2, mixer);
            EXPECT_EQ(opl.use_count(), 1);
            EXPECT_EQ(mixer.use_count(), 2);

            auto adlDrv = std::make_shared<ADLDriver>(opl, adlFile);
            EXPECT_EQ(adlDrv.use_count(), 1);
            EXPECT_EQ(opl.use_count(), 2);
            EXPECT_EQ(adlFile.use_count(), 2);
        }
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
