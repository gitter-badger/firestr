/*
 * Copyright (C) 2012  Maxim Noah Khailo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FIRESTR_UIL_MENCODE_H
#define FIRESTR_UIL_MENCODE_H

#include <map>
#include <iostream>
#include <sstream>
#include <initializer_list>

#include <boost/any.hpp>

#include "util/bytes.hpp"
#include "util/dbc.hpp"

namespace fire
{
    namespace util
    {
        class dict;
        class array;

        class value
        {
            public:
                value();
                value(int v);
                value(size_t v);
                value(double v);
                value(const std::string& v);
                value(const char* v);
                value(const bytes& v);
                value(const dict& v);
                value(const array& v);
                value(const value& o);

            public:
                operator int() const;
                operator size_t() const;
                operator double() const;
                operator std::string() const;
                operator bytes() const;
                operator dict() const;
                operator array() const;

            public:
                value& operator=(int v);
                value& operator=(size_t v);
                value& operator=(double v);
                value& operator=(const std::string& v);
                value& operator=(const char* v);
                value& operator=(const bytes& v);
                value& operator=(const dict& v);
                value& operator=(const array& v);
                value& operator=(const value& o);

            public:
                int as_int() const;
                size_t as_size() const;
                double as_double() const;
                std::string as_string() const;
                const bytes& as_bytes() const;
                const dict& as_dict() const;
                const array& as_array() const;
                
            public:
                bool is_int() const;
                bool is_size() const;
                bool is_double() const;
                bool is_bytes() const;
                bool is_dict() const;
                bool is_array() const;

            private:
                boost::any _v;
        };

        typedef std::pair<std::string, value> kv;

        class dict
        {
            private:
                typedef std::map<std::string, value> value_map;
        
            public:
                dict();
                dict(std::initializer_list<kv>);

            public:
                typedef value_map::const_iterator const_iterator;
                typedef value_map::iterator iterator;
                
            public:
                value& operator[](const std::string& k);
                const value& operator[] (const std::string& k) const;

                size_t size() const;
                bool has(const std::string& k) const; 

            public:
                const_iterator begin() const;
                const_iterator end() const;
                iterator begin();
                iterator end();

            private:
                value_map _m;
        };

        class array
        {
            private:
                typedef std::vector<value> value_array;

            public:
                typedef value_array::const_iterator const_iterator;
                typedef value_array::iterator iterator;

            public:
                array();
                array(std::initializer_list<value>);

            public:
                void add(const value&);

            public:
                value& operator[](size_t);
                const value& operator[] (size_t) const;
                size_t size() const;

            public:
                const_iterator begin() const;
                const_iterator end() const;
                iterator begin();
                iterator end();

            private:
                value_array _a;
        };

        std::ostream& operator<<(std::ostream&, const dict&);
        std::ostream& operator<<(std::ostream&, const array&);
        std::ostream& operator<<(std::ostream&, const value&);

        std::istream& operator>>(std::istream&, dict&);
        std::istream& operator>>(std::istream&, array&);
        std::istream& operator>>(std::istream&, value&);

        template <typename type> 
            bytes encode(const type& v)
            {
                std::stringstream s;
                s << v;
                return to_bytes(s.str());
            }

        template <typename type> 
            void decode(const bytes& b, type& v)
            {
                std::stringstream s{to_str(b)};
                s >> v;
            }

        template <typename type> 
            type decode(const bytes& b)
            {
                type v;
                std::stringstream s{to_str(b)};
                s >> v;
                return v;
            }
    }
}

namespace std
{
    std::ostream& operator<<(std::ostream&, const fire::util::bytes&);
    std::istream& operator>>(std::istream&, fire::util::bytes&);
}

#endif
