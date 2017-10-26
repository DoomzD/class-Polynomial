#include <iostream>
#include <vector>


template<typename T>
class Polynomial {
public:
    Polynomial(std::vector<T>& v) : v_(v) {}
    Polynomial(T num) : v_({num}) {}
    Polynomial() : v_({}) {}

    template<typename Iter>
    Polynomial(Iter begin, Iter end) {
        while (begin != end) {
            v_.push_back(*(begin++));
        }
    }

    bool operator == (const Polynomial<T>& other) const;
    bool operator != (const Polynomial<T>& other) const;

    template<typename T1>
    friend Polynomial<T1> operator + (const Polynomial<T1>& lhs, const Polynomial<T1>& rhs);

    template<typename T1>
    friend Polynomial<T1> operator - (const Polynomial<T1>& lhs, const Polynomial<T1>& rhs);

    template<typename T1>
    friend Polynomial<T1> operator & (const Polynomial<T1>& lhs, const Polynomial<T1>& rhs);

    template<typename T1>
    friend Polynomial<T1> operator * (const Polynomial<T1>& lhs, const Polynomial<T1>& rhs);

    template<typename T1>
    friend Polynomial<T1> operator / (const Polynomial<T1>& lhs, const Polynomial<T1>& rhs);

    template<typename T1>
    friend Polynomial<T1> operator % (const Polynomial<T1>& lhs, const Polynomial<T1>& rhs);

    Polynomial<T>& operator += (const Polynomial<T>& other);
    Polynomial<T>& operator -= (const Polynomial<T>& other);
    Polynomial<T>& operator *= (const Polynomial<T>& other);

    template<typename T1>
    friend std::ostream& operator << (std::ostream& out, const Polynomial<T1>& cur);

    const T operator[] (int pos) const {
        return (static_cast<int>(v_.size()) > pos ? v_[pos] : T(0));
    }

    int Degree() const {
        int sz = static_cast<int>(v_.size());
        while (sz > 0 && v_[sz - 1] == T(0))
            --sz;
        return (sz == 0 ? -1 : sz - 1);
    }

    T operator () (T num) const;

    typename std::vector<T>::const_iterator begin() const;

    typename std::vector<T>::const_iterator end() const;

    Polynomial<T> operator , (const Polynomial<T>& other) const;

private:
    std::vector<T> v_;
};

template<typename T>
T ab(T& arg) {
    if (arg < T(0))
        return arg * T(-1);
    return arg;
}

template<typename T>
bool Polynomial<T>::operator==(const Polynomial<T> &other) const {
    if (Degree() != other.Degree())
        return false;
    for (int i = 0; i <= Degree(); ++i) {
        if (v_[i] != other.v_[i])
            return false;
    }
    return true;
}

template<typename T>
bool Polynomial<T>::operator!=(const Polynomial<T> &other) const {
    return !(*this == other);
}

template<typename T>
Polynomial<T> operator+(const Polynomial<T> &lhs, const Polynomial<T> &rhs) {
    Polynomial<T> temp = lhs;
    return temp += rhs;
}

template<typename T>
Polynomial<T> operator-(const Polynomial<T> &lhs, const Polynomial<T> &rhs) {
    Polynomial<T> temp = lhs;
    return temp -= rhs;
}

template<typename T>
Polynomial<T> &Polynomial<T>::operator+=(const Polynomial<T> &other) {
    for (size_t i = 0; i < other.v_.size(); ++i) {
        if (v_.size() > i)
            v_[i] += other.v_[i];
        else
            v_.push_back(other.v_[i]);
    }
    return *this;
}

template<typename T>
Polynomial<T> &Polynomial<T>::operator-=(const Polynomial<T> &other) {
    for (size_t i = 0; i < other.v_.size(); ++i) {
        if (v_.size() > i) {
            v_[i] -= other.v_[i];
        } else {
            v_.push_back(other.v_[i]);
            v_[v_.size() - 1] *= T(-1);
        }
    }
    return *this;
}

template <typename T>
typename std::vector<T>::const_iterator Polynomial<T>::begin() const {
    return v_.begin();
}

template <typename T>
typename std::vector<T>::const_iterator Polynomial<T>::end() const {
    return (v_.begin() + Degree() + 1);
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const Polynomial<T> &cur) {
    if (cur.Degree() == -1) {
        out << 0;
        return out;
    }
    for (int i = cur.Degree(); i >= 0; --i) {
        if (cur.v_[i] == T(0))
            continue;
        if (i == 0) {
            if (i != cur.Degree()) {
                if (cur.v_[i] < T(0)) {
                    out << cur.v_[i];
                } else {
                    out << '+' << cur.v_[i];
                }
            } else {
                out << cur.v_[i];
            }
            return out;
        }
        if (i == cur.Degree()) {
            if (ab(cur.v_[i]) == T(1)) {
                if (cur.v_[i] < T(0)) {
                    if (i != 1) {
                        out << "-x^" << i;
                    } else {
                        out << "-x";
                    }
                } else {
                    if (i != 1) {
                        out << "x^" << i;
                    } else {
                        out << "x";
                    }
                }
            } else {
                if (i != 1) {
                    out << cur.v_[i] << "*x^" << i;
                } else {
                    out << cur.v_[i] << "*x";
                }
            }
        } else {
            if (ab(cur.v_[i]) == T(1)) {
                if (cur.v_[i] < T(0)) {
                    if (i != 1) {
                        out << "-x^" << i;
                    } else {
                        out << "-x";
                    }
                } else {
                    if (i != 1) {
                        out << '+' << "x^" << i;
                    } else {
                        out << '+' << "x";
                    }
                }
            } else {
                if (cur.v_[i] < T(0)) {
                    if (i != 1) {
                        out << cur.v_[i] << "*x^" << i;
                    } else {
                        out << cur.v_[i] << "*x";
                    }
                } else {
                    if (i != 1) {
                        out << '+' << cur.v_[i] << "*x^" << i;
                    } else {
                        out << '+' << cur.v_[i] << "*x";
                    }
                }
            }
        }
    }
    return out;
}

template<typename T>
Polynomial<T> operator * (const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    Polynomial<T> temp = lhs;
    return temp *= rhs;
}

template<typename T>
Polynomial<T> &Polynomial<T>::operator*=(const Polynomial<T> &other) {
    std::vector<T> result(other.Degree() + Degree() + 2, T(0));
    for (int i = 0; i <= Degree(); ++i) {
        for (int j = 0; j <= other.Degree(); ++j) {
            result[i + j] += v_[i] * other.v_[j];
        }
    }
    (*this).v_ = result;
    return *this;
}

template<typename T>
T Polynomial<T>::operator()(T num) const {
    T result = T(0);
    T curPow = T(1);
    for (int i = 0; i <= Degree(); ++i, curPow *= num) {
        result += v_[i] * curPow;
    }
    return result;
}

template<typename T1>
Polynomial<T1> operator&(const Polynomial<T1> &lhs, const Polynomial<T1> &rhs) {
    std::vector<T1> result(lhs.Degree() * rhs.Degree() + 1, T1(0));
    for (int i = 0; i <= lhs.Degree(); ++i) {
        int curDegree = i;
        Polynomial<T1> temp(T1(1));
        while (curDegree > 0) {
            temp *= rhs;
            --curDegree;
        }
        for (int j = 0; j <= temp.Degree(); ++j) {
            result[j] += lhs.v_[i] * temp.v_[j];
        }
    }
    return {result};
}

template<typename T1>
Polynomial<T1> operator/(const Polynomial<T1> &lhs, const Polynomial<T1> &rhs) {
    if (lhs.Degree() < rhs.Degree()) {
        return {T1(0)};
    }
    Polynomial<T1> temp = lhs;
    std::vector<T1> result(lhs.Degree() - rhs.Degree() + 1, T1(0));
    while (temp.Degree() >= rhs.Degree()) {
        T1 cff = temp.v_[temp.Degree()] / rhs.v_[rhs.Degree()];
        result[temp.Degree() - rhs.Degree()] += cff;
        std::vector<T1> curCff(temp.Degree() - rhs.Degree() + 1, T1(0));
        curCff[temp.Degree() - rhs.Degree()] += cff;
        temp -= (rhs * Polynomial<T1>(curCff));
    }
    return {result};
}

template<typename T1>
Polynomial<T1> operator%(const Polynomial<T1> &lhs, const Polynomial<T1> &rhs) {
    if (lhs.Degree() < rhs.Degree()) {
        return {T1(0)};
    }
    Polynomial<T1> temp = lhs;
    std::vector<T1> result(lhs.Degree() - rhs.Degree() + 1, T1(0));
    while (temp.Degree() >= rhs.Degree()) {
        T1 cff = temp.v_[temp.Degree()] / rhs.v_[rhs.Degree()];
        result[temp.Degree() - rhs.Degree()] += cff;
        std::vector<T1> curCff(temp.Degree() - rhs.Degree() + 1, T1(0));
        curCff[temp.Degree() - rhs.Degree()] += cff;
        temp -= (rhs * Polynomial<T1>(curCff));
    }
    return temp;
}

template<typename T>
Polynomial<T> Polynomial<T>::operator,(const Polynomial<T> &other) const {
    Polynomial<T> num;
    Polynomial<T> den;
    if (Degree() >= other.Degree()) {
        num = *this;
        den = other;
    } else {
        num = other;
        den = *this;
    }
    Polynomial<T> temp = num % den;
    while (temp != T(0)) {
        num = den;
        den = temp;
        temp = num % den;
    }
    return (den / Polynomial<T>(static_cast<T>(den[den.Degree()])));
    return den;
}
