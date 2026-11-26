#pragma once

#include "patcher_impl.hpp"
#include "armv8.hpp"
#include "../util/offsets.hpp"

namespace exl::patch {
    
    class StreamPatcher : public PatcherImpl {
        private:
        constexpr bool DoneNothing() const {
            return m_Start == m_Current;
        }

        inline void Flush() {
            /* Nothing to flush if there was nothing done. */
            if(DoneNothing())
                return;

            /* Get actual pointers. */
            void* ro = (void*) RoFromAddr(m_Start);
            void* rw = (void*) RwFromAddr(m_Start);
            auto size = m_Current - m_Start;

            /* Flush data/instructions. */
            armDCacheFlush(rw, size);
            armICacheInvalidate(ro, size);

            /* Reset start position. */
            m_Start += size;
        }

        protected:
        inline ptrdiff_t RelativeAddressFromBase(game::Location address) const {
            return util::offsets::Resolve(address) - m_Current;
        }
        inline ptrdiff_t RelativeAddressFromPointer(void* ptr) const {
            return AddrFromRoPointer(ptr) - m_Current;
        }
        
        uintptr_t m_Start;
        uintptr_t m_Current;

        public:
        inline StreamPatcher() : m_Start(0), m_Current(0) {}
        inline explicit StreamPatcher(game::Location start) : m_Start(util::offsets::Resolve(start)), m_Current(m_Start) {}

        template<typename T>
        inline void Write(T v) {
            At<T>(m_Current) = v;

            m_Current += sizeof(T);
        }

        /* Flush current data then move to a new address. */
        inline void SeekRel(uintptr_t address) {
            /* Don't need to do anything if the address doesn't need to change. */
            if(address == 0)
                return;

            /* Get address relative to base. */
            address += m_Current;

            /* Flush what we've already done. */
            Flush();

            /* Reset to the position to the provided address. */
            m_Start = address;
            m_Current = address;
        }

        /* Address relative to the base (Ro). */
        inline void Seek(game::Location address) {
            SeekRel(RelativeAddressFromBase(address));
        }
        /* Absolute address. */
        inline void Seek(void* ptr) {
            SeekRel(RelativeAddressFromPointer(ptr));
        }

        inline ~StreamPatcher() {
            Flush();
        }
    };
}
