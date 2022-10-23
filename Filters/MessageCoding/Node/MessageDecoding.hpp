/**
 * @file MessageDecoding.hpp
 * @author paul
 * @date 20.10.22
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_MESSAGEDECODING_HPP
#define FLIGHTCOMPUTER_MESSAGEDECODING_HPP

#include "../Lib/MessageConversion.hpp"
#include "pb_decode.h"

namespace filter {
    enum class DecodingState { INITIAL, END_FOUND, FIRST_FOUND, IN_DATA, IN_WRONG_DATA };

    template<typename T>
    class MessageDecoding : public Node {
      public:
        MessageDecoding();

        void run() override;

        auto getChannelIn() -> InputChannel<uint8_t> &;

        auto getChannelOut() -> OutputChannel<T> &;

      private:
        using Conversion = lib::messages::Conversion<T>;

        static constexpr auto START_BYTE = 0x0F;
        static constexpr auto ID_BYTE = Conversion::ID;
        static constexpr auto END_BYTE = 0xFF;

        InputChannel<uint8_t> inputChannel;
        OutputChannel<T> outputChannel;
    };

    template<typename T>
    MessageDecoding<T>::MessageDecoding() {
        this->start();
    }

    template<typename T>
    void MessageDecoding<T>::run() {
        DecodingState decodingState = DecodingState::INITIAL;
        std::vector<uint8_t> buf;
        while (not inputChannel.isClosed()) {
            uint8_t data;
            inputChannel.get(data);

            switch (decodingState) {
                case DecodingState::INITIAL:
                    if (data == END_BYTE) {
                        decodingState = DecodingState::END_FOUND;
                    }
                    break;
                case DecodingState::END_FOUND:
                    if (data == START_BYTE) {
                        decodingState = DecodingState::FIRST_FOUND;
                        if (not buf.empty()) {
                            buf.pop_back();

                            auto istream = pb_istream_from_buffer(buf.data(), buf.size());

                            typename Conversion::ProtobufType protobufType;
                            pb_decode(&istream, Conversion::description, &protobufType);
                            T t = Conversion::fromProtobuf(protobufType);
                            outputChannel.put(std::move(t));

                            buf.clear();
                        }
                    } else {
                        if (buf.empty()) {
                            decodingState = DecodingState::INITIAL;
                        } else {
                            decodingState = DecodingState::IN_DATA;
                            buf.emplace_back(data);
                        }
                    }
                    break;
                case DecodingState::FIRST_FOUND:
                    if (data == ID_BYTE) {
                        decodingState = DecodingState::IN_DATA;
                    } else {
                        decodingState = DecodingState::IN_WRONG_DATA;
                    }
                    break;
                case DecodingState::IN_DATA:
                    if (data == END_BYTE) {
                        decodingState = DecodingState::END_FOUND;
                    }
                    buf.emplace_back(data);
                    break;
                case DecodingState::IN_WRONG_DATA:
                    if (data == END_BYTE) {
                        decodingState = DecodingState::END_FOUND;
                    }
                    break ;
            }
        }
    }

    template<typename T>
    auto MessageDecoding<T>::getChannelIn() -> InputChannel<uint8_t> & {
        return inputChannel;
    }

    template<typename T>
    auto MessageDecoding<T>::getChannelOut() -> OutputChannel<T> & {
        return outputChannel;
    }
} // namespace filter

#endif // FLIGHTCOMPUTER_MESSAGEDECODING_HPP
