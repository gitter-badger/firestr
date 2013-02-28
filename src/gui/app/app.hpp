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

#ifndef FIRESTR_GUI_APP_APP_H
#define FIRESTR_GUI_APP_APP_H

#include "message/message.hpp"

#include <string>
#include <memory>

namespace fire
{
    namespace gui
    {
        namespace app
        {
            struct app_metadata
            {
                std::string id;
                std::string name;
            };

            class app
            {
                public:
                    app();
                    app(const std::string& id);
                    app(const fire::message::message&);
                    operator fire::message::message();

                public:
                    const std::string& name() const;
                    const std::string& id() const;
                    const std::string& code() const;

                    void name(const std::string&);
                    void code(const std::string&);

                private:
                    app_metadata _meta;
                    std::string _code;
            };

            using app_ptr = std::shared_ptr<app>;
            using app_wptr = std::weak_ptr<app>;

            bool save_app(const std::string& dir, const app&);
            app_ptr load_app(const std::string& dir);
            bool load_app_metadata(const std::string& dir, app_metadata&);
        }
    }
}

#endif
