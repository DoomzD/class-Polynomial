#include <vector>
#include <iostream>

template<typename T>
class Polynomial {
public:
    Polynomial() = default;

    Polynomial(T num)
            : Data({num}) {}

    explicit Polynomial(const std::vector<T> &data)
            : Data(data) {}

    explicit Polynomial(const std::initializer_list<T> &data)
            : Data(data) {}

    template<typename Iter>
    Polynomial(Iter begin, Iter end) {
        while (begin != end) {
            Data.push_back(*(begin++));
        }
    }

    int Degree() const {
        int sz = static_cast<int>(Data.size());
        while (sz > 0 && Data[sz - 1] == T{0})
            --sz;
        return sz - 1;
    }

    const T operator[] (int pos) const {
        return (static_cast<int>(Data.size()) > pos ? Data[pos] : T{0});
    }

    friend bool operator==(const Polynomial& lhs, const Polynomial& rhs) {
        if (lhs.Degree() != rhs.Degree())
            return false;
        for (int i = 0; i <= lhs.Degree(); ++i) {
            if (lhs.Data[i] != rhs.Data[i])
                return false;
        }
        return true;
    }

    friend bool operator!=(const Polynomial& lhs, const Polynomial& rhs) {
        return !(lhs == rhs);
    }

    Polynomial<T>& operator += (const Polynomial<T>& other) {
        for (std::size_t i = 0; i < other.Data.size(); ++i) {
            if (Data.size() > i) {
                Data[i] += other.Data[i];
            } else {
                Data.push_back(other.Data[i]);
            }
        }
        return *this;
    }

    Polynomial<T>& operator -= (const Polynomial<T>& other) {
        for (std::size_t i = 0; i < other.Data.size(); ++i) {
            if (Data.size() > i) {
                Data[i] -= other.Data[i];
            } else {
                Data.push_back(T {-1} * other.Data[i]);
            }
        }
        return *this;
    }

    Polynomial<T>& operator *= (const Polynomial<T>& other) {
        std::vector<T> result(Degree() + other.Degree() + 2, T{0});

        for (int i = 0; i <= Degree(); ++i) {
            for (int j = 0; j <= other.Degree(); ++j) {
                result[i + j] += Data[i] * other.Data[j];
            }
        }

        Data = result;
        return *this;
    }

    friend Polynomial operator + (Polynomial lhs, const Polynomial& rhs) {
        lhs += rhs;
        return lhs;
    }
    friend Polynomial operator - (Polynomial lhs, const Polynomial& rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend Polynomial operator * (Polynomial lhs, const Polynomial& rhs) {
        lhs *= rhs;
        return lhs;
    }
    friend Polynomial operator & (const Polynomial& lhs, const Polynomial& rhs) {
        std::vector<T> result(lhs.Degree() * rhs.Degree() + 1, T{0});

        for (int i = 0; i <= lhs.Degree(); ++i) {
            int power = i;
            Polynomial temp{T{1}};

            while (power--) {
                temp *= rhs;
            }

            for (int j = 0; j <= temp.Degree(); ++j) {
                result[j] += temp[j] * lhs[i];
            }
        }

        return Polynomial{result};
    }
    friend Polynomial operator / (const Polynomial& lhs, const Polynomial& rhs) {
        Polynomial ans = Divide(lhs, rhs).first;
        return ans;
    }
    friend Polynomial operator % (const Polynomial& lhs, const Polynomial& rhs) {
        Polynomial ans = Divide(lhs, rhs).second;
        return ans;
    }
    friend Polynomial operator , (const Polynomial<T>& lhs, const Polynomial& rhs) {
        if (rhs == T{0}) {
            std::vector<T> highesCoeff(1, T{0});
            highesCoeff[0] = lhs[lhs.Degree()];
            return lhs / Polynomial(highesCoeff);
        }
        return rhs, (lhs % rhs);
    }

    T operator () (T num) const {
        T result = T{0};
        T power = T{1};

        for (int i = 0; i <= Degree(); ++i, power *= num) {
            result += Data[i] * power;
        }

        return result;
    }

    typename std::vector<T>::const_iterator begin() const {
        return Data.cbegin();
    }

    typename std::vector<T>::const_iterator end() const {
        return Data.cbegin() + Degree() + 1;
    }

    template<typename U>
    friend std::ostream& operator << (std::ostream& out, const Polynomial<U>& data);

private:
    friend std::pair<Polynomial<T>, Polynomial<T>>
    Divide(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
        if (lhs.Degree() < rhs.Degree())
            return {T{0}, lhs};

        Polynomial<T> temp{lhs.Data};
        std::vector<T> result(lhs.Degree() - rhs.Degree() + 1, T{0});

        while (temp.Degree() >= rhs.Degree()) {
            T coeff = temp[temp.Degree()] / rhs[rhs.Degree()];
            result[temp.Degree() - rhs.Degree()] += coeff;

            std::vector<T> subtract(temp.Degree() - rhs.Degree() + 1, T{0});
            subtract[temp.Degree() - rhs.Degree()] += coeff;
            temp -= Polynomial {subtract} * rhs;
        }

        return {Polynomial{result}, temp};
    }
    T AbsoluteValue(const T& elem) const {
        return elem < T{0} ? elem * T{-1} : elem;
    }
    std::string Sign(int pos, int deg) const {
        if (Data[pos] < T{0}) {
            return "-";
        } else {
            if (pos != deg)
                return "+";
            return "";
        }
    }
    std::ostream& Coeff(int pos, std::ostream& out) const {
        if (Data[pos] == T{1} || Data[pos] == T{-1}) {
            if (pos == 0)
                return out << "1";
            return out << "";
        } else {
            if (pos == 0)
                return out << AbsoluteValue(Data[pos]);
            out << AbsoluteValue(Data[pos]);
            return out << "*";
        }
    }
    std::string Var(int pos) const {
        if (pos != 0)
            return "x";
        return "";
    }
    std::string Power(int pos) const {
        if (pos > 1)
            return "^" + std::to_string(pos);
        return "";
    }

    std::vector<T> Data;
};

template<typename T>
std::ostream& operator << (std::ostream& out, const Polynomial<T>& data) {
    int deg = data.Degree();
    if (deg == -1) {
        out << 0;
        return out;
    }

    for (int i = deg; i >= 0; --i) {
        if (data[i] == T{0})
            continue;

        std::string currentValue;
        out << data.Sign(i, deg);
        data.Coeff(i, out);
        out << data.Var(i);
        out << data.Power(i);
    }

    return out;
}
