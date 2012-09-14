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


#include "gui/session.hpp"
#include "gui/unknown_message.hpp"
#include "gui/util.hpp"

#include <QtGui>

namespace s = fire::session;
namespace m = fire::message;
namespace ms = fire::messages;
namespace us = fire::user;
namespace a = fire::gui::app;

namespace fire
{
    namespace gui
    {
        namespace 
        {
            const size_t TIMER_SLEEP = 200;//in milliseconds
            const size_t CW_WIDTH = 50;//in milliseconds
            const size_t ADD_CONTACT_WIDTH = 10;
        }

        session_widget::session_widget(
                s::session_service_ptr session_service,
                s::session_ptr session,
                a::app_service_ptr app_service) :
            _session_service{session_service},
            _session{session},
            _app_service{app_service},
            _messages{new message_list{app_service, session}}
        {
            REQUIRE(session_service);
            REQUIRE(session);
            REQUIRE(app_service);

            _layout = new QGridLayout;

            _contact_select = new QComboBox;
            _add_contact = new QPushButton{"+"};
            connect(_add_contact, SIGNAL(clicked()), this, SLOT(add_contact()));

            update_contact_select();

            _contacts = new contact_list{_session_service->user_service(), _session->contacts()};

            auto* cw = new QWidget;
            auto* cl = new QGridLayout;
            cw->setLayout(cl);
            cl->addWidget(_contact_select, 0,0);
            cl->addWidget(_add_contact, 0, 1);
            cl->addWidget(_contacts, 1, 0, 1, 2);
            cw->resize(CW_WIDTH, cw->height());

            _splitter = new QSplitter{Qt::Horizontal};
            _splitter->addWidget(_messages);
            _splitter->addWidget(cw);
            _splitter->setStretchFactor(0, 1);
            _splitter->setStretchFactor(1, 0);
           
            _layout->addWidget(_splitter);

            setLayout(_layout);
            _layout->setContentsMargins(0,0,0,0);

            //setup mail timer
            auto *t2 = new QTimer(this);
            connect(t2, SIGNAL(timeout()), this, SLOT(check_mail()));
            t2->start(TIMER_SLEEP);

            INVARIANT(_session_service);
            INVARIANT(_session);
            INVARIANT(_messages);
            INVARIANT(_layout);
            INVARIANT(_app_service);
        }
        
        void session_widget::update_contact_select()
        {
            INVARIANT(_contact_select);
            INVARIANT(_session_service);
            INVARIANT(_session);
            INVARIANT(_add_contact);
            INVARIANT(_session_service->user_service());

            _contact_select->clear();
            for(auto p : _session_service->user_service()->user().contacts().list())
            {
                CHECK(p);
                //skip contact already in session
                if(_session->contacts().by_id(p->id())) continue;

                //skip contact which is disconnected
                if(!_session_service->user_service()->contact_available(p->id())) continue;
                _contact_select->addItem(p->name().c_str(), p->id().c_str());
            }

            bool enabled = _contact_select->count() > 0;

            _contact_select->setEnabled(enabled);
            _add_contact->setEnabled(enabled);
        }

        void session_widget::add(message* m)
        {
            REQUIRE(m);
            INVARIANT(_messages);
            _messages->add(m);
        }

        void session_widget::add(QWidget* w)
        {
            REQUIRE(w);
            INVARIANT(_messages);
            _messages->add(w);
        }

        s::session_ptr session_widget::session()
        {
            ENSURE(_session);
            return _session;
        }

        QWidget* contact_alert(us::user_info_ptr c, const std::string message)
        {
            REQUIRE(c);

            auto w = new QWidget;
            auto l = new QHBoxLayout;
            w->setLayout(l);

            std::stringstream s;
            s << "<b>" << c->name() << "</b> " << message;

            auto t = new QLabel{s.str().c_str()};
            l->addWidget(t);

            ENSURE(w);
            return w;
        }

        void session_widget::add_contact()
        {
            INVARIANT(_contact_select);
            INVARIANT(_session_service);
            INVARIANT(_session);
            INVARIANT(_contacts);

            size_t i = _contact_select->currentIndex();
            auto id = convert(_contact_select->itemData(i).toString());

            auto contact = _session_service->user_service()->user().contacts().by_id(id);
            if(!contact) return;

            _session_service->add_contact_to_session(contact, _session);
            _contacts->add_contact(contact);

            update_contact_select();
            add(contact_alert(contact, "added to session"));
        }

        void session_widget::update_contacts()
        {
            _contacts->update(_session->contacts());
            update_contact_select();
        }

        void session_widget::name(const QString& s)
        {
            _name = s;
        }

        QString session_widget::name() const
        {
            return _name;
        }

        void session_widget::check_mail()
        try
        {
            INVARIANT(_messages);
            INVARIANT(_session);
            INVARIANT(_session->mail());

            m::message m;
            while(_session->mail()->pop_inbox(m))
            {
                //for now show encoded message
                //TODO: use factory class to create gui from messages
                if(m.meta.type == ms::NEW_APP)
                {
                    _messages->add_new_app(m);
                }
                else if(m.meta.type == us::event::CONTACT_CONNECTED)
                {
                    update_contacts();
                }
                else if(m.meta.type == us::event::CONTACT_DISCONNECTED)
                {
                    update_contacts();
                }
                else
                {
                    std::stringstream s;
                    s << m;

                    _messages->add(new unknown_message{s.str()});
                }
            }
        }
        catch(std::exception& e)
        {
            std::cerr << "session: error in check_mail. " << e.what() << std::endl;
        }
        catch(...)
        {
            std::cerr << "session: unexpected error in check_mail." << std::endl;
        }
    }
}

