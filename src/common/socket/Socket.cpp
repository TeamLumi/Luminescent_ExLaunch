#include "Socket.h"
#include "exlaunch.hpp"
#include "nifm.h"
#include "nn/socket.h"
#include "imgui.h"
#include "memory/string.h"

namespace {
    constexpr inline auto MaxSockets                       = 2;
    constexpr inline auto DefaultTcpAutoBufferSizeMax      = 192 * 1024; /* 192kb */
    constexpr inline auto MinTransferMemorySize            = (2 * DefaultTcpAutoBufferSizeMax + (128 * 1024));
    constexpr inline auto MinSocketAllocatorSize           = 128 * 1024;

    constexpr inline auto SocketAllocatorSize = MinSocketAllocatorSize * MaxSockets;
    constexpr inline auto TransferMemorySize = MinTransferMemorySize * MaxSockets;

    constexpr inline auto SocketPoolSize = SocketAllocatorSize + TransferMemorySize;
};

char socketPool[SocketPoolSize] __attribute__((aligned(0x4000)));

static bool initialized = false;

bool do_initializeNetwork() {
    if (initialized) return true;

    nn::nifm::Initialize();

    nn::nifm::SubmitNetworkRequest();

    while (nn::nifm::IsNetworkRequestOnHold()) { }

    if (!nn::nifm::IsNetworkAvailable()) {
        return false;
    }

    nn::socket::Config config;

    config.pool = socketPool;
    config.allocPoolSize = SocketAllocatorSize;
    config.poolSize = SocketPoolSize;
    config.concurLimit = 0xE;

    if (nn::socket::Initialize(config).isFailure()) {
        return false;
    }

    initialized = true;
    return true;
}

nn::Result Socket::init(const char *ip, u16 port) {
    in_addr hostAddress = { 0 };
    sockaddr serverAddress = { 0 };

    if (mState != SocketState::UNINITIALIZED) {
        return -1;
    }

    if (!do_initializeNetwork()) {
        mState = SocketState::UNAVAILABLE;
        return -1;
    }

    if ((mSocketFd = nn::socket::Socket(2, 1, 0)) < 0) {
        mState = SocketState::UNAVAILABLE;
        return -1;
    }

    nn::socket::InetAton(ip, &hostAddress);

    serverAddress.address = hostAddress;
    serverAddress.port = nn::socket::InetHtons(port);
    serverAddress.family = 2;

    nn::Result result = nn::socket::Connect(mSocketFd, &serverAddress, sizeof(serverAddress));

    mState = result.isSuccess() ? SocketState::CONNECTED : SocketState::DISCONNECTED;

    return result;
}

void Socket::close() {
    mState = SocketState::UNINITIALIZED;
}

void Socket::sendMessage(const char *message) {
    if (mState != SocketState::CONNECTED)
        return;

    size_t length = strlen(message);
    size_t offset = 0;
    while (offset < length) {
        size_t minSize = std::min(length - offset, (size_t)PACKET_MAX_SIZE);
        int sent = nn::socket::Send(mSocketFd, message + offset, minSize, 0);
        offset += sent;
    }
}

const char *Socket::receiveMessage() {
    if (mState != SocketState::CONNECTED)
        return nullptr;

    nn::string message;
    char buffer[PACKET_MAX_SIZE] = {};
    while (!message.ends_with('\n')) {
        if (nn::socket::Recv(mSocketFd, buffer, sizeof(buffer), 0) > 0) {
            message.append(buffer);
        } else {
            return nullptr;
        }
    }

    char* ptr = (char*)nn_malloc(message.length() * sizeof(char));
    strcpy(ptr, message.c_str());

    return ptr;
}
