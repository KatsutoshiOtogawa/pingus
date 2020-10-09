// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "reader/overrride_reader.hpp"

#include <set>

#include <geom/size.hpp>

#include "reader/reader.hpp"
#include "reader/reader_impl.hpp"

class OverrideReaderMappingImpl : public ReaderMappingImpl
{
private:
  ReaderMapping m_reader;
  ReaderMapping m_overrides;

public:
  OverrideReaderMappingImpl(const ReaderMapping& reader,
                            const ReaderMapping& overrides) :
    m_reader(reader),
    m_overrides(overrides)
  {
  }

  std::vector<std::string> get_keys() const override
  {
    std::set<std::string> result;

    auto lst = m_reader.get_keys();
    result.insert(lst.begin(), lst.end());
    lst = m_overrides.get_keys();
    result.insert(lst.begin(), lst.end());

    return std::vector<std::string>(result.begin(), result.end());
  }

  bool read_int(const char* name, int& v) const override
  {
    if (m_overrides.read_int(name, v))
    {
      return true;
    }
    else
    {
      return m_reader.read_int(name, v);
    }
  }

  bool read_float(const char* name, float& v) const override
  {
    if (m_overrides.read_float(name, v))
    {
      return true;
    }
    else
    {
      return m_reader.read_float(name, v);
    }
  }

  bool read_bool(const char* name, bool& v) const override
  {
    if (m_overrides.read_bool(name, v))
    {
      return true;
    }
    else
    {
      return m_reader.read_bool(name, v);
    }
  }

  bool read_string(const char* name, std::string& str) const override
  {
    if (m_overrides.read_string(name, str))
    {
      return true;
    }
    else
    {
      return m_reader.read_string(name, str);
    }
  }

  bool read_vector(const char* name, Vector2f& vec, float& z_index) const override
  {
    if (m_overrides.read_vector(name, vec, z_index))
    {
      return true;
    }
    else
    {
      return m_reader.read_vector(name, vec, z_index);
    }
  }

  bool read_size(const char* name, geom::isize& size) const override
  {
    if (m_overrides.read_size(name, size))
    {
      return true;
    }
    else
    {
      return m_reader.read_size(name, size);
    }
  }

  bool read_vector2i(const char* name, Vector2i& vec) const override
  {
    if (m_overrides.read_vector2i(name, vec))
    {
      return true;
    }
    else
    {
      return m_reader.read_vector2i(name, vec);
    }
  }

  bool read_rect(const char* name, Rect& rect) const override
  {
    if (m_overrides.read_rect(name, rect))
    {
      return true;
    }
    else
    {
      return m_reader.read_rect(name, rect);
    }
  }

  bool read_colorf(const char* name, Colorf& color) const override
  {
    if (m_overrides.read_colorf(name, color))
    {
      return true;
    }
    else
    {
      return m_reader.read_colorf(name, color);
    }
  }

  bool read_colori(const char* name, Color& color) const override
  {
    if (m_overrides.read_colori(name, color))
    {
      return true;
    }
    else
    {
      return m_reader.read(name, color);
    }
  }

  bool read_mapping(const char* name, ReaderMapping& result) const override
  {
    ReaderMapping overwrite_result;
    if (m_overrides.read(name, overwrite_result))
    {
      if (m_reader.read(name, result))
      {
        result = make_override_mapping(result, overwrite_result);
        return true;
      }
      else
      {
        result = overwrite_result;
        return true;
      }
    }
    else
    {
      return m_reader.read(name, result);
    }
  }

  bool read_collection(const char* key, ReaderCollection& result) const override
  {
    if (m_overrides.read_collection(key, result))
    {
      return true;
    }
    else
    {
      return m_reader.read(key, result);
    }
  }

  bool read_object(const char* key, ReaderObject& result) const override
  {
    if (m_overrides.read_object(key, result))
    {
      return true;
    }
    else
    {
      return m_reader.read(key, result);
    }
  }
};

ReaderMapping
make_override_mapping(const ReaderMapping& reader, const ReaderMapping& overrides)
{
  return ReaderMapping(std::make_shared<OverrideReaderMappingImpl>(reader, overrides));
}

/* EOF */
