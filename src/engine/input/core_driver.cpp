// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
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

#include "engine/input/core_driver.hpp"

#include "engine/input/manager.hpp"

namespace pingus::input {

class AxisPointer : public Pointer
{
private:
  std::unique_ptr<Axis> x_axis;
  std::unique_ptr<Axis> y_axis;
  std::unique_ptr<Button> speed_button;
  float speed;

public:
  AxisPointer(Control* parent_) :
    Pointer(parent_),
    x_axis(), y_axis(), speed_button(),
    speed(400.0f)
  {
  }

  ~AxisPointer() override
  {
  }

  void setup(std::unique_ptr<Axis> x, std::unique_ptr<Axis> y, std::unique_ptr<Button> s = {})
  {
    x_axis = std::move(x);
    y_axis = std::move(y);
    speed_button = std::move(s);
  }

  void update(Control* ) override
  {
    //log_error("event");
  }

  void update(float delta) override
  {
    x_axis->update(delta);
    y_axis->update(delta);
    if (speed_button) speed_button->update(delta);

    geom::fpoint new_pos = pos;
    float c_speed = speed;

    if (speed_button && speed_button->get_state() == BUTTON_PRESSED)
    {
      c_speed *= 5.0f;
    }

    new_pos += geom::foffset(x_axis->get_pos() * c_speed * delta,
                             y_axis->get_pos() * c_speed * delta);

    new_pos = geom::clamp(new_pos, m_limits);

    if (new_pos != pos)
    {
      pos = new_pos;
      notify_parent();
    }
  }

private:
  AxisPointer(AxisPointer const&);
  AxisPointer & operator=(AxisPointer const&);
};

class AxisScroller : public Scroller
{
private:
  std::unique_ptr<Axis> x_axis;
  std::unique_ptr<Axis> y_axis;
  std::unique_ptr<Button> speed_button;
  float speed;

public:
  AxisScroller(Control* parent_) :
    Scroller(parent_),
    x_axis(), y_axis(), speed_button(),
    speed(800.0f)
  {
  }

  ~AxisScroller() override
  {
  }

  void setup(std::unique_ptr<Axis> x, std::unique_ptr<Axis> y, std::unique_ptr<Button> s = {})
  {
    x_axis = std::move(x);
    y_axis = std::move(y);
    speed_button = std::move(s);
  }

  void update(Control* ) override
  {
    //log_error("event");
  }

  void update(float delta_) override
  {
    x_axis->update(delta_);
    y_axis->update(delta_);

    if (speed_button) speed_button->update(delta_);

    float    c_speed = speed;

    if (speed_button && speed_button->get_state() == BUTTON_PRESSED)
    {
      c_speed *= 5.0f;
    }

    this->delta = {-x_axis->get_pos() * c_speed * delta_,
      y_axis->get_pos() * c_speed * delta_};

    notify_parent();
  }

private:
  AxisScroller(AxisScroller const&);
  AxisScroller & operator=(AxisScroller const&);
};

class ButtonScroller : public Scroller
{
private:
  std::unique_ptr<Button> up;
  std::unique_ptr<Button> down;
  std::unique_ptr<Button> left;
  std::unique_ptr<Button> right;
  float speed;

public:
  ButtonScroller(Control* parent_) :
    Scroller(parent_),
    up(), down(), left(), right(),
    speed(800.0f)
  {
  }

  ~ButtonScroller() override
  {
  }

  void setup(std::unique_ptr<Button> up_,
             std::unique_ptr<Button> down_,
             std::unique_ptr<Button> left_,
             std::unique_ptr<Button> right_)
  {
    up = std::move(up_);
    down = std::move(down_);
    left = std::move(left_);
    right = std::move(right_);
  }

  void update(Control* ) override
  {
  }

  void update(float delta_t) override
  {
    if (up) up->update(delta_t);
    if (down) down->update(delta_t);
    if (left) left->update(delta_t);
    if (right) right->update(delta_t);

    float delta_x = 0.0f;
    float delta_y = 0.0f;

    if (left && left->get_state() == BUTTON_PRESSED)
      delta_x += speed * delta_t;

    if (right && right->get_state() == BUTTON_PRESSED)
      delta_x += -speed * delta_t;

    if (up && up->get_state() == BUTTON_PRESSED)
      delta_y += speed * delta_t;

    if (down && down->get_state() == BUTTON_PRESSED)
      delta_y += -speed * delta_t;

    delta = geom::fpoint(delta_x, delta_y);

    notify_parent();
  }

private:
  ButtonScroller(ButtonScroller const&);
  ButtonScroller & operator=(ButtonScroller const&);
};

std::unique_ptr<Button>
CoreDriver::create_button(prio::ReaderObject const& reader_object, Control* parent)
{
  return {};
}

std::unique_ptr<Axis>
CoreDriver::create_axis(prio::ReaderObject const& reader_object, Control* parent)
{
  return {};
}

std::unique_ptr<Scroller>
CoreDriver::create_scroller(prio::ReaderObject const& reader_object, Control* parent)
{
  if (reader_object.get_name() == "core:axis-scroller")
  {
    prio::ReaderMapping reader = reader_object.get_mapping();

    auto axis = std::make_unique<AxisScroller>(parent);

    prio::ReaderObject x_reader;
    if (!reader.read("x-axis", x_reader))
    {
      log_error("CoreDriver: Couldn't find x-axis");
      return {};
    }

    prio::ReaderObject y_reader;
    if (!reader.read("y-axis", y_reader))
    {
      log_error("CoreDriver: Couldn't find y-axis");
      return {};
    }

    std::unique_ptr<Axis> x_axis = manager->create_axis(x_reader, axis.get());
    std::unique_ptr<Axis> y_axis = manager->create_axis(y_reader, axis.get());

    std::unique_ptr<Button> button;
    prio::ReaderObject button_reader;
    if (reader.read("button", button_reader))
    {
      button = manager->create_button(button_reader, axis.get());
    }

    if (x_axis && y_axis)
    {
      axis->setup(std::move(x_axis), std::move(y_axis), std::move(button));
      return axis;
    }
    else
    {
      return {};
    }
  }
  else if (reader_object.get_name() == "core:button-scroller")
  {
    prio::ReaderMapping reader = reader_object.get_mapping();

    auto scroller = std::make_unique<ButtonScroller>(parent);

    prio::ReaderObject left_reader;
    if (!reader.read("left", left_reader))
    {
      log_error("CoreDriver: core:button-scroller: Couldn't find 'left'");
      return {};
    }

    prio::ReaderObject right_reader;
    if (!reader.read("right", right_reader))
    {
      log_error("CoreDriver: core:button-scroller: Couldn't find 'right'");
      return {};
    }

    prio::ReaderObject up_reader;
    if (!reader.read("up", up_reader))
    {
      log_error("CoreDriver: core:button-scroller: Couldn't find 'up'");
      return {};
    }

    prio::ReaderObject down_reader;
    if (!reader.read("down", down_reader))
    {
      log_error("CoreDriver: core:button-scroller: Couldn't find 'down'");
      return {};
    }

    // FIXME: Add more error checking
    auto up_button = manager->create_button(up_reader, scroller.get());
    auto down_button = manager->create_button(down_reader, scroller.get());
    auto left_button = manager->create_button(left_reader, scroller.get());
    auto right_button = manager->create_button(right_reader, scroller.get());

    scroller->setup(std::move(up_button),
                    std::move(down_button),
                    std::move(left_button),
                    std::move(right_button));
    return scroller;
  }
  else
  {
    return {};
  }
}

std::unique_ptr<Pointer>
CoreDriver::create_pointer(prio::ReaderObject const& reader_object, Control* parent)
{
  if (reader_object.get_name() == "core:axis-pointer")
  {
    prio::ReaderMapping reader = reader_object.get_mapping();

    auto axis = std::make_unique<AxisPointer>(parent);

    prio::ReaderObject x_reader;
    if (!reader.read("x-axis", x_reader))
    {
      log_error("CoreDriver: Couldn't find x-axis");
      return {};
    }

    prio::ReaderObject y_reader;
    if (!reader.read("y-axis", y_reader))
    {
      log_error("CoreDriver: Couldn't find y-axis");
      return {};
    }

    std::unique_ptr<Axis> x_axis = manager->create_axis(x_reader, axis.get());
    std::unique_ptr<Axis> y_axis = manager->create_axis(y_reader, axis.get());

    std::unique_ptr<Button> button;
    prio::ReaderObject button_reader;
    if (reader.read("button", button_reader))
    {
      button = manager->create_button(button_reader, axis.get());
    }

    if (x_axis && y_axis)
    {
      axis->setup(std::move(x_axis), std::move(y_axis), std::move(button));
      return axis;
    }
    else
    {
      return {};
    }
  }
  else
  {
    return {};
  }
}

std::unique_ptr<Keyboard>
CoreDriver::create_keyboard(prio::ReaderObject const& reader, Control* parent)
{
  return {};
}

} // namespace pingus::input

/* EOF */
