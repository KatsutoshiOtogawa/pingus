//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/pingus_error.hpp"

#include "gettext.h"


PingusError::PingusError (const std::string& mes) 
  : message("PingusError: " + mes)
{
}

PingusError::~PingusError() throw()
{
}

void
PingusError::raise (const std::string& msg)
{
  throw PingusError(msg);
}

const std::string&
PingusError::get_message () const
{
  return message;
}

const char* 
PingusError::what() const throw()
{
  return message.c_str(); 
}


/* EOF */
