#include <drivers/midi/devices/SbPro2.hpp>

namespace drivers
{
    namespace midi
    {
        namespace devices
        {
            SbPro2::SbPro2(const std::shared_ptr<hardware::opl::OPL>& opl, const std::shared_ptr<audio::opl::banks::OP2Bank>& op2Bank)
                : Opl(opl, op2Bank, true)
            {
            }

            SbPro2::SbPro2(const hardware::opl::OplEmulator emuType,
                const std::shared_ptr<audio::scummvm::Mixer>& mixer,
                const std::shared_ptr<audio::opl::banks::OP2Bank>& op2Bank)
                : Opl(hardware::opl::OplType::OPL3, emuType, mixer, op2Bank)
            {
            }
        }
    }
}
