//  $Id: snow_generator_data.cxx,v 1.4 2002/09/28 11:52:27 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <iostream>
#include "../editor/weather_obj.hxx"
#include "../worldobjs/snow_generator.hxx"
#include "snow_generator_data.hxx"

namespace WorldObjsData {

void
SnowGeneratorData::write_xml(std::ostream& xml)
{
  xml << "<worldobj type=\"snow-generator\"/>" << std::endl;
}

WorldObj*
SnowGeneratorData::create_WorldObj ()
{
  return new WorldObjs::SnowGenerator();
}

EditorObjLst
SnowGeneratorData::create_EditorObj ()
{
  return EditorObjLst(1, new WeatherObj("snow-generator"));
}

} // namespace WorldObjsData

/* EOF */
