# syntax=docker/dockerfile:1.7

FROM devkitpro/devkita64:20260219 AS builder

ENV DEBIAN_FRONTEND=noninteractive \
    CCACHE_DIR=/root/.cache/ccache \
    CCACHE_COMPRESS=1 \
    CCACHE_MAXSIZE=1G

RUN apt-get update -y \
    && apt-get install -y --no-install-recommends ccache \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY . .

RUN cmake -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain.cmake \
    -DCMAKE_CXX_FLAGS="-fpermissive -fPIC" \
    -DCMAKE_C_COMPILER_LAUNCHER=ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -Bbuild

RUN --mount=type=cache,target=/root/.cache/ccache \
    cmake --build build --target RomBase_release_all --parallel "$(nproc)"

FROM scratch AS artifacts

COPY --from=builder /src/build/RomBase_releases/atmosphere/ /atmosphere/
COPY --from=builder /src/build/RomBase_releases/modmanager/ /modmanager/
COPY --from=builder /src/build/RomBase_releases/ryujinx/ /ryujinx/
COPY --from=builder /src/build/RomBase_releases/yuzu/ /yuzu/