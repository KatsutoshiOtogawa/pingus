//  $Id: info_box_obj.hxx,v 1.5 2002/09/28 11:52:25 torangan Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_EDITOROBJS_INFO_BOX_OBJ_HXX
#define HEADER_PINGUS_EDITOROBJS_INFO_BOX_OBJ_HXX

#include "../editor/sprite_editorobj.hxx"

namespace WorldObjsData {
class InfoBoxData;
}

namespace EditorObjs {

class InfoBoxObj : public SpriteEditorObj
{
private:
  WorldObjsData::InfoBoxData* const data;

public:
  InfoBoxObj (const WorldObjsData::InfoBoxData& data_);
 ~InfoBoxObj ();

  static EditorObjLst create (const Vector& pos);

  void write_xml (std::ostream& xml);
  EditorObj* duplicate ();
  std::string status_line ();
  
private:
  InfoBoxObj (const InfoBoxObj&);
  InfoBoxObj& operator= (const InfoBoxObj&);
};

} // namespace EditorObjs

#endif

/* EOF */
