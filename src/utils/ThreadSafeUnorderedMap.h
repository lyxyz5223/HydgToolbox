#include <unordered_map>
#include <mutex>

#define THREAD_SAFE_CLASS_FUNCTION(funcName, sig, call) \
    auto safe_##funcName##sig             \
    {                                             \
        std::lock_guard<std::mutex> lock(mutex_); \
        return map_.funcName##call;        \
    }

#define THREAD_SAFE_CLASS_CONST_FUNCTION(funcName, sig, call) \
    auto safe_##funcName##sig const             \
    {                                                   \
        std::lock_guard<std::mutex> lock(mutex_);       \
        return map_.funcName##call;              \
    }

template <typename K, typename V>
class ThreadSafeUnorderedMap : public std::unordered_map<K, V>
{

public:
    typedef std::unordered_map<K, V> UMapType;
    typedef typename UMapType::iterator Iterator;
    typedef typename UMapType::const_iterator ConstIterator;

private:
    UMapType map_;
    std::mutex mutex_;

public:
    // 手动加锁
    void lock() {
        mutex_.lock();
    }
    // 手动解锁
    void unlock() {
        mutex_.unlock();
    }

    auto operator[](const K &key) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return map_[key];
    }
    THREAD_SAFE_CLASS_FUNCTION(insert, (const std::pair<K, V> &value), (value))
    THREAD_SAFE_CLASS_FUNCTION(insert, (K &&key, V &&value), (std::move(key), std::move(value)))
    THREAD_SAFE_CLASS_FUNCTION(find, (const K &key), (key))
    THREAD_SAFE_CLASS_FUNCTION(erase, (const K &key), (key))
    THREAD_SAFE_CLASS_FUNCTION(erase, (ConstIterator &it), (it))
    THREAD_SAFE_CLASS_FUNCTION(erase, (Iterator &it), (it))
    THREAD_SAFE_CLASS_FUNCTION(clear, (), ())
    THREAD_SAFE_CLASS_CONST_FUNCTION(size, (), ())
    THREAD_SAFE_CLASS_CONST_FUNCTION(empty, (), ())
    THREAD_SAFE_CLASS_CONST_FUNCTION(count, (), ())
};


// 取消宏定义，防止在其他地方误用
#undef THREAD_SAFE_CLASS_FUNCTION
#undef THREAD_SAFE_CLASS_CONST_FUNCTION