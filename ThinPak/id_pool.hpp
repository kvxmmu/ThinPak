//
// Created by kvxmmu on 8/24/20.
//

#ifndef THINPAK_ID_POOL_HPP
#define THINPAK_ID_POOL_HPP

#include <deque>

#ifdef IDPOOL_THREADSAFE
# include <mutex>
#endif

template <typename IntegerType,
        IntegerType initial_value = 0>
class IdPool {
public:
    IntegerType last_id = initial_value;
    std::deque<IntegerType> ids;

#ifdef IDPOOL_THREADSAFE
    std::mutex *mtx = nullptr;

    IdPool<IntegerType, initial_value>() {
        this->mtx = new std::mutex;
    }
#endif

    IntegerType get_id() {
        if (!ids.empty()) {
#ifdef IDPOOL_THREADSAFE
            this->mtx->lock();
#endif
            IntegerType first = ids.front();
            ids.pop_front();
#ifdef IDPOOL_THREADSAFE
            this->mtx->unlock();
#endif

            return first;
        }

        return last_id++;
    }

    void remove_id(size_t id) {
#ifdef IDPOOL_THREADSAFE
        this->mtx->lock();
#endif
        this->ids.push_back(id);
#ifdef IDPOOL_THREADSAFE
        this->mtx->unlock();
#endif
    }

#ifdef IDPOOL_THREADSAFE
    ~IdPool<IntegerType, initial_value>() {
        delete this->mtx;
    }
#endif
};

#endif //THINPAK_ID_POOL_HPP
