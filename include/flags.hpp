/** @file flags.hpp

@author jim
@date 26/10/2014

The MIT License (MIT)

Copyright © 2014 jim

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef FLAGS_HPP
#define FLAGS_HPP

class Flag
{
    public:
        explicit constexpr inline Flag(int t_i) : m_i(t_i)
        {}
        constexpr inline operator int() const { return m_i; }

    private:
        int m_i {};
};

template<typename Enum>
class Flags
{
    public:
        typedef Enum enum_type;

        explicit constexpr inline Flags(Enum t_f) : m_i(t_f) {}
        explicit constexpr inline Flags(Flag t_f) : m_i(t_f) {}
        inline Flags &operator&=(int t_mask) { m_i &= t_mask; return *this; }
        inline Flags &operator&=(unsigned int t_mask) { m_i &= t_mask; return *this; }
        inline Flags &operator|=(Flags t_f) { m_i |= t_f.mI; return *this; }
        inline Flags &operator|=(Enum t_f) { m_i |= t_f; return *this; }
        inline Flags &operator^=(Flags t_f) { m_i ^= t_f.i; return *this; }
        inline Flags &operator^=(Enum t_f) { m_i ^= t_f; return *this; }

        constexpr inline operator int() const { return m_i; }

        constexpr inline Flags operator|(Flags t_f) const
        { return Flags(Enum(m_i | t_f.i)); }
        constexpr inline Flags operator|(Enum t_f) const
        { return Flags(Enum(m_i | t_f)); }
        constexpr inline Flags operator^(Flags t_f) const
        { return Flags(Enum(m_i ^ t_f.i)); }
        constexpr inline Flags operator^(Enum t_f) const
        { return Flags(Enum(m_i ^ t_f)); }
        constexpr inline Flags operator&(int t_mask) const
        { return Flags(Enum(m_i & t_mask)); }
        constexpr inline Flags operator&(unsigned int t_mask) const
        { return Flags(Enum(m_i & t_mask)); }
        constexpr inline Flags operator&(Enum t_f) const
        { return Flags(Enum(m_i & t_f)); }
        constexpr inline Flags operator~() const
        { return Flags(Enum(~m_i)); }
        constexpr inline bool operator!() const
        { return !m_i; }
        constexpr inline bool testFlag(Enum t_f) const
        { return (m_i & t_f) == t_f && (t_f != 0 || mI == int(t_f) ); }

    private:
        int m_i {};
};

#define SFTD_DECLARE_OPERATORS_FOR_FLAGS(t_flags) \
constexpr inline Flags<t_flags::enum_type>  \
operator|(t_flags::enum_type f1, t_flags::enum_type f2) \
{ return Flags<t_flags::enum_type>(f1) | f2; } \
constexpr inline Flags<t_flags::enum_type> \
operator|(t_flags::enum_type f1, Flags<t_flags::enum_type> f2) \
{ return f2 | f1; }

#endif // FLAGS_HPP
