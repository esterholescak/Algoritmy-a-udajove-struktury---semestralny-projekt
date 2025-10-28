#pragma once

#include <libds/amt/implicit_sequence.h>
#include <libds/adt/queue.h>
#include <libds/adt/array.h>
#include <functional>
#include <cmath>
#include <algorithm>
#include <type_traits>

namespace ds::adt
{
    template <typename T>
    struct Sort
    {
    public:
        virtual void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) = 0;
        void sort(amt::ImplicitSequence<T>& is) { sort(is, [](const T& a, const T& b)->bool {return a < b; }); }
    };

    //----------

    template <typename T>
    class SelectSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;
    };

    //----------

    template <typename T>
    class InsertSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;
    };

    //----------

    template <typename T>
    class BubbleSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;
    };

    //----------

    template <typename T>
    class QuickSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;

    private:
        void quick(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare, size_t min, size_t max);
    };

    //----------

    template <typename T>
    class HeapSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;
    };

    //----------

    template <typename T>
    class ShellSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;

    private:
        void shell(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare, size_t k);
    };

    //----------

    template <typename Key, typename T>
    class RadixSort :
        public Sort<T>
    {
        static_assert(std::is_integral_v<Key>, "Radix sort supports only integral types.");

    public:
        RadixSort();
        RadixSort(std::function<Key(const T&)> getKey);

        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;

    private:
        std::function<Key(const T&)> getKey_;
    };

    //----------

    template <typename T>
    class MergeSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;

    private:
        void split(size_t n);
        void merge(std::function<bool(const T&, const T&)> compare, size_t n);

    private:
        ImplicitQueue<T>* queue1_{ nullptr };
        ImplicitQueue<T>* queue2_{ nullptr };
        ImplicitQueue<T>* mergeQueue_{ nullptr };
    };

    //----------

    template<typename T>
    void SelectSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        for (size_t i = 0; i < is.size() - 1; ++i)
        {
            size_t min = i;
            for (size_t j = i + 1; j < is.size(); ++j)
            {
                if (compare(is.access(j)->data_, is.access(min)->data_))
                {
                    min = j;
                }
            }
            using std::swap;
            swap(is.access(i)->data_, is.access(min)->data_);
        }
    }

    template<typename T>
    void InsertSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        for (size_t i = 1; i < is.size(); ++i)
        {
            T tmp = is.access(i)->data_;
            size_t j = i;
            while (j > 0 && compare(tmp, is.access(j - 1)->data_))
            {
                is.access(j)->data_ = is.access(j - 1)->data_;
                --j;
            }
            is.access(j)->data_ = tmp;
        }
    }

    template<typename T>
    void BubbleSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        bool swapped;
        do
        {
            swapped = false;
            for (size_t i = 0; i < is.size() - 1; ++i)
            {
                if (compare(is.access(i + 1)->data_, is.access(i)->data_))
                {
                    using std::swap;
                    swap(is.access(i + 1)->data_, is.access(i)->data_);
                    swapped = true;
                }
            }
        } while (swapped);
    }

    template<typename T>
    void QuickSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        if (!is.isEmpty())
        {
            quick(is, compare, 0, is.size() - 1);
        }
    }

    template<typename T>
    void QuickSort<T>::quick(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare, size_t min, size_t max)
    {
        // TODO 12
        if (min >= max) {
            return;
        }

        T pivot = is.access(min + (max - min) / 2)->data_;
        long long left_ll = min;
        long long right_ll = max;

        while (left_ll <= right_ll) {
            while (compare(is.access(static_cast<size_t>(left_ll))->data_, pivot)) {
                left_ll++;
            }

            while (right_ll > 0 && compare(pivot, is.access(static_cast<size_t>(right_ll))->data_)) {
                right_ll--;
            }

            if (left_ll <= right_ll) {
                std::swap(is.access(static_cast<size_t>(left_ll))->data_, is.access(static_cast<size_t>(right_ll))->data_);
                left_ll++;
                if (right_ll > 0) {
                    right_ll--;
                }
            }
        }

        if ((long long)min < right_ll) {
            quick(is, compare, min, static_cast<size_t>(right_ll));
        }

        if (left_ll < (long long)max) {
            quick(is, compare, static_cast<size_t>(left_ll), max);
        }
    }

    template<typename T>
    void HeapSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        // TODO 12
        for (size_t i = 1; i < is.size(); ++i) {
            bool swapped;
            size_t current = i;
            do {
                swapped = false;
                size_t parent = (current - 1) / 2;
                if (current > 0 && compare(is.access(parent)->data_, is.access(current)->data_)) {
                    std::swap(is.access(current)->data_, is.access(parent)->data_);
                    current = parent;
                    swapped = true;
                }
            } while (swapped);
        }

        for (size_t i = is.size() - 1; i > 0; --i) {
            std::swap(is.access(0)->data_, is.access(i)->data_);

            bool swapped;
            size_t current = 0;
            do {
                swapped = false;
                size_t leftChild = 2 * current + 1;
                size_t rightChild = 2 * current + 2;
                size_t maxChildIndex;

                if (leftChild < i && rightChild < i) {
                    if (compare(is.access(rightChild)->data_, is.access(leftChild)->data_)) {
                        maxChildIndex = leftChild;
                    }
                    else {
                        maxChildIndex = rightChild;
                    }
                }
                else {
                    if (leftChild < i) {
                        maxChildIndex = leftChild;
                    }
                    else {
                        maxChildIndex = current;
                    }
                }

                if (maxChildIndex != current && maxChildIndex < i && compare(is.access(current)->data_, is.access(maxChildIndex)->data_)) {
                    std::swap(is.access(current)->data_, is.access(maxChildIndex)->data_);
                    current = maxChildIndex;
                    swapped = true;
                }
            } while (swapped);
        }
    }

    template<typename T>
    void ShellSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        shell(is, compare, static_cast<size_t>(std::ceil(std::log10(is.size()))));
    }

    template<typename T>
    void ShellSort<T>::shell(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare, size_t k)
    {
        // TODO 12
        if (k == 0) {
            return;
        }

        for (size_t d = 0; d < k; ++d) {
            for (size_t i = d; i < is.size(); ++i) {
                size_t j = i;
                while (j >= k && (j - k >= d) && compare(is.access(j)->data_, is.access(j - k)->data_)) {
                    std::swap(is.access(j)->data_, is.access(j - k)->data_);
                    j -= k;
                }
            }
        }

        if (k > 1) {
            shell(is, compare, k - 1);
        }
    }

    template<typename Key, typename T>
    RadixSort<Key, T>::RadixSort() :
        getKey_([](auto const& x) { return x; })
    {
    }

    template<typename Key, typename T>
    RadixSort<Key, T>::RadixSort(std::function<Key(const T&)> getKey) :
        getKey_(getKey)
    {
    }

    template<typename Key, typename T>
    void RadixSort<Key, T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        // TODO 12
        if (is.isEmpty()) {
            return;
        }

        ds::adt::Array<ImplicitQueue<T>*> buckets(10);

        for (size_t i = 0; i < 10; ++i) {
            buckets.set(new ImplicitQueue<T>(), i);
        }

        long long component = 1;
        bool existsNextComponent = true;

        while (existsNextComponent) {
            existsNextComponent = false;

            for (size_t i = 0; i < is.size(); ++i) {
                T element = is.access(i)->data_;
                Key key = getKey_(element);
                size_t bucketIndex = (key / component) % 10;
                buckets.access(bucketIndex)->push(element);

                if (key / (component * 10) > 0) {
                    existsNextComponent = true;
                }
            }
            component *= 10;

            size_t index = 0;
            for (size_t i = 0; i < 10; ++i) {
                ImplicitQueue<T>* bucket = buckets.access(i);
                while (!bucket->isEmpty()) {
                    is.access(index)->data_ = bucket->pop();
                    index++;
                }
            }
        }

        for (size_t i = 0; i < 10; ++i) {
            delete buckets.access(i);
            buckets.set(nullptr, i);
        }
    }

    template<typename T>
    void MergeSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        // TODO 12
        queue1_ = new ImplicitQueue<T>(is.size());
        queue2_ = new ImplicitQueue<T>(is.size());
        mergeQueue_ = new ImplicitQueue<T>(is.size());

        for (size_t k = 0; k < is.size(); ++k) {
            mergeQueue_->push(is.access(k)->data_);
        }

        size_t i = 1;
        while (i < is.size()) {
            split(i);
            merge(compare, i);
            i *= 2;
        }

        split(i);
        merge(compare, i);

        for (size_t k = 0; k < is.size(); ++k) {
            is.access(k)->data_ = mergeQueue_->pop();
        }

        delete queue1_;
        delete queue2_;
        delete mergeQueue_;
        queue1_ = nullptr;
        queue2_ = nullptr;
        mergeQueue_ = nullptr;
    }

    template<typename T>
    void MergeSort<T>::split(size_t n)
    {
        // TODO 12
        queue1_->clear();
        queue2_->clear();

        size_t count = 0;
        bool isFirstSegment = true;

        while (!mergeQueue_->isEmpty()) {
            if (count % n == 0 && count != 0) {
                count = 0;
                isFirstSegment = !isFirstSegment;
            }

            if (isFirstSegment) {
                queue1_->push(mergeQueue_->pop());
            }
            else {
                queue2_->push(mergeQueue_->pop());
            }
            count++;
        }
    }

    template<typename T>
    void MergeSort<T>::merge(std::function<bool(const T&, const T&)> compare, size_t n)
    {
        // TODO 12
        mergeQueue_->clear();

        long long count1 = 0;
        long long count2 = 0;

        while (!queue1_->isEmpty() || !queue2_->isEmpty()) {
            if (count1 == 0 && count2 == 0) {
                count1 = std::min(n, queue1_->size());
                count2 = std::min(n, queue2_->size());
            }

            T* key1 = count1 > 0 ? &queue1_->peek() : nullptr;
            T* key2 = count2 > 0 ? &queue2_->peek() : nullptr;

            if (key1 != nullptr && key2 != nullptr) {
                if (compare(*key1, *key2)) {
                    count1--;
                    mergeQueue_->push(queue1_->pop());
                }
                else {
                    count2--;
                    mergeQueue_->push(queue2_->pop());
                }
            }
            else {
                if (key1 != nullptr) {
                    count1--;
                    mergeQueue_->push(queue1_->pop());
                }
                else if (key2 != nullptr) {
                    count2--;
                    mergeQueue_->push(queue2_->pop());
                }
                else {
                    break;
                }
            }
        }
    }
}