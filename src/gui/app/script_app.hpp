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

#ifndef FIRESTR_APP_SCRIPT_APP_H
#define FIRESTR_APP_SCRIPT_APP_H

#include "gui/app/app.hpp"
#include "gui/app/app_service.hpp"
#include "gui/list.hpp"
#include "gui/lua/api.hpp"
#include "gui/mail_service.hpp"
#include "gui/message.hpp"

#include "conversation/conversation_service.hpp"

#include "message/mailbox.hpp"
#include "messages/sender.hpp"

#include <QObject>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSignalMapper>

#include <string>

namespace fire
{
    namespace gui
    {
        namespace app
        {
            class script_app : public message
            {
                Q_OBJECT

                public:
                    script_app(
                            app_ptr, 
                            app_service_ptr, 
                            conversation::conversation_service_ptr, 
                            conversation::conversation_ptr);
                    script_app(
                            const std::string& from_id, 
                            const std::string& id, 
                            app_ptr, 
                            app_service_ptr, 
                            conversation::conversation_service_ptr, 
                            conversation::conversation_ptr);
                    ~script_app();

                public:
                    const std::string& id() const;
                    const std::string& type() const;
                    fire::message::mailbox_ptr mail();

                public slots:
                    void check_mail(fire::message::message);
                    void clone_app();

                private:
                    void init();

                private:
                    std::string _from_id;
                    std::string _id;
                    conversation::conversation_service_ptr _conversation_service;
                    conversation::conversation_ptr _conversation;
                    fire::message::mailbox_ptr _mail;
                    messages::sender_ptr _sender;
                    user::contact_list _contacts;

                    app_ptr _app;
                    app_service_ptr _app_service;

                private:
                    mail_service* _mail_service;
                    lua::lua_api_ptr _api;
                    QWidget* _canvas;
                    QGridLayout* _canvas_layout;
                    QPushButton* _clone;
            };

            extern const std::string SCRIPT_APP;

        }
    }
}

#endif

