/**
 * @file MessageEncoding.hpp
 * @author paul
 * @date 20.10.22
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_MESSAGEENCODING_HPP
#define FLIGHTCOMPUTER_MESSAGEENCODING_HPP

#include <pb_encode.h>

#include "../Lib/MessageConversion.hpp"

namespace filter {
    template<typename T>
    class MessageEncoding : public Node {
      public:
        MessageEncoding();

        void run() override;

        auto getChannelIn() -> InputChannel<T> &;

        auto getChannelOut() -> OutputChannel<uint8_t> &;

      private:
        InputChannel<T> inputChannel;
        OutputChannel<uint8_t> outputChannel;

        using Conversion = lib::messages::Conversion<T>;
        static constexpr auto START_BYTE = 0x0F;
        static constexpr auto ID_BYTE = Conversion::ID;
        static constexpr auto END_BYTE = 0xFF;
        constexpr static auto BUF_SIZE = 1024;
    };

    template<typename T>
    MessageEncoding<T>::MessageEncoding() {
        this->start();
    }

    template<typename T>
    void MessageEncoding<T>::run() {
        T t;
        std::array<uint8_t, BUF_SIZE> buf{};
        while (not inputChannel.isClosed()) {
            inputChannel.get(t);
            auto protobufType = Conversion::toProtobuf(t);

            auto ostream = pb_ostream_from_buffer(buf.data() + 2, buf.size() - 2);
            pb_encode(&ostream, Conversion::description, &protobufType);
            buf[0] = START_BYTE;
            buf[1] = ID_BYTE;
            buf[ostream.bytes_written + 2] = END_BYTE;

            for (auto c = 0U; c < ostream.bytes_written + 3; ++c) {
                outputChannel.put(buf[c]);
            }
        }
    }

    template<typename T>
    auto MessageEncoding<T>::getChannelIn() -> InputChannel<T> & {
        return inputChannel;
    }

    template<typename T>
    auto MessageEncoding<T>::getChannelOut() -> OutputChannel<uint8_t> & {
        return outputChannel;
    }
} // namespace filters

#endif // FLIGHTCOMPUTER_MESSAGEENCODING_HPP
