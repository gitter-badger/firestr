/*
 * Copyright (C) 2014  Maxim Noah Khailo
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


#include "gui/contactselect.hpp"
#include "gui/util.hpp"

#include "util/log.hpp"
#include "util/dbc.hpp"

#include <QtWidgets>

namespace us = fire::user;
namespace u = fire::util;

namespace fire
{
    namespace gui
    {
        contact_select_widget::contact_select_widget(
                us::user_service_ptr s,
                contact_select_filter f
                ) : _user_service{s}, _filter{f}
        {
            REQUIRE(s);

            update_contacts();

            INVARIANT(_user_service);
        }

        void contact_select_widget::update_contacts()
        {
            INVARIANT(_user_service);

            clear();
            for(auto p : _user_service->user().contacts().list())
            {
                CHECK(p);
                if(!_filter(*p)) continue;

                //skip contact which is disconnected
                if(!_user_service->contact_available(p->id())) continue;
                addItem(p->name().c_str(), p->id().c_str());
            }

            bool enabled = count() > 0;

            setEnabled(enabled);
        }

        us::user_service_ptr contact_select_widget::user_service()
        {
            ENSURE(_user_service);
            return _user_service;
        }

        us::user_info_ptr contact_select_widget::selected_contact()
        {
            INVARIANT(_user_service);

            if(count() == 0) return {};

            size_t i = currentIndex();
            auto id = convert(itemData(i).toString());
            update_contacts();

            auto c = _user_service->user().contacts().by_id(id);

            if(!_user_service->contact_available(id)) return {};
            if(!c) return {};
            if(!_filter(*c)) return{};

            return c;
        }
    }
}

