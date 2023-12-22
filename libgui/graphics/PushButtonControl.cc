////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2011-2024 The Octave Project Developers
//
// See the file COPYRIGHT.md in the top-level directory of this
// distribution or <https://octave.org/copyright/>.
//
// This file is part of Octave.
//
// Octave is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Octave is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Octave; see the file COPYING.  If not, see
// <https://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////

#if defined (HAVE_CONFIG_H)
#  include "config.h"
#endif

#include <QPushButton>
#include <QtDebug>

#include "PushButtonControl.h"
#include "Container.h"
#include "QtHandlesUtils.h"

OCTAVE_BEGIN_NAMESPACE(octave)

PushButtonControl *
PushButtonControl::create (octave::interpreter& interp,
                           const graphics_object& go)
{
  Object *parent = parentObject (interp, go);

  if (parent)
    {
      Container *container = parent->innerContainer ();

      if (container)
        return new PushButtonControl (interp, go,
                                      new QPushButton (container));
    }

  return nullptr;
}

PushButtonControl::PushButtonControl (octave::interpreter& interp,
                                      const graphics_object& go,
                                      QPushButton *btn)
  : ButtonControl (interp, go, btn)
{
  uicontrol::properties& up = properties<uicontrol> ();

  btn->setAutoFillBackground (true);
  octave_value cdat = up.get_cdata ();
  QImage img = Utils::makeImageFromCData (cdat,
                                          cdat.columns (), cdat.rows ());
  btn->setIcon (QIcon (QPixmap::fromImage (img)));
  btn->setIconSize (QSize (cdat.columns (), cdat.rows ()));
}

PushButtonControl::~PushButtonControl ()
{ }

void
PushButtonControl::update (int pId)
{
  uicontrol::properties& up = properties<uicontrol> ();
  QPushButton *btn = qWidget<QPushButton> ();

  switch (pId)
    {
    case uicontrol::properties::ID_CDATA:
      {
        octave_value cdat = up.get_cdata ();
        QImage img = Utils::makeImageFromCData (cdat,
                                                cdat.rows (),
                                                cdat.columns ());
        btn->setIcon (QIcon (QPixmap::fromImage (img)));
        btn->setIconSize (QSize (cdat.columns (), cdat.rows ()));
      }
      break;

    default:
      ButtonControl::update (pId);
      break;
    }
}

OCTAVE_END_NAMESPACE(octave)
