#pragma once

#include <utility>
struct IOError {};

template <class T, class E>
struct Result {

    enum class Tag : unsigned char {
        OK, ERR
    };

    union Data {
        T ok;
        E error;

        Data() {}
        ~Data() {}
    };

    Result(const Result& result) noexcept : _tag(result.tag()) {
        if (tag() == Tag::OK) {
            _data.ok = result._data.ok;
        } else if (tag() == Tag::ERR) {
            _data.error = result._data.error;
        }
    }

    Result(Result&& result) noexcept : _tag(result.tag()) {
        if (tag() == Tag::OK) {
            std::swap(_data.ok, result._data.ok);
        } else if (tag() == Tag::ERR) {
            std::swap(_data.error, result._data.error);
        }
    }

    ~Result() {
        if (tag() == Tag::OK) {
            _data.ok.~T();
        } else if (tag() == Tag::ERR) {
            _data.error.~E();
        }
    }

    bool isOk() const noexcept {
        return tag() == Tag::OK;
    }

    bool isErr() const noexcept {
        return tag() == Tag::ERR;
    }

    template<class F>
    void ifOk(F function) const noexcept {
        if (isOk()) {
            function(_data.ok);
        }
    }

    template<class F>
    void ifErr(F function) const noexcept {
        if (isErr()) {
            function(_data.error);
        }
    }

    template<class R>
    struct Match {
        const Result<T, E>& parent;
        template<class FOK, class FERR>
        R operator()(FOK onOk, FERR onErr) const noexcept {
            if (parent.isOk()) {
                return onOk(parent._data.ok);
            } else {
                return onErr(parent._data.error);
            }
        }
    };

    template<class R>
    Match<R> match() const noexcept {
        return Match<R> { *this };
    }

    static Result ok(const T& okData) noexcept {
        Result<T, E> result;
        result._tag = Tag::OK;
        new (&result._data.ok) T (okData);
        return result;
    }

    static Result ok(T&& okData) noexcept {
        Result<T, E> result;
        result._tag = Tag::OK;
        new (&result._data.ok) T (okData);
        return result;
    }

    static Result error(const E& errData) noexcept {
        Result<T, E> result;
        result._tag = Tag::ERR;
        new (&result._data.error) E (errData);
        return result;
    }

    static Result error(E&& errData) noexcept {
        Result<T, E> result;
        result._tag = Tag::ERR;
        new (&result._data.error) E (errData);
        return result;
    }

    Tag tag() const noexcept {
        return _tag;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"
    T unwrap() const {
        if (tag() == Tag::OK) {
            return _data.ok;
        } else if (tag() == Tag::ERR) {
            throw _data.error;
            // This is here just to get rid of compiler warnings
            return _data.ok;
        }
    }
#pragma clang diagnostic pop

private:
    Result() = default;
    Tag _tag;
    Data _data;

};
