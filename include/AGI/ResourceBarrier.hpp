#pragma once

#include "agipch.hpp"

namespace AGI {

    class RefCounted
    {
    public:
        virtual ~RefCounted() = default;

        void IncRefCount() const
        {
            ++m_RefCount;
        }
        void DecRefCount() const
        {
            --m_RefCount;
        }

        uint32_t GetRefCount() const { return m_RefCount.load(); }

    private:
        mutable std::atomic<uint32_t> m_RefCount = 0;
    };

    template <typename T>
    class ResourceBarrier
    {
    public:
        static_assert(std::is_base_of<RefCounted, T>::value, "Class is not RefCounted!");

        ResourceBarrier()
            : m_Instance(nullptr)
        {
        }

        ResourceBarrier(std::nullptr_t n)
            : m_Instance(nullptr)
        {
        }

        ResourceBarrier(T *instance)
            : m_Instance(instance)
        {
            IncRef();
        }

        template <typename T2>
        ResourceBarrier(const ResourceBarrier<T2> &other)
        {
            m_Instance = (T *)other.m_Instance;
            IncRef();
        }

        template <typename T2>
        ResourceBarrier(ResourceBarrier<T2> &&other)
        {
            m_Instance = (T *)other.m_Instance;
            other.m_Instance = nullptr;
        }

        ~ResourceBarrier()
        {
            DecRef();
        }

        ResourceBarrier(const ResourceBarrier<T> &other)
            : m_Instance(other.m_Instance)
        {
            IncRef();
        }

        ResourceBarrier &operator=(std::nullptr_t)
        {
            DecRef();
            m_Instance = nullptr;
            return *this;
        }

        ResourceBarrier &operator=(const ResourceBarrier<T> &other)
        {
            if (this == &other)
                return *this;

            other.IncRef();
            DecRef();

            m_Instance = other.m_Instance;
            return *this;
        }

        template <typename T2>
        ResourceBarrier &operator=(const ResourceBarrier<T2> &other)
        {
            other.IncRef();
            DecRef();

            m_Instance = other.m_Instance;
            return *this;
        }

        template <typename T2>
        ResourceBarrier &operator=(ResourceBarrier<T2> &&other)
        {
            DecRef();

            m_Instance = other.m_Instance;
            other.m_Instance = nullptr;
            return *this;
        }

        operator bool() { return m_Instance != nullptr; }
        operator bool() const { return m_Instance != nullptr; }

        T *operator->() { return m_Instance; }
        const T *operator->() const { return m_Instance; }

        T &operator*() { return *m_Instance; }
        const T &operator*() const { return *m_Instance; }

        T *Raw() { return m_Instance; }
        const T *Raw() const { return m_Instance; }

        void Reset(T *instance = nullptr)
        {
            DecRef();
            m_Instance = instance;
            IncRef();
        }

        template <typename T2>
        ResourceBarrier<T2> As() const
        {
            return ResourceBarrier<T2>(*this);
        }

        template <typename... Args>
        static ResourceBarrier<T> Create(Args &&...args)
        {
            return ResourceBarrier<T>(new T(std::forward<Args>(args)...));
        }

        bool operator==(const ResourceBarrier<T> &other) const
        {
            return m_Instance == other.m_Instance;
        }

        bool operator!=(const ResourceBarrier<T> &other) const
        {
            return !(*this == other);
        }

    private:
        void IncRef() const
        {
            if (m_Instance)
            {
                m_Instance->IncRefCount();
            }
        }

        void DecRef() const
        {
            if (m_Instance)
            {
                m_Instance->DecRefCount();

                if (m_Instance->GetRefCount() == 0)
                {
                    delete m_Instance;
                    m_Instance = nullptr;
                }
            }
        }

        template <class T2>
        friend class ResourceBarrier;
        mutable T *m_Instance;
    };

}